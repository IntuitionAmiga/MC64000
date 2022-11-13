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

/**
 * @inheritDoc
 */
IEnvelope* IEnvelope::reset() {
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
void DecayPulse::recalculateDecay() {
    fCurrent = (fInitial * fLevelScale) - fTarget;
    float64 fHalfLifeInSamples = (PROCESS_RATE * fHalflife * fTimeScale);
    fDecayPerSample   = 0.5 * std::exp2((fHalfLifeInSamples - 1.0) / fHalfLifeInSamples);
    bParameterChanged = false;
}


/**
 * @inheritDoc
 */
Packet::ConstPtr DecayPulse::emit(size_t uIndex) {
    if (!bEnabled) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return poLastPacket;
    }
    if (bParameterChanged) {
        recalculateDecay();
    }
    if (!poLastPacket.get()) {
        poLastPacket = Packet::create();
    }
    float32* afSamples = poLastPacket->afSamples;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        fCurrent *= fDecayPerSample;
        afSamples[u] = (float32)fCurrent + fTarget;
    }
    uSamplePosition += PACKET_SIZE;
    return poLastPacket;
}

/**
 * @inheritDoc
 */
DecayPulse* DecayPulse::setInitial(float32 fNewInitial) {
    if (std::fabs(fNewInitial - fInitial) > 1e-5) {
        fInitial = fNewInitial;
        bParameterChanged = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
DecayPulse* DecayPulse::setTarget(float32 fNewTarget) {
    if (std::fabs(fNewTarget - fTarget) > 1e-5) {
        fTarget = fNewTarget;
        bParameterChanged = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
DecayPulse* DecayPulse::setHalflife(float32 fNewHalflife) {
    if (std::fabs(fNewHalflife - fHalflife) > 1e-5) {
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
Shape::Shape(float32 fInitial, Point const* pInputPoints, size_t uNumInputPoints):
    pPoints(0),
    uNumPoints(0)
{
    std::fprintf(stderr, "Created Shape at %p with %lu vertices\n", this, uNumInputPoints);
    processPointList(fInitial, pInputPoints, uNumInputPoints);
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
Shape* Shape::reset() {
    IEnvelope::reset();
    pNextProcessPoint = pProcessPoints.get();
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr Shape::emit(size_t uIndex) {
    if (!bEnabled) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return pOutputPacket;
    }
    if (bParameterChanged) {
        recalculate();
    }
    if (uSamplePosition > uFinalSamplePosition) {
        uSamplePosition += PACKET_SIZE;
        return pFinalPacket;
    }

    if (!pOutputPacket.get()) {
        pOutputPacket = Packet::create();
    }

    // Does the next packet contain a vertex?
    if (uSamplePosition + PACKET_SIZE > pNextProcessPoint->uSamplePosition) {
        // version that checks for interpolant change. There could even be more than one...
        float32* afSamples = pOutputPacket->afSamples;
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            if (pNextProcessPoint->uSamplePosition == uSamplePosition) {
                updateInterpolants();
                ++pNextProcessPoint; // we should probably range check this too...
            }
            afSamples[u] = (float32)(fYOffset + (float64)(++uSamplePosition - uXOffset) * fGradient);
        }
    } else {
        // version that doesn't check for interpolant change
        float32* afSamples = pOutputPacket->afSamples;
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] = (float32)(fYOffset + (float64)(++uSamplePosition - uXOffset) * fGradient);
        }
    }

    return pOutputPacket;
}

/**
 * @inheritDoc
 */
void Shape::processPointList(float32 fInitial, Point const* pInputPoints, size_t uNumInputPoints) {
    uNumPoints = uNumInputPoints + 1;
    pPoints.reset(new Point[uNumPoints]);
    pProcessPoints.reset(new ProcessPoint[uNumPoints + 1]);
    pPoints[0].fLevel = fInitial;
    pPoints[0].fTime  = 0.0f;
    std::printf("\t[%.2f, %.2f]\n", pPoints[0].fLevel, pPoints[0].fTime);
    for (size_t u = 0; u < uNumInputPoints; ++u) {
        float32 fTime  = pInputPoints[u].fTime;
        pPoints[u + 1].fLevel = pInputPoints[u].fLevel;
        pPoints[u + 1].fTime  = (fTime < MIN_TIME) ? MIN_TIME : ((fTime > MAX_TIME) ? MAX_TIME : fTime);
        std::printf("\t[%.2f, %.2f]\n", pPoints[u + 1].fLevel, pPoints[u + 1].fTime);
    }
    bParameterChanged = true;
    pNextProcessPoint = pProcessPoints.get();
}

/**
 * @inheritDoc
 */
void Shape::recalculate() {
    float64 fTimeTotal = 0.0f;
    for (size_t u = 0; u < uNumPoints; ++u) {
        fTimeTotal += pPoints[u].fTime * fTimeScale;
        pProcessPoints[u].uSamplePosition = (size_t)(fTimeTotal * PROCESS_RATE);
        pProcessPoints[u].fLevel = pPoints[u].fLevel * fLevelScale;
    }

    size_t uLastPoint = uNumPoints - 1;

    // Pad on the last ProcessPoint again, with a slight time offset to ensure that the interpolation code
    // is always acting between a pair of ProcessPoints and doesn't wander off the end of the array
    pProcessPoints[uNumPoints] = pProcessPoints[uLastPoint];
    pProcessPoints[uNumPoints].uSamplePosition += 16; // will never be reached.

    // Fill the final packet with the last level value.
    if (!pFinalPacket.get()) {
        pFinalPacket = Packet::create();
    }
    pFinalPacket->fillWith((float32)pProcessPoints[uLastPoint].fLevel);

    uFinalSamplePosition = pProcessPoints[uLastPoint].uSamplePosition;

    // Clear the changed flag
    bParameterChanged = false;
}

/**
 * @inheritDoc
 */
void Shape::updateInterpolants() {
    ProcessPoint const& rA = pNextProcessPoint[0];
    ProcessPoint const& rB = pNextProcessPoint[1];
    fGradient = (rB.fLevel - rA.fLevel) / (float64)(rB.uSamplePosition - rA.uSamplePosition);
    fYOffset  = rA.fLevel;
    uXOffset  = rA.uSamplePosition;
}

} // namespace
