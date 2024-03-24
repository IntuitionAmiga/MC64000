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
bool IOscillator::canEnable() const {
    return oWaveformPtr.get() != 0;
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
IOscillator* IOscillator::setWaveform(IWaveform::Ptr const& roNewWaveformPtr) {
    if (roNewWaveformPtr.get()) {
        oWaveformPtr    = roNewWaveformPtr->copy();
        fWaveformPeriod = oWaveformPtr->getPeriod();
        fTimeStep       = fWaveformPeriod * SAMPLE_PERIOD;
        fScaleVal       = fTimeStep * fFrequency;
        bAperiodic      = false;
    } else {
        oWaveformPtr    = 0;
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
IOscillator* IOscillator::setFrequency(float32 fNewFrequency) {
    fCurrentFrequency = fFrequency = clampFrequency(fNewFrequency);
    fScaleVal         = fTimeStep * fFrequency;
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr IOscillator::emit(size_t uIndex) {
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
float32 LFO::clampFrequency(float32 fNewFrequency) const {
    return fNewFrequency < MIN_FREQUENCY ? MIN_FREQUENCY : (
        fNewFrequency > MAX_FREQUENCY ?
        MAX_FREQUENCY : fNewFrequency
    );
}

Packet const* LFO::generateCommon() {
    float32* pSamples    = oOutputPacketPtr->afSamples;
    float32  fCorrection = fPhaseCorrection;
    uint32   uCounter    = getCyclicSampleCounter();
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        pSamples[u] = (float32)(fScaleVal * uCounter++) + fCorrection;
    }
    uSamplePosition += PACKET_SIZE;
    handleCyclicSampleCounterReset(pSamples[PACKET_SIZE - 1]);
    return oOutputPacketPtr.get();
}

/**
 * @inheritDoc
 */
Packet::ConstPtr LFO::emitNew() {
    oOutputPacketPtr = oWaveformPtr->map(generateCommon());
    oOutputPacketPtr->scaleBy(fDepth);
    return oOutputPacketPtr;
}


/**
 * @inheritDoc
 */
Packet::ConstPtr LFOZeroToOne::emitNew() {
    oOutputPacketPtr = oWaveformPtr->map(generateCommon());
    oOutputPacketPtr->scaleAndBiasBy(Waveform::HALF * fDepth, Waveform::HALF * fDepth);
    return oOutputPacketPtr;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr LFOOneToZero::emitNew() {
    oOutputPacketPtr = oWaveformPtr->map(generateCommon());
    oOutputPacketPtr->scaleAndBiasBy(
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
    IOscillator(roWaveformPtr, fFrequency, fPhase)
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
Sound* Sound::reset() {
    IOscillator::reset();
    IStream* poInput;
    if ((poInput = oPitchModulatorPtr.get())) {
        poInput->reset();
    }
    if ((poInput = oPhaseModulatorPtr.get())) {
        poInput->reset();
    }
    if ((poInput = oLevelModulatorPtr.get())) {
        poInput->reset();
    }
    if ((poInput = oLevelEnvelopePtr.get())) {
        poInput->reset();
    }
    if ((poInput = oPitchEnvelopePtr.get())) {
        poInput->reset();
    }
    return this;
}

/**
 * @inheritDoc
 *
 * Make sure the enable signal is propagated to the inputs
 */
Sound* Sound::enable() {
    IOscillator::enable();
    if (isEnabled()) {
        IStream* poInput;

        if ((poInput = oPitchModulatorPtr.get())) {
            poInput->reset();
        }
        if ((poInput = oPhaseModulatorPtr.get())) {
            poInput->reset();
        }
        if ((poInput = oLevelModulatorPtr.get())) {
            poInput->reset();
        }
        if ((poInput = oLevelEnvelopePtr.get())) {
            poInput->reset();
        }
        if ((poInput = oPitchEnvelopePtr.get())) {
            poInput->reset();
        }
    }
    return this;
}

/**
 * @inheritDoc
 */
float32 Sound::clampFrequency(float32 fNewFrequency) const {
    return fNewFrequency < MIN_FREQUENCY ? MIN_FREQUENCY : (
        fNewFrequency > MAX_FREQUENCY ?
        MAX_FREQUENCY : fNewFrequency
    );
}

/**
 * @inheritDoc
 */
void Sound::populatePitchShiftedPacket(Packet const* poPitchShifts) {
    // Pitch Samples are in relative semitones, e.g. 2.0 is a whole note increase.
    float32 const* pPitchSamples     = poPitchShifts->afSamples;
    float32*       pInputSamples     = oOutputPacketPtr->afSamples;

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
) {
    // Pitch Samples are in relative semitones, e.g. 2.0 is a whole note increase.
    float32 const* pPitchSamples     = poPitchShifts->afSamples;
    float32 const* pPhaseSamples     = poPhaseShifts->afSamples;
    float32*       pInputSamples     = oOutputPacketPtr->afSamples;

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
void Sound::populateOutputPacketWithFeedback(Packet const* poLevelPacket) {
    float32        fIndex        = fPhaseFeedbackIndex * FEEDBACK_SCALE;
    float32*       pSamples      = oOutputPacketPtr->afSamples;
    float32 const* pLevelSamples = poLevelPacket->afSamples;
    IWaveform*     pWave         = oWaveformPtr.get();

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
void Sound::inputAperiodic(Sound* poOscillator) {
    poOscillator->uSamplePosition += PACKET_SIZE;
}


/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is no pitch/phase modulation
 */
void Sound::inputDirect(Sound* poOscillator) {
    float32* pSamples    = poOscillator->oOutputPacketPtr->afSamples;
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
void Sound::inputPitchMod(Sound* poOscillator) {
    poOscillator->populatePitchShiftedPacket(
        poOscillator
            ->oPitchModulatorPtr
            ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is only pitch envelope
 */
void Sound::inputPitchEnv(Sound* poOscillator) {
    poOscillator->populatePitchShiftedPacket(
        poOscillator
            ->oPitchEnvelopePtr
            ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is both pitch modulation and envelope
 */
void Sound::inputPitchModEnv(Sound* poOscillator) {
    Packet::Ptr poPitchShifts = poOscillator
        ->oPitchModulatorPtr
        ->emit(poOscillator->uLastIndex)
        ->clone();
    poPitchShifts->sumWith(
        poOscillator
            ->oPitchEnvelopePtr
            ->emit(poOscillator->uLastIndex).get()
    );
    poOscillator->populatePitchShiftedPacket(poPitchShifts.get());
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is only phase modulation
 */
void Sound::inputPhaseMod(Sound* poOscillator) {
    float32 const* poPhaseShifts = poOscillator
        ->oPhaseModulatorPtr
        ->emit(poOscillator->uLastIndex)
        ->afSamples;
    float32* pSamples    = poOscillator->oOutputPacketPtr->afSamples;
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
void Sound::inputPhaseModPitchMod(Sound* poOscillator) {
    poOscillator->populatePitchAndPhaseShiftedPacket(
        poOscillator
            ->oPitchModulatorPtr
            ->emit(poOscillator->uLastIndex).get(),
        poOscillator
            ->oPhaseModulatorPtr
            ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is phase modulation and pitch envelope
 */
void Sound::inputPhaseModPitchEnv(Sound* poOscillator) {
    poOscillator->populatePitchAndPhaseShiftedPacket(
        poOscillator
        ->oPitchEnvelopePtr
        ->emit(poOscillator->uLastIndex).get(),
        poOscillator
        ->oPhaseModulatorPtr
        ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 *
 * Optimised input packet generator for the case where there is phase modulation and both pitch
 * modulation and envelope
 */
void Sound::inputPhaseModPitchModEnv(Sound* poOscillator) {
    Packet::Ptr poPitchShifts = poOscillator
        ->oPitchModulatorPtr
        ->emit(poOscillator->uLastIndex)
        ->clone();
    poPitchShifts->sumWith(
        poOscillator
            ->oPitchEnvelopePtr
            ->emit(poOscillator->uLastIndex).get()
        );
    poOscillator->populatePitchAndPhaseShiftedPacket(
        poPitchShifts.get(),
        poOscillator
        ->oPhaseModulatorPtr
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
void Sound::configureInputStage() {
    if (oWaveformPtr.get() && oWaveformPtr->isAperiodic()) {
        cInput = &inputAperiodic;
        std::fprintf(
            stderr,
            "Oscillator::Sound::configureInputStage(): Waveform %d is aperiodic\n",
            oWaveformPtr->getShape()
        );

    } else {
        unsigned uUseStage =
            (oPitchModulatorPtr.get() ? 1 : 0) |
            (oPitchEnvelopePtr.get()  ? 2 : 0) |
            ((oPhaseModulatorPtr.get() && MIN_PHASE_MOD_INDEX < fPhaseModulationIndex) ? 4 : 0);
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
void Sound::outputDirect(Sound* poOscillator) {
    poOscillator->oOutputPacketPtr = poOscillator->oWaveformPtr->map(
        poOscillator->oOutputPacketPtr.get()
    );
}

/**
 * @inheritDoc
 */
void Sound::outputLevelMod(Sound* poOscillator) {
    poOscillator->oOutputPacketPtr = poOscillator->oWaveformPtr->map(
        poOscillator->oOutputPacketPtr.get()
    );
    Packet::Ptr pOutputLevel = poOscillator
        ->oLevelModulatorPtr
        ->emit(poOscillator->uLastIndex)
        ->clone();
    pOutputLevel->scaleBy(poOscillator->fLevelModulationIndex);
    poOscillator
        ->oOutputPacketPtr
        ->modulateWith(pOutputLevel.get());
}

/**
 * @inheritDoc
 */
void Sound::outputLevelEnv(Sound* poOscillator) {
    poOscillator->oOutputPacketPtr = poOscillator->oWaveformPtr->map(
        poOscillator->oOutputPacketPtr.get()
    );
    poOscillator->oOutputPacketPtr->modulateWith(
        poOscillator
            ->oLevelEnvelopePtr
            ->emit(poOscillator->uLastIndex).get()
    );
}

/**
 * @inheritDoc
 */
void Sound::outputLevelModEnv(Sound* poOscillator) {
    poOscillator->oOutputPacketPtr = poOscillator->oWaveformPtr->map(
        poOscillator->oOutputPacketPtr.get()
    );
    Packet::Ptr pOutputLevel = poOscillator
        ->oLevelModulatorPtr
        ->emit(poOscillator->uLastIndex)
        ->clone();
    pOutputLevel
        ->scaleBy(poOscillator->fLevelModulationIndex)
        ->modulateWith(
            poOscillator
                ->oLevelModulatorPtr
                ->emit(poOscillator->uLastIndex).get()
          );
    poOscillator
        ->oOutputPacketPtr
        ->modulateWith(pOutputLevel.get());
}

/**
 * @inheritDoc
 */
void Sound::outputFeedback(Sound* poOscillator) {
    float32    fIndex   = poOscillator->fPhaseFeedbackIndex * FEEDBACK_SCALE;
    float32*   pSamples = poOscillator->oOutputPacketPtr->afSamples;
    IWaveform* pWave    = poOscillator->oWaveformPtr.get();

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
void Sound::outputFeedbackLevelMod(Sound* poOscillator) {
    Packet::Ptr pOutputLevel = poOscillator
        ->oLevelModulatorPtr
        ->emit(poOscillator->uLastIndex)
        ->clone();
    pOutputLevel->scaleBy(poOscillator->fLevelModulationIndex);
    poOscillator->populateOutputPacketWithFeedback(pOutputLevel.get());
}

/**
 * @inheritDoc
 */
void Sound::outputFeedbackLevelEnv(Sound* poOscillator) {
    Packet::Ptr pOutputLevel = poOscillator
        ->oLevelEnvelopePtr
        ->emit(poOscillator->uLastIndex)
        ->clone();
    poOscillator->populateOutputPacketWithFeedback(pOutputLevel.get());
}

/**
 * @inheritDoc
 */
void Sound::outputFeedbackLevelModEnv(Sound* poOscillator) {
    Packet::Ptr pOutputLevel = poOscillator
        ->oLevelModulatorPtr
        ->emit(poOscillator->uLastIndex)
        ->clone();
    pOutputLevel->scaleBy(poOscillator->fLevelModulationIndex);
    pOutputLevel->modulateWith(
        poOscillator
            ->oLevelEnvelopePtr
            ->emit(poOscillator->uLastIndex).get()
    );
    poOscillator->populateOutputPacketWithFeedback(pOutputLevel.get());
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
        ((oLevelModulatorPtr.get() && MIN_LEVEL_MOD_INDEX < fLevelModulationIndex) ? 1 : 0) |
        (oLevelEnvelopePtr.get() ? 2 : 0) |
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
            bAntialias = (oWaveformPtr.get() && oWaveformPtr->isDiscontinuous());
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
        oOutputPacketPtr = pNextOutputPacket;
    }
    return oOutputPacketPtr;
}


} // namespace
