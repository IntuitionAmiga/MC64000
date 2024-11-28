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
IEnvelope* IEnvelope::setTimeScale(float32 fNewTimeScale) noexcept {
    if (fNewTimeScale < MIN_TIME_SCALE) {
        fNewTimeScale = MIN_TIME_SCALE;
    }
    if (std::fabs(fNewTimeScale - fTimeScale) > MIN_TIME_SCALE_DIFF) {
        fTimeScale        = fNewTimeScale;
        bParameterChanged = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
IEnvelope* IEnvelope::setLevelScale(float32 fNewLevelScale) noexcept {
    if (std::fabs(fNewLevelScale - fLevelScale) > MIN_LEVEL_SCALE_DIFF) {
        fLevelScale       = fNewLevelScale;
        bParameterChanged = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
IEnvelope* IEnvelope::reset() noexcept {
    uSamplePosition   = 0;
    bParameterChanged = true;
    return this;
}

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/envelope/decaypulse.hpp>
namespace MC64K::Synth::Audio::Signal::Envelope {

/**
 * @inheritDoc
 */
DecayPulse::DecayPulse(float32 fInitial, float32 fHalflife, float fTarget):
    IEnvelope{},
    fInitial{fInitial},
    fHalflife{fHalflife},
    fTarget{fTarget}
{
    reset();
    enable();
    std::fprintf(stderr, "Created DecayPulse at %p\n", this);
}

/**
 * @inheritDoc
 */
DecayPulse::~DecayPulse() {
    std::fprintf(stderr, "Destroyed DecayPulse at %p\n", this);
}

/**
 * @inheritDoc
 */
void DecayPulse::recalculateDecay() noexcept {
    fCurrent = (fInitial * fLevelScale) - fTarget;
    float64 fHalfLifeInSamples = (PROCESS_RATE * fHalflife * fTimeScale);
    fDecayPerSample   = 0.5 * std::exp2((fHalfLifeInSamples - 1.0) / fHalfLifeInSamples);
    bParameterChanged = false;
}


/**
 * @inheritDoc
 */
Packet::ConstPtr DecayPulse::emit(size_t uIndex) noexcept {
    if (!bEnabled) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return oOutputPacketPtr;
    }
    if (bParameterChanged) {
        recalculateDecay();
    }
    float32* afSamples = oOutputPacketPtr->afSamples;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        fCurrent *= fDecayPerSample;
        afSamples[u] = (float32)fCurrent + fTarget;
    }
    uSamplePosition += PACKET_SIZE;
    return oOutputPacketPtr;
}

/**
 * @inheritDoc
 */
DecayPulse* DecayPulse::setInitial(float32 fNewInitial) noexcept {
    if (std::fabs(fNewInitial - fInitial) > PEAK_SILENCE_LIMIT) {
        fInitial = fNewInitial;
        bParameterChanged = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
DecayPulse* DecayPulse::setTarget(float32 fNewTarget) noexcept {
    if (std::fabs(fNewTarget - fTarget) > PEAK_SILENCE_LIMIT) {
        fTarget = fNewTarget;
        bParameterChanged = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
DecayPulse* DecayPulse::setHalflife(float32 fNewHalflife) noexcept {
    if (std::fabs(fNewHalflife - fHalflife) > PEAK_SILENCE_LIMIT) {
        fHalflife = fNewHalflife;
        bParameterChanged = true;
    }
    return this;
}

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/envelope/shape.hpp>
namespace MC64K::Synth::Audio::Signal::Envelope {

/**
 * @inheritDoc
 */
Shape::Shape(float32 fInitial, Point const* aoInputPoints, size_t uNumInputPoints):
    IEnvelope{},
    poPoints(0),
    uNumPoints(0)
{
    std::fprintf(stderr, "Created Shape at %p with %lu vertices\n", this, uNumInputPoints);
    processPointList(fInitial, aoInputPoints, uNumInputPoints);
    enable();
}

/**
 * @inheritDoc
 */
Shape::~Shape() {
    std::fprintf(stderr, "Destroyed Shape at %p\n", this);
}

/**
 * @inheritDoc
 */
Shape* Shape::reset() noexcept {
    IEnvelope::reset();
    pNextProcessPoint = aoProcessPoints.get();
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr Shape::emit(size_t uIndex) noexcept {
    if (!bEnabled) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return oOutputPacketPtr;
    }
    if (bParameterChanged) {
        recalculate();
    }
    if (uSamplePosition > uFinalSamplePosition) {
        uSamplePosition += PACKET_SIZE;
        return oFinalPacketPtr;
    }

    // Does the next packet contain a vertex?
    if (uSamplePosition + PACKET_SIZE > pNextProcessPoint->uSamplePosition) {
        // version that checks for interpolant change. There could even be more than one...
        float32* afSamples = oOutputPacketPtr->afSamples;
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            if (pNextProcessPoint->uSamplePosition == uSamplePosition) {
                updateInterpolants();
                ++pNextProcessPoint; // we should probably range check this too...
            }
            afSamples[u] = (float32)(fYOffset + (float64)(++uSamplePosition - uXOffset) * fGradient);
        }
    } else {
        // version that doesn't check for interpolant change
        float32* afSamples = oOutputPacketPtr->afSamples;
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] = (float32)(fYOffset + (float64)(++uSamplePosition - uXOffset) * fGradient);
        }
    }

    return oOutputPacketPtr;
}

/**
 * @inheritDoc
 */
void Shape::processPointList(float32 fInitial, Point const* aoInputPoints, size_t uNumInputPoints) noexcept {
    uNumPoints = uNumInputPoints + 1;
    poPoints.reset(new Point[uNumPoints]);
    aoProcessPoints.reset(new ProcessPoint[uNumPoints + 1]);
    poPoints[0].fLevel = fInitial;
    poPoints[0].fTime  = 0.0f;
    std::fprintf(stderr, "\t[%.2f, %.2f]\n", poPoints[0].fLevel, poPoints[0].fTime);
    for (size_t u = 0; u < uNumInputPoints; ++u) {
        float32 fTime  = aoInputPoints[u].fTime;
        poPoints[u + 1].fLevel = aoInputPoints[u].fLevel;
        poPoints[u + 1].fTime  = (fTime < MIN_TIME) ? MIN_TIME : ((fTime > MAX_TIME) ? MAX_TIME : fTime);
        std::fprintf(stderr, "\t[%.2f, %.2f]\n", poPoints[u + 1].fLevel, poPoints[u + 1].fTime);
    }
    bParameterChanged = true;
    pNextProcessPoint = aoProcessPoints.get();
}

/**
 * @inheritDoc
 */
void Shape::recalculate() noexcept {
    float64 fTimeTotal = 0.0f;
    for (size_t u = 0; u < uNumPoints; ++u) {
        fTimeTotal += poPoints[u].fTime * fTimeScale;
        aoProcessPoints[u].uSamplePosition = (size_t)(fTimeTotal * PROCESS_RATE);
        aoProcessPoints[u].fLevel = poPoints[u].fLevel * fLevelScale;
    }

    size_t uLastPoint = uNumPoints - 1;

    // Pad on the last ProcessPoint again, with a slight time offset to ensure that the interpolation code
    // is always acting between a pair of ProcessPoints and doesn't wander off the end of the array
    aoProcessPoints[uNumPoints] = aoProcessPoints[uLastPoint];
    aoProcessPoints[uNumPoints].uSamplePosition += 16; // will never be reached.

    // Fill the final packet with the last level value.
    if (!oFinalPacketPtr.get()) {
        oFinalPacketPtr = Packet::create();
    }
    oFinalPacketPtr->fillWith((float32)aoProcessPoints[uLastPoint].fLevel);

    uFinalSamplePosition = aoProcessPoints[uLastPoint].uSamplePosition;

    // Clear the changed flag
    bParameterChanged = false;
}

/**
 * @inheritDoc
 */
void Shape::updateInterpolants() noexcept {
    ProcessPoint const& rA = pNextProcessPoint[0];
    ProcessPoint const& rB = pNextProcessPoint[1];
    fGradient = (rB.fLevel - rA.fLevel) / (float64)(rB.uSamplePosition - rA.uSamplePosition);
    fYOffset  = rA.fLevel;
    uXOffset  = rA.uSamplePosition;
}

} // namespace
