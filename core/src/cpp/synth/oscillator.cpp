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
    IWaveform::Ptr const& roWaveformPtr,
    float32 fInitialFrequency,
    float32 fInitialPhase
):
    TOutputStream{},
    oWaveInputPacketPtr{Packet::create()},
    poWaveform{nullptr},
    fTimeStep{SAMPLE_PERIOD},
    fPhaseOffset{fInitialPhase},
    fPhaseCorrection{fInitialPhase},
    fWaveformPeriod{1.0f}
{
    setFrequency(fInitialFrequency);
    setWaveform(roWaveformPtr);
}

/**
 * @inheritDoc
 *
 * We only allow the oscillator to be enabled if it has a waveform.
 */
bool IOscillator::canEnable() const noexcept {
    return poWaveform != nullptr;
}

/**
 * @inheritDoc
 */
IOscillator* IOscillator::reset() noexcept {
    uSamplePosition   = 0;
    fCurrentFrequency = fFrequency;
    fPhaseCorrection  = fPhaseOffset;
    return this;
}

/**
 * @inheritDoc
 */
IOscillator* IOscillator::setWaveform(IWaveform::Ptr const& roNewWaveformPtr) noexcept {
    if (roNewWaveformPtr.get()) {
        oWaveformPtr    = roNewWaveformPtr->copy();
        poWaveform      = oWaveformPtr.get();
        fWaveformPeriod = poWaveform->getPeriod();
        fTimeStep       = fWaveformPeriod * SAMPLE_PERIOD;
        fScaleVal       = fTimeStep * fFrequency;
        bAperiodic      = false;
    } else {
        oWaveformPtr    = nullptr;
        poWaveform      = nullptr;
        fWaveformPeriod = 1.0f;
        fTimeStep       = fWaveformPeriod * SAMPLE_PERIOD;
        bAperiodic      = false;
        disable();
    }
    return this;
}

/**
 * @inheritDoc
 */
IOscillator* IOscillator::setFrequency(float32 fNewFrequency) noexcept {
    fCurrentFrequency = fFrequency = clampFrequency(fNewFrequency);
    fScaleVal         = fTimeStep * fFrequency;
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr IOscillator::emit(size_t uIndex)  noexcept {
    if (!bEnabled) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return oOutputPacketPtr;
    }
    return emitNew();
}

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////


#include <synth/signal/oscillator/LFO.hpp>
namespace MC64K::Synth::Audio::Signal::Oscillator {

LFO::LFO(
    IWaveform::Ptr const& roWaveformPtr,
    float32 fFrequency,
    float32 fDepth,
    bool    bRetrigger
) :
    IOscillator(roWaveformPtr, fFrequency, 0.0f),
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
float32 LFO::clampFrequency(float32 fNewFrequency) const noexcept {
    return fNewFrequency < MIN_FREQUENCY ? MIN_FREQUENCY : (
        fNewFrequency > MAX_FREQUENCY ?
        MAX_FREQUENCY : fNewFrequency
    );
}

Packet const* LFO::generateCommon() noexcept {
    float32* pSamples    = oWaveInputPacketPtr->afSamples;
    float32  fCorrection = fPhaseCorrection;
    uint32   uCounter    = getCyclicSampleCounter();
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        pSamples[u] = (float32)(fScaleVal * uCounter++) + fCorrection;
    }
    uSamplePosition += PACKET_SIZE;
    handleCyclicSampleCounterReset(pSamples[PACKET_SIZE - 1]);
    return oWaveInputPacketPtr.get();
}

/**
 * @inheritDoc
 */
Packet::ConstPtr LFO::emitNew() noexcept {
    Packet* poOutput = oOutputPacketPtr.get();
    poWaveform->map(generateCommon(), poOutput);
    poOutput->scaleBy(fDepth);
    return oOutputPacketPtr;
}


/**
 * @inheritDoc
 */
