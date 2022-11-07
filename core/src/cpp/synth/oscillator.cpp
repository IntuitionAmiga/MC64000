/**
 *   888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
 *   8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
 *   88888b.d88888 888    888 888          d8P 888  888  d8P
 *   888Y88888P888 888        888d888b.   d8P  888  888d88K
 *   888 Y888P 888 888        888P "Y88b d88   888  8888888b
 *   888  Y8P  888 888    888 888    888 8888888888 888  Y88b
 *   888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
 *   888       888  "Y8888P"   "Y8888P"        888  888    Y88b
 *
 *    - 64-bit 680x0-inspired Virtual Machine and assembler -
 */

#include <cmath>
#include <cstdio>
#include <synth/note.hpp>
#include <synth/signal/waveform/constants.hpp>
#include <synth/signal/waveform/sine.hpp>
#include <synth/signal/waveform/triangle.hpp>
#include <synth/signal/waveform/saw.hpp>
#include <synth/signal/waveform/square.hpp>

#include <synth/signal/oscillator.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;

/**
 * @inheritDoc
 */
IOscillator::IOscillator(
    IWaveform::Ptr const& pWaveform,
    float32 fInitialFrequency,
    float32 fInitialPhase
):
    fTimeStep(SAMPLE_PERIOD),
    fPhaseOffset(fInitialPhase),
    fPhaseCorrection(fInitialPhase),
    fWaveformPeriod(1.0f)
{
    setFrequency(fInitialFrequency);
    setWaveform(pWaveform);
}

/**
 * @inheritDoc
 */
IOscillator* IOscillator::reset() {
    uSamplePosition   = 0;
    fCurrentFrequency = fFrequency;
    fPhaseCorrection  = fPhaseOffset;
    return this;
}

/**
 * @inheritDoc
 */
IOscillator* IOscillator::setWaveform(IWaveform::Ptr const& pNewWaveform) {
    if (pNewWaveform.get()) {
        pWaveform       = pNewWaveform->copy();
        fWaveformPeriod = pWaveform->getPeriod();
        fTimeStep       = fWaveformPeriod * SAMPLE_PERIOD;
        fScaleVal       = fTimeStep * fFrequency;
        bAperiodic      = false;
    } else {
        pWaveform       = 0;
        fWaveformPeriod = 1.0f;
        fTimeStep       = fWaveformPeriod * SAMPLE_PERIOD;
        bAperiodic      = false;
    }
    return this;
}

/**
 * @inheritDoc
 */
IOscillator* IOscillator::setFrequency(float32 fNewFrequency) {
    fCurrentFrequency = fFrequency = clampFrequency(fNewFrequency);
    fScaleVal         = fTimeStep * fFrequency;
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr IOscillator::emit(size_t uIndex) {
    if (!bEnabled || !pWaveform.get()) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return pLastPacket;
    }
    return emitNew();
}

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////


