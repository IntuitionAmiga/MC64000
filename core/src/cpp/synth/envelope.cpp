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
#include <synth/signal.hpp>
#include <synth/signal/envelope.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;

/**
 * @inheritDoc
 */
IEnvelope* IEnvelope::setTimeScale(float32 fNewTimeScale) {
    if (fNewTimeScale < MIN_TIME_SCALE) {
        fNewTimeScale = MIN_TIME_SCALE;
    }
    if (fabs(fNewTimeScale - fTimeScale) > MIN_TIME_SCALE_DIFF) {
        fTimeScale        = fNewTimeScale;
        bParameterChanged = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
IEnvelope* IEnvelope::setLevelScale(float32 fNewLevelScale) {
    if (fabs(fNewLevelScale - fLevelScale) > MIN_LEVEL_SCALE_DIFF) {
        fLevelScale       = fNewLevelScale;
        bParameterChanged = true;
    }
    return this;
}

IEnvelope* IEnvelope::reset() {
    uSamplePosition   = 0;
    bParameterChanged = true;
    return this;
}

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/envelope/decaypulse.hpp>
namespace MC64K::Synth::Audio::Signal::Envelope {

DecayPulse::DecayPulse(float32 fInitial, float32 fHalflife, float fTarget):
    fInitial{fInitial},
    fHalflife{fHalflife},
    fTarget{fTarget}
{
    reset();
    enable();
    std::fprintf(stderr, "Created DecayPulse at %p\n", this);
}

DecayPulse::~DecayPulse() {
    std::fprintf(stderr, "Destroyed DecayPulse at %p\n", this);
}

void DecayPulse::recalculateDecay() {
    fCurrent = (fInitial * fLevelScale) - fTarget;
    float64 fHalfLifeInSamples = (PROCESS_RATE * fHalflife * fTimeScale);
    fDecayPerSample   = 0.5 * std::exp2((fHalfLifeInSamples - 1.0) / fHalfLifeInSamples);
    bParameterChanged = false;
}


Packet::ConstPtr DecayPulse::emit(size_t uIndex) {

    if (!bEnabled) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return pLastPacket;
    }
    if (bParameterChanged) {
        recalculateDecay();
    }
    if (!pLastPacket.get()) {
        pLastPacket = Packet::create();
    }
    float32* aSamples = pLastPacket->aSamples;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        fCurrent *= fDecayPerSample;
        aSamples[u] = (float32)fCurrent + fTarget;
    }
    uSamplePosition += PACKET_SIZE;
    return pLastPacket;
}

DecayPulse* DecayPulse::setInitial(float32 fNewInitial) {
    if (std::fabs(fNewInitial - fInitial) > 1e-5) {
        fInitial = fNewInitial;
        bParameterChanged = true;
    }
    return this;
}

DecayPulse* DecayPulse::setTarget(float32 fNewTarget) {
    if (std::fabs(fNewTarget - fTarget) > 1e-5) {
        fTarget = fNewTarget;
        bParameterChanged = true;
    }
    return this;
}

DecayPulse* DecayPulse::setHalflife(float32 fNewHalflife) {
    if (std::fabs(fNewHalflife - fHalflife) > 1e-5) {
        fHalflife = fNewHalflife;
        bParameterChanged = true;
    }
    return this;
}

} // namespace