Packet::ConstPtr LFOZeroToOne::emitNew() noexcept {
    Packet* poOutput = oOutputPacketPtr.get();
    poWaveform->map(generateCommon(), poOutput);
    poOutput->scaleAndBiasBy(Waveform::HALF * fDepth, Waveform::HALF * fDepth);
    return oOutputPacketPtr;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr LFOOneToZero::emitNew() noexcept {
    Packet* poOutput = oOutputPacketPtr.get();
    poWaveform->map(generateCommon(), poOutput);
    poOutput->scaleAndBiasBy(
        Waveform::HALF * fDepth,
        Waveform::ONE - Waveform::HALF * fDepth
    );
    return oOutputPacketPtr;
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/oscillator/sound.hpp>
namespace MC64K::Synth::Audio::Signal::Oscillator {

Sound::Sound(
    IWaveform::Ptr const& roWaveformPtr,
    float32 fFrequency,
    float32 fPhase
) :
    IOscillator(roWaveformPtr, fFrequency, fPhase),
    poPitchModulator{nullptr},
    poPhaseModulator{nullptr},
    poLevelModulator{nullptr},
    poPitchEnvelope{nullptr},
    poLevelEnvelope{nullptr}
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
 *
 * Make sure the reset signal is propagated to the inputs
 */
Sound* Sound::reset() noexcept {
    IOscillator::reset();

    if (poPitchModulator) {
        poPitchModulator->reset();
    }
    if (poPhaseModulator) {
        poPhaseModulator->reset();
    }
    if (poLevelModulator) {
        poLevelModulator->reset();
    }
    if (poLevelEnvelope) {
        poLevelEnvelope->reset();
    }
    if (poPitchEnvelope) {
        poPitchEnvelope->reset();
    }
    return this;
}

/**
 * @inheritDoc
 *
 * Make sure the enable signal is propagated to the inputs
 */
Sound* Sound::enable() noexcept {
    IOscillator::enable();
    if (isEnabled()) {
        // should this be enable()?
        if (poPitchModulator) {
            poPitchModulator->reset();
        }
        if (poPhaseModulator) {
            poPhaseModulator->reset();
        }
        if (poLevelModulator) {
            poLevelModulator->reset();
        }
        if (poLevelEnvelope) {
            poLevelEnvelope->reset();
        }
        if (poPitchEnvelope) {
            poPitchEnvelope->reset();
        }
    }
    return this;
}

/**
 * @inheritDoc
 */
float32 Sound::clampFrequency(float32 fNewFrequency) const noexcept {
    return fNewFrequency < MIN_FREQUENCY ? MIN_FREQUENCY : (
        fNewFrequency > MAX_FREQUENCY ?
        MAX_FREQUENCY : fNewFrequency
    );
}

/**
 * @inheritDoc
 */
void Sound::populatePitchShiftedPacket(Packet const* poPitchShifts) noexcept {
    // Pitch Samples are in relative semitones, e.g. 2.0 is a whole note increase.
    float32 const* pPitchSamples     = poPitchShifts->afSamples;
    float32*       pInputSamples     = oWaveInputPacketPtr->afSamples;

    // Use higher definition calculation for time
    float64        fInstantFrequency = fCurrentFrequency;
    uint32         uCounter          = getCyclicSampleCounter();
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        // Calculate frequency of next sample after applying the semitone delta.
        float64 fNextFrequency = fFrequency * Note::semisToMultiplierApprox(pPitchSamples[u]);

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
    Packet const* poPitchShifts,
    Packet const* poPhaseShifts
) noexcept {
    // Pitch Samples are in relative semitones, e.g. 2.0 is a whole note increase.
    float32 const* pPitchSamples     = poPitchShifts->afSamples;
    float32 const* pPhaseSamples     = poPhaseShifts->afSamples;
    float32*       pInputSamples     = oWaveInputPacketPtr->afSamples;

    // Use higher definition calculation for time
    float64        fInstantFrequency = fCurrentFrequency;
    float32        fPeriod           = fPhaseModulationIndex * fWaveformPeriod;
    uint32         uCounter          = getCyclicSampleCounter();
    float32        fCorrection       = fPhaseCorrection;
    float32        fBasePhase        = 0.0f;
    float32        fPhaseShift       = 0.0f;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        // Calculate frequency of next sample after applying the semitone delta.
        float64 fNextFrequency = fFrequency * Note::semisToMultiplierApprox(pPitchSamples[u]);

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
void Sound::populateOutputPacketWithFeedback(Packet const* poLevelPacket) noexcept {
    float32        fIndex        = fPhaseFeedbackIndex * FEEDBACK_SCALE;
    float32*       pSamples      = oOutputPacketPtr->afSamples;
    float32 const* pLevelSamples = poLevelPacket->afSamples;
    IWaveform*     pWave         = poWaveform;

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
void Sound::inputAperiodic(Sound* poOscillator) noexcept {
    poOscillator->uSamplePosition += PACKET_SIZE;
}


/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is no pitch/phase modulation
 */
void Sound::inputDirect(Sound* poOscillator) noexcept {
    float32* pSamples    = poOscillator->oWaveInputPacketPtr->afSamples;
    float32  fCorrection = poOscillator->fPhaseCorrection;
    uint32   uCounter    = poOscillator->getCyclicSampleCounter();
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        pSamples[u] = (float32)(poOscillator->fScaleVal * uCounter++) + fCorrection;
    }
    poOscillator->uSamplePosition += PACKET_SIZE;
    poOscillator->handleCyclicSampleCounterReset(pSamples[PACKET_SIZE - 1]);
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is only pitch modulation
 */
void Sound::inputPitchMod(Sound* poOscillator) noexcept {
    poOscillator->populatePitchShiftedPacket(
        poOscillator
            ->poPitchModulator
            ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is only pitch envelope
 */
void Sound::inputPitchEnv(Sound* poOscillator) noexcept {
    poOscillator->populatePitchShiftedPacket(
        poOscillator
            ->poPitchEnvelope
            ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is both pitch modulation and envelope.
 *
 * We avoided clone() here since we can populate the packet with a sum directly.
 */
void Sound::inputPitchModEnv(Sound* poOscillator) noexcept {
    Packet::Ptr oPitchShiftsPtr = Packet::create();
    oPitchShiftsPtr->sum(
        poOscillator
            ->poPitchModulator
            ->emit(poOscillator->uLastIndex).get(),
        poOscillator
            ->poPitchEnvelope
            ->emit(poOscillator->uLastIndex).get()
    );
    poOscillator->populatePitchShiftedPacket(oPitchShiftsPtr.get());
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is only phase modulation
 */
void Sound::inputPhaseMod(Sound* poOscillator) noexcept {
    float32 const* poPhaseShifts = poOscillator
        ->poPhaseModulator
        ->emit(poOscillator->uLastIndex)
        ->afSamples;

    float32* pSamples    = poOscillator->oWaveInputPacketPtr->afSamples;
    float32  fPeriod     = poOscillator->fPhaseModulationIndex * poOscillator->fWaveformPeriod;
    float32  fCorrection = poOscillator->fPhaseCorrection;
    uint32   uCounter    = poOscillator->getCyclicSampleCounter();
    float32  fBasePhase;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        fBasePhase = (float32)(poOscillator->fScaleVal * uCounter++) + fCorrection;
        pSamples[u] = fBasePhase + fPeriod * poPhaseShifts[u];
    }
    poOscillator->uSamplePosition += PACKET_SIZE;
    poOscillator->handleCyclicSampleCounterReset(fBasePhase);
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is phase and pitch modulation
 */
void Sound::inputPhaseModPitchMod(Sound* poOscillator) noexcept {
    poOscillator->populatePitchAndPhaseShiftedPacket(
        poOscillator
            ->poPitchModulator
            ->emit(poOscillator->uLastIndex).get(),
        poOscillator
            ->poPhaseModulator
            ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is phase modulation and pitch envelope
 */
void Sound::inputPhaseModPitchEnv(Sound* poOscillator) noexcept {
    poOscillator->populatePitchAndPhaseShiftedPacket(
        poOscillator
            ->poPitchEnvelope
            ->emit(poOscillator->uLastIndex).get(),
        poOscillator
            ->poPhaseModulator
            ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is phase modulation and both pitch
 * modulation and envelope.
 *
 * We avoided clone() here since we can populate the packet with a sum directly.
 */
void Sound::inputPhaseModPitchModEnv(Sound* poOscillator) noexcept {
    Packet::Ptr oPitchShiftsPtr = Packet::create();
    oPitchShiftsPtr->sum(
        poOscillator
            ->poPitchModulator
            ->emit(poOscillator->uLastIndex).get(),
        poOscillator
            ->poPitchEnvelope
            ->emit(poOscillator->uLastIndex).get()
    );

    poOscillator->populatePitchAndPhaseShiftedPacket(
        oPitchShiftsPtr.get(),
        poOscillator
            ->poPhaseModulator
            ->emit(poOscillator->uLastIndex).get()
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
void Sound::configureInputStage() noexcept {
    if (poWaveform && poWaveform->isAperiodic()) {
        cInput = &inputAperiodic;
        std::fprintf(
            stderr,
            "Oscillator::Sound::configureInputStage(): Waveform %d is aperiodic\n",
            poWaveform->getShape()
        );

    } else {
        unsigned uUseStage =
            (poPitchModulator ? 1 : 0) |
            (poPitchEnvelope  ? 2 : 0) |
            ((poPhaseModulator && MIN_PHASE_MOD_INDEX < fPhaseModulationIndex) ? 4 : 0);
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
void Sound::outputDirect(Sound* poOscillator) noexcept {
    poOscillator->poWaveform->map(
        poOscillator->oWaveInputPacketPtr.get(),
        poOscillator->oOutputPacketPtr.get()
    );
}

/**
 * @inheritDoc
 *
 */
void Sound::outputLevelMod(Sound* poOscillator) noexcept {
    poOscillator->poWaveform->map(
        poOscillator->oWaveInputPacketPtr.get(),
        poOscillator->oOutputPacketPtr.get()
    );

    Packet::Ptr oModulationPacketPtr = Packet::create();
    oModulationPacketPtr->scaleBy(
        poOscillator
            ->poLevelModulator
            ->emit(poOscillator->uLastIndex).get(),
        poOscillator->fLevelModulationIndex
    );
    poOscillator
        ->oOutputPacketPtr
        ->modulateWith(oModulationPacketPtr.get());
}

/**
 * @inheritDoc
 */
void Sound::outputLevelEnv(Sound* poOscillator) noexcept {
    poOscillator->poWaveform->map(
        poOscillator->oWaveInputPacketPtr.get(),
        poOscillator->oOutputPacketPtr.get()
    );
    poOscillator->oOutputPacketPtr->modulateWith(
        poOscillator
            ->poLevelEnvelope
            ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 */
void Sound::outputLevelModEnv(Sound* poOscillator) noexcept {
    poOscillator->poWaveform->map(
        poOscillator->oWaveInputPacketPtr.get(),
        poOscillator->oOutputPacketPtr.get()
    );

    Packet::Ptr oModulationPacketPtr = Packet::create();
    oModulationPacketPtr
        ->scaleBy(
            poOscillator
                ->poLevelModulator
                ->emit(poOscillator->uLastIndex).get(),
            poOscillator->fLevelModulationIndex
        )
        ->modulateWith(
            poOscillator
                ->poLevelEnvelope
                ->emit(poOscillator->uLastIndex).get()
          );
    poOscillator
        ->oOutputPacketPtr
        ->modulateWith(oModulationPacketPtr.get());
}

/**
 * @inheritDoc
 */
void Sound::outputFeedback(Sound* poOscillator) noexcept {
    float32    fIndex   = poOscillator->fPhaseFeedbackIndex * FEEDBACK_SCALE;
    float32*   pSamples = poOscillator->oOutputPacketPtr->afSamples;
    IWaveform* pWave    = poOscillator->poWaveform;

    float32 fFeedback1 = poOscillator->fFeedback1;
    float32 fFeedback2 = poOscillator->fFeedback2;

    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        float32 fOutput = pWave->value(
            pSamples[u] +
            fIndex * (fFeedback1 + fFeedback2)
        );
        pSamples[u] = fOutput;
        fFeedback2  = fFeedback1;
        fFeedback1  = fOutput;
    }

    poOscillator->fFeedback1 = fFeedback1;
    poOscillator->fFeedback2 = fFeedback2;
}

/**
 * @inheritDoc
 */
void Sound::outputFeedbackLevelMod(Sound* poOscillator) noexcept {
    Packet::Ptr oModulationPacketPtr = Packet::create();
    oModulationPacketPtr->scaleBy(
        poOscillator
            ->poLevelModulator
            ->emit(poOscillator->uLastIndex).get(),
        poOscillator->fLevelModulationIndex
    );
    poOscillator->populateOutputPacketWithFeedback(oModulationPacketPtr.get());
}

/**
 * @inheritDoc
 */
void Sound::outputFeedbackLevelEnv(Sound* poOscillator) noexcept {
    poOscillator->populateOutputPacketWithFeedback(poOscillator
        ->poLevelEnvelope
        ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 */
void Sound::outputFeedbackLevelModEnv(Sound* poOscillator) noexcept {
    Packet::Ptr oModulationPacketPtr = Packet::create();
    oModulationPacketPtr
        ->scaleBy(
            poOscillator
                ->poLevelModulator
                ->emit(poOscillator->uLastIndex).get(),
            poOscillator->fLevelModulationIndex
        )
        ->modulateWith(
            poOscillator
                ->poLevelEnvelope
                ->emit(poOscillator->uLastIndex).get()
        );
    poOscillator->populateOutputPacketWithFeedback(oModulationPacketPtr.get());
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
void Sound::configureOutputStage() noexcept {
    unsigned uUseStage =
        ((poLevelModulator && MIN_LEVEL_MOD_INDEX < fLevelModulationIndex) ? 1 : 0) |
        (poLevelEnvelope ? 2 : 0) |
        ((!bAperiodic && MIN_FEEDBACK_MOD_INDEX < fPhaseFeedbackIndex) ? 4 : 0);
    cOutput = aOutputStages[uUseStage];
    std::fprintf(
        stderr,
        "Oscillator::Sound::configureOutputStage(): %u [%s]\n",
        uUseStage,
        aOutputStageNames[uUseStage]
    );
}

void Sound::configureAntialias() noexcept {
    switch (eAAMode) {
        case AA_OFF:
            bAntialias = false;
            break;
        case AA_ON:
            bAntialias = true;
            break;
        case AA_AUTO:
            bAntialias = (poWaveform && poWaveform->isDiscontinuous());
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
Packet::ConstPtr Sound::emitNew() noexcept {

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
        Packet::Ptr pNextOutputPacket = Packet::create();

        float32* aUnfiltered = oOutputPacketPtr->afSamples;
        float32* aFiltered   = pNextOutputPacket->afSamples;

        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
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
        oOutputPacketPtr = pNextOutputPacket;
    }
    return oOutputPacketPtr;
}


} // namespace
