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
#include <synth/signal/oscillator.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;

/**
 * @inheritDoc
 */
IOscillator::IOscillator(
    IWaveform::Ptr pWaveform,
    float32 fInitialFrequency,
    float32 fInitialPhase
):
    fPhaseOffset(fInitialPhase),
    fPhaseCorrection(fInitialPhase),
    fWaveformPeriod(1.0f),
    fTimeStep(SAMPLE_PERIOD)

{
    pLastOutput = Packet::create();
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
IOscillator* IOscillator::setWaveform(IWaveform::Ptr pNewWaveform) {
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
Packet::Ptr IOscillator::emit(size_t uIndex) {
    if (!bEnabled || !pWaveform.get()) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return pLastOutput;
    }
    return emitNew();
}

} // namespace

#include <synth/signal/oscillator/LFO.hpp>
namespace MC64K::Synth::Audio::Signal::Oscillator {

/**
 * @inheritDoc
 */
LFO::~LFO() {
    std::printf("Destroyed LFO at %p\n", this);
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
/**
 * @inheritDoc
 */
Packet::Ptr LFO::emitNew() {
    Packet::Ptr pInput = Packet::create();
    float32* pSamples  = pInput->aSamples;
    for (unsigned u=0; u < PACKET_SIZE; ++u) {
        pSamples[u] = fScaleVal * (float32)(uSamplePosition++);
    }
    pLastOutput = pWaveform->map(pInput);
    pLastOutput->scaleBy(fDepth);
    return pLastOutput;
}

}