#include <synth/signal/oscillator/LFO.hpp>
namespace MC64K::Synth::Audio::Signal::Oscillator {

LFO::LFO(
    IWaveform::Ptr const& pWaveform,
    float32 fFrequency,
    float32 fDepth,
    bool    bRetrigger
) :
    IOscillator(pWaveform, fFrequency, 0.0f),
    fDepth(fDepth),
    fBias(0.0f),
    bRetrigger(bRetrigger)
{
    setFrequency(fFrequency);
    std::fprintf(stderr, "Created LFO at %p\n", this);
}

/**
 * @inheritDoc
 */
LFO::~LFO() {
    std::fprintf(stderr, "Destroyed LFO at %p\n", this);
}

/**
 * @inheritDoc
 */
float32 LFO::clampFrequency(float32 fNewFrequency) {
    return fNewFrequency < MIN_FREQUENCY ? MIN_FREQUENCY : (
        fNewFrequency > MAX_FREQUENCY ?
        MAX_FREQUENCY : fNewFrequency
    );
}

Packet const* LFO::generateCommon() {
    // Recycle our last output packet if we have one
    Packet::Ptr pInput   = pLastPacket.get() ? pLastPacket : Packet::create();
    float32* pSamples    = pInput->aSamples;
    float32  fCorrection = fPhaseCorrection;
    uint32   uCounter    = getCyclicSampleCounter();
    for (unsigned u=0; u < PACKET_SIZE; ++u) {
        pSamples[u] = (float32)(fScaleVal * uCounter++) + fCorrection;
    }
    uSamplePosition += PACKET_SIZE;
    handleCyclicSampleCounterReset(pSamples[PACKET_SIZE - 1]);
    return pInput.get();
}

/**
 * @inheritDoc
 */
Packet::ConstPtr LFO::emitNew() {
    pLastPacket = pWaveform->map(generateCommon());
    pLastPacket->scaleBy(fDepth);
    return pLastPacket;
}


/**
 * @inheritDoc
 */
Packet::ConstPtr LFOZeroToOne::emitNew() {
    pLastPacket = pWaveform->map(generateCommon());
    pLastPacket->scaleAndBiasBy(Waveform::HALF * fDepth, Waveform::HALF * fDepth);
    return pLastPacket;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr LFOOneToZero::emitNew() {
    pLastPacket = pWaveform->map(generateCommon());
    pLastPacket->scaleAndBiasBy(
        Waveform::HALF * fDepth,
        Waveform::ONE - Waveform::HALF * fDepth
    );
    return pLastPacket;
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/oscillator/sound.hpp>
namespace MC64K::Synth::Audio::Signal::Oscillator {

Sound::Sound(
    IWaveform::Ptr const& pWaveform,
    float32 fFrequency,
    float32 fPhase
) :
    IOscillator(pWaveform, fFrequency, fPhase)
{
    setFrequency(fFrequency);
    configureInputStage();
    configureOutputStage();
    configureAntialias();
    std::fprintf(stderr, "Created Oscillator::Sound at %p\n", this);
}

/**
 * @inheritDoc
 */
Sound::~Sound() {
    std::fprintf(stderr, "Destroyed Oscillator::Sound at %p\n", this);
}

/**
 * @inheritDoc
 */
Sound* Sound::reset() {
    IOscillator::reset();
    if (pPitchModulator.get()) {
        pPitchModulator->reset();
    }
    if (pPhaseModulator.get()) {
        pPhaseModulator->reset();
    }
    if (pLevelModulator.get()) {
        pLevelModulator->reset();
    }
    if (pLevelEnvelope.get()) {
        pLevelEnvelope->reset();
    }
    if (pPitchEnvelope.get()) {
        pPitchEnvelope->reset();
    }
    return this;
}

/**
 * @inheritDoc
 */
float32 Sound::clampFrequency(float32 fNewFrequency) {
    return fNewFrequency < MIN_FREQUENCY ? MIN_FREQUENCY : (
        fNewFrequency > MAX_FREQUENCY ?
        MAX_FREQUENCY : fNewFrequency
    );
}

/**
 * @inheritDoc
 */
void Sound::populatePitchShiftedPacket(Packet const* pPitchShifts) {
    // Pitch Samples are in relative semitones, e.g. 2.0 is a whole note increase.
    float32 const* pPitchSamples     = pPitchShifts->aSamples;
    float32*       pInputSamples     = pLastPacket->aSamples;

    // Use higher definition calculation for time
    float64        fInstantFrequency = fCurrentFrequency;
    uint32         uCounter          = getCyclicSampleCounter();
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        // Calculate frequency of next sample after applying the semitone delta.
        float64 fNextFrequency = fFrequency * Note::semisToMultiplier(pPitchSamples[u]);

        // Evaluate the point in time
        float64 fTime          = fTimeStep * uCounter++;

        // Apply the phase adjustetment
        pInputSamples[u]       = (float32)(fInstantFrequency * fTime) + fPhaseCorrection;

        // Update the phase adjustment and frequency
        fPhaseCorrection      += (float32)(fTime * (fInstantFrequency - fNextFrequency));
        fInstantFrequency      = fNextFrequency;
    }
    fCurrentFrequency = (float32)fInstantFrequency;
    uSamplePosition += PACKET_SIZE;
    handleCyclicSampleCounterReset(pInputSamples[PACKET_SIZE - 1]);
}

/**
 * @inheritDoc
 */
void Sound::populatePitchAndPhaseShiftedPacket(
    Packet const* pPitchShifts,
    Packet const* pPhaseShifts
) {
    // Pitch Samples are in relative semitones, e.g. 2.0 is a whole note increase.
    float32 const* pPitchSamples     = pPitchShifts->aSamples;
    float32 const* pPhaseSamples     = pPhaseShifts->aSamples;
    float32*       pInputSamples     = pLastPacket->aSamples;

    // Use higher definition calculation for time
    float64        fInstantFrequency = fCurrentFrequency;
    float32        fPeriod           = fPhaseModulationIndex * fWaveformPeriod;
    uint32         uCounter          = getCyclicSampleCounter();
    float32        fCorrection       = fPhaseCorrection;
    float32        fBasePhase        = 0.0f;
    float32        fPhaseShift       = 0.0f;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        // Calculate frequency of next sample after applying the semitone delta.
        float64 fNextFrequency = fFrequency * Note::semisToMultiplier(pPitchSamples[u]);

        // Evaluate the point in time
        float64 fTime = fTimeStep * uCounter++;

        // Calculate the changing base phase without any contribution from modulation
        fBasePhase = (float32)(fInstantFrequency * fTime) + fCorrection;

        // Store the sample
        pInputSamples[u] = fBasePhase + (fPeriod * pPhaseSamples[u]) + fPhaseShift;

        // Update the phase adjustment and frequency
        fPhaseShift           += (float32)(fTime * (fInstantFrequency - fNextFrequency));
        fInstantFrequency      = fNextFrequency;
    }
    fPhaseCorrection += fPhaseShift;
    fCurrentFrequency = (float32)fInstantFrequency;
    uSamplePosition += PACKET_SIZE;

    // Only the base phase and shift contributions need to be applied when we reset the counter
    handleCyclicSampleCounterReset(fBasePhase + fPhaseShift);
}

/**
 * @inheritDoc
 *
 * This is the expensive one - we have to call the value() operation on the waveform.
 */
void Sound::populateOutputPacketWithFeedback(Packet const* pLevelPacket) {
    float32        fIndex        = fPhaseFeedbackIndex * FEEDBACK_SCALE;
    float32*       pSamples      = pLastPacket->aSamples;
    float32 const* pLevelSamples = pLevelPacket->aSamples;
    IWaveform*     pWave         = pWaveform.get();

    float32 fFb1 = fFeedback1;
    float32 fFb2 = fFeedback2;

    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        float32 fOutput = pWave->value(
            pSamples[u] +
            fIndex * (fFb1 + fFb2)
        ) * pLevelSamples[u];
        pSamples[u] = fOutput;
        fFb2  = fFb1;
        fFb1  = fOutput;
    }

    fFeedback1 = fFb1;
    fFeedback2 = fFb2;
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is no pitch/phase modulation
 */
void Sound::inputAperiodic(Sound* pOscillator) {
    pOscillator->uSamplePosition += PACKET_SIZE;
}


/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is no pitch/phase modulation
 */
void Sound::inputDirect(Sound* pOscillator) {
    float32* pSamples    = pOscillator->pLastPacket->aSamples;
    float32  fCorrection = pOscillator->fPhaseCorrection;
    uint32   uCounter    = pOscillator->getCyclicSampleCounter();
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        pSamples[u] = (float32)(pOscillator->fScaleVal * uCounter++) + fCorrection;
    }
    pOscillator->uSamplePosition += PACKET_SIZE;
    pOscillator->handleCyclicSampleCounterReset(pSamples[PACKET_SIZE - 1]);
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is only pitch modulation
 */
void Sound::inputPitchMod(Sound* pOscillator) {
    pOscillator->populatePitchShiftedPacket(
        pOscillator
            ->pPitchModulator
            ->emit(pOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is only pitch envelope
 */
void Sound::inputPitchEnv(Sound* pOscillator) {
    pOscillator->populatePitchShiftedPacket(
        pOscillator
            ->pPitchEnvelope
            ->emit(pOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is both pitch modulation and envelope
 */
void Sound::inputPitchModEnv(Sound* pOscillator) {
    Packet::Ptr pPitchShifts = pOscillator
        ->pPitchModulator
        ->emit(pOscillator->uLastIndex)
        ->clone();
    pPitchShifts->sumWith(
        pOscillator
            ->pPitchEnvelope
            ->emit(pOscillator->uLastIndex).get()
    );
    pOscillator->populatePitchShiftedPacket(pPitchShifts.get());
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is only phase modulation
 */
void Sound::inputPhaseMod(Sound* pOscillator) {
    float32 const* pPhaseShifts = pOscillator
        ->pPhaseModulator
        ->emit(pOscillator->uLastIndex)
        ->aSamples;
    float32* pSamples    = pOscillator->pLastPacket->aSamples;
    float32  fPeriod     = pOscillator->fPhaseModulationIndex * pOscillator->fWaveformPeriod;
    float32  fCorrection = pOscillator->fPhaseCorrection;
    uint32   uCounter    = pOscillator->getCyclicSampleCounter();
    float32  fBasePhase;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        fBasePhase = (float32)(pOscillator->fScaleVal * uCounter++) + fCorrection;
        pSamples[u] = fBasePhase + fPeriod * pPhaseShifts[u];
    }
    pOscillator->uSamplePosition += PACKET_SIZE;
    pOscillator->handleCyclicSampleCounterReset(fBasePhase);
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is phase and pitch modulation
 */
void Sound::inputPhaseModPitchMod(Sound* pOscillator) {
    pOscillator->populatePitchAndPhaseShiftedPacket(
        pOscillator
            ->pPitchModulator
            ->emit(pOscillator->uLastIndex).get(),
        pOscillator
            ->pPhaseModulator
            ->emit(pOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is phase modulation and pitch envelope
 */
void Sound::inputPhaseModPitchEnv(Sound* pOscillator) {
    pOscillator->populatePitchAndPhaseShiftedPacket(
        pOscillator
        ->pPitchEnvelope
        ->emit(pOscillator->uLastIndex).get(),
        pOscillator
        ->pPhaseModulator
        ->emit(pOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is phase modulation and both pitch
 * modulation and envelope
 */
void Sound::inputPhaseModPitchModEnv(Sound* pOscillator) {
    Packet::Ptr pPitchShifts = pOscillator
        ->pPitchModulator
        ->emit(pOscillator->uLastIndex)
        ->clone();
    pPitchShifts->sumWith(
        pOscillator
            ->pPitchEnvelope
            ->emit(pOscillator->uLastIndex).get()
        );
    pOscillator->populatePitchAndPhaseShiftedPacket(
        pPitchShifts.get(),
        pOscillator
        ->pPhaseModulator
        ->emit(pOscillator->uLastIndex).get()
    );
}

/**
 * Input stage table
 */
Sound::Stage Sound::aInputStages[8] = {
    inputDirect,
    inputPitchMod,
    inputPitchEnv,
    inputPitchModEnv,
    inputPhaseMod,
    inputPhaseModPitchMod,
    inputPhaseModPitchEnv,
    inputPhaseModPitchModEnv
};

char const* aInputStageNames[8] = {
    "-- -- --",
    "-- PM --",
    "-- -- PE",
    "-- PM PE",
    "FM -- --",
    "FM PM --",
    "FM -- PE",
    "FM PM PE"
};

/**
 * @inheritDoc
 */
void Sound::configureInputStage() {
    if (pWaveform.get() && pWaveform->isAperiodic()) {
        cInput = &inputAperiodic;
        std::fprintf(
            stderr,
            "Oscillator::Sound::configureInputStage(): Waveform %d is aperiodic\n",
            pWaveform->getShape()
        );

    } else {
        unsigned uUseStage =
            (pPitchModulator.get() ? 1 : 0) |
            (pPitchEnvelope.get()  ? 2 : 0) |
            ((pPhaseModulator.get() && MIN_PHASE_MOD_INDEX < fPhaseModulationIndex) ? 4 : 0);
        cInput = aInputStages[uUseStage];
        std::fprintf(
            stderr,
            "Oscillator::Sound::configureInputStage(): %u [%s]\n",
            uUseStage,
            aInputStageNames[uUseStage]
        );
    }
}

/**
 * @inheritDoc
 */
void Sound::outputDirect(Sound* pOscillator) {
    pOscillator->pLastPacket = pOscillator->pWaveform->map(
        pOscillator->pLastPacket.get()
    );
}

/**
 * @inheritDoc
 */
void Sound::outputLevelMod(Sound* pOscillator) {
    pOscillator->pLastPacket = pOscillator->pWaveform->map(
        pOscillator->pLastPacket.get()
    );
    Packet::Ptr pOutputLevel = pOscillator
        ->pLevelModulator
        ->emit(pOscillator->uLastIndex)
        ->clone();
    pOutputLevel->scaleBy(pOscillator->fLevelModulationIndex);
    pOscillator
        ->pLastPacket
        ->modulateWith(pOutputLevel.get());
}

/**
 * @inheritDoc
 */
void Sound::outputLevelEnv(Sound* pOscillator) {
    pOscillator->pLastPacket = pOscillator->pWaveform->map(
        pOscillator->pLastPacket.get()
    );
    pOscillator->pLastPacket->modulateWith(
        pOscillator
            ->pLevelEnvelope
            ->emit(pOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 */
void Sound::outputLevelModEnv(Sound* pOscillator) {
    pOscillator->pLastPacket = pOscillator->pWaveform->map(
        pOscillator->pLastPacket.get()
    );
    Packet::Ptr pOutputLevel = pOscillator
        ->pLevelModulator
        ->emit(pOscillator->uLastIndex)
        ->clone();
    pOutputLevel
        ->scaleBy(pOscillator->fLevelModulationIndex)
        ->modulateWith(
            pOscillator
                ->pLevelModulator
                ->emit(pOscillator->uLastIndex).get()
          );
    pOscillator
        ->pLastPacket
        ->modulateWith(pOutputLevel.get());
}

/**
 * @inheritDoc
 */
void Sound::outputFeedback(Sound* pOscillator) {
    float32    fIndex   = pOscillator->fPhaseFeedbackIndex * FEEDBACK_SCALE;
    float32*   pSamples = pOscillator->pLastPacket->aSamples;
    IWaveform* pWave    = pOscillator->pWaveform.get();

    float32 fFeedback1 = pOscillator->fFeedback1;
    float32 fFeedback2 = pOscillator->fFeedback2;

    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        float32 fOutput = pWave->value(
            pSamples[u] +
            fIndex * (fFeedback1 + fFeedback2)
        );
        pSamples[u] = fOutput;
        fFeedback2  = fFeedback1;
        fFeedback1  = fOutput;
    }

    pOscillator->fFeedback1 = fFeedback1;
    pOscillator->fFeedback2 = fFeedback2;
}

/**
 * @inheritDoc
 */
void Sound::outputFeedbackLevelMod(Sound* pOscillator) {
    Packet::Ptr pOutputLevel = pOscillator
        ->pLevelModulator
        ->emit(pOscillator->uLastIndex)
        ->clone();
    pOutputLevel->scaleBy(pOscillator->fLevelModulationIndex);
    pOscillator->populateOutputPacketWithFeedback(pOutputLevel.get());
}

/**
 * @inheritDoc
 */
void Sound::outputFeedbackLevelEnv(Sound* pOscillator) {
    Packet::Ptr pOutputLevel = pOscillator
        ->pLevelEnvelope
        ->emit(pOscillator->uLastIndex)
        ->clone();
    pOscillator->populateOutputPacketWithFeedback(pOutputLevel.get());
}

/**
 * @inheritDoc
 */
void Sound::outputFeedbackLevelModEnv(Sound* pOscillator) {
    Packet::Ptr pOutputLevel = pOscillator
        ->pLevelModulator
        ->emit(pOscillator->uLastIndex)
        ->clone();
    pOutputLevel->scaleBy(pOscillator->fLevelModulationIndex);
    pOutputLevel->modulateWith(
        pOscillator
            ->pLevelEnvelope
            ->emit(pOscillator->uLastIndex).get()
    );
    pOscillator->populateOutputPacketWithFeedback(pOutputLevel.get());
}

/**
 * Output stage table
 */
Sound::Stage Sound::aOutputStages[8] = {
    outputDirect,
    outputLevelMod,
    outputLevelEnv,
    outputLevelModEnv,
    outputFeedback,
    outputFeedbackLevelMod,
    outputFeedbackLevelEnv,
    outputFeedbackLevelModEnv
};

char const* aOutputStageNames[8] = {
    "-- -- --",
    "-- LM --",
    "-- -- LE",
    "-- LM LE",
    "FB -- --",
    "FB LM --",
    "FB -- LE",
    "FB LM LE"
};

/**
 * @inheritDoc
 */
void Sound::configureOutputStage() {
    unsigned uUseStage =
        ((pLevelModulator.get() && MIN_LEVEL_MOD_INDEX < fLevelModulationIndex) ? 1 : 0) |
        (pLevelEnvelope.get() ? 2 : 0) |
        ((!bAperiodic && MIN_FEEDBACK_MOD_INDEX < fPhaseFeedbackIndex) ? 4 : 0);
    cOutput = aOutputStages[uUseStage];
    std::fprintf(
        stderr,
        "Oscillator::Sound::configureOutputStage(): %u [%s]\n",
        uUseStage,
        aOutputStageNames[uUseStage]
    );
}

void Sound::configureAntialias() {
    switch (eAAMode) {
        case AA_OFF:
            bAntialias = false;
            break;
        case AA_ON:
            bAntialias = true;
            break;
        case AA_AUTO:
            bAntialias = (pWaveform.get() && pWaveform->isDiscontinuous());
            break;
        default:
            break;
    }
    std::fprintf(
        stderr,
            "Oscillator::Sound::configureAntialias(): %d [%s]\n",
            eAAMode,
            (bAntialias ? "enabled" : "disabled")
    );
}

/**
 * @inheritDoc
 */
Packet::ConstPtr Sound::emitNew() {
    if (!pLastPacket.get()) {
        pLastPacket = Packet::create();
    }
    cInput(this);
    cOutput(this);

    if (bAntialias) {
        /**
         * Apply a 5 sample travelling hamming window over the output
         */
        float32 fPrev1  = fAAPrev1;
        float32 fPrev2  = fAAPrev2;
        float32 fPrev3  = fAAPrev3;
        float32 fPrev4  = fAAPrev4;
        Packet::Ptr pOutputPacket = Packet::create();

        float32* aUnfiltered = pLastPacket->aSamples;
        float32* aFiltered   = pOutputPacket->aSamples;

        for (unsigned u=0; u < PACKET_SIZE; ++u) {
            float32 fSample = aUnfiltered[u];
            aFiltered[u] = 0.0625f * (
                fSample + fPrev4 +
                3.0f * (fPrev1 + fPrev3)
                + 8.0f * fPrev2
            );
            fPrev4 = fPrev3;
            fPrev3 = fPrev2;
            fPrev2 = fPrev1;
            fPrev1 = fSample;
        }
        fAAPrev1 = fPrev1;
        fAAPrev2 = fPrev2;
        fAAPrev3 = fPrev3;
        fAAPrev4 = fPrev4;
        pLastPacket = pOutputPacket;
    }
    return pLastPacket;
}


} // namespace
