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
#include <synth/signal/filter.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/filter/4polemulti.hpp>
namespace MC64K::Synth::Audio::Signal::Filter {
using namespace MC64K::StandardTestHost::Audio::IConfig;

/**
 * @inheritDoc
 */
void FourPoleMultiMode::filterSample(float64 fInput, float64 fCutoff, float64 fResonance) {
    float64 fInputSH    = fInput;
    float64 fInvCutoff  = 1.0 - fCutoff;
    float64 fPrevFeedback;
    float64 fFeedbackPhase;
    int iOverSample     = 2;
    while (iOverSample--) {
        fPrevFeedback   = fFeedback > 1.0 ? 1.0 : fFeedback;
        fFeedback       = fFeedback * FEEDBACK + fResonance * fPole4 * (1.0 - FEEDBACK);        // dynamic feedback
        fFeedbackPhase  = fFeedback * FEEDBACK_PHASE + fPrevFeedback * (1.0 - FEEDBACK_PHASE); // feedback phase
        fInput          = fInputSH - fFeedbackPhase;                                            // inverted feedback
        fPole1          = fInput * fCutoff + fPole1 * fInvCutoff;

        // Clip pole1
        fPole1 = fPole1 > 1.0 ? 1.0 : (fPole1 < -1.0 ? 1.0 : fPole1);
        fPole2 = fPole1 * fCutoff + fPole2 * fInvCutoff;
        fPole3 = fPole2 * fCutoff + fPole3 * fInvCutoff;
        fPole4 = fPole3 * fCutoff + fPole4 * fInvCutoff;
    }
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::emit(uint64 uIndex) {
    if (!bEnabled) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return poLastOutputPacket;
    }
    cProcess(this);
    return poLastOutputPacket;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Input controls
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::cutoffMod(FourPoleMultiMode* poFilter) {
    return poFilter->poCutoffModulator->emit(poFilter->uLastIndex);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::cutoffEnv(FourPoleMultiMode* poFilter) {
    return poFilter->poCutoffEnvelope->emit(poFilter->uLastIndex);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::cutoffModEnv(FourPoleMultiMode* poFilter) {
    Packet::Ptr poPacket   = poFilter->poCutoffModulator->emit(poFilter->uLastIndex)->clone();
    poPacket->modulateWith(poFilter->poCutoffEnvelope->emit(poFilter->uLastIndex));
    return poPacket;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::resonanceMod(FourPoleMultiMode* poFilter) {
    return poFilter->poResonanceModulator->emit(poFilter->uLastIndex);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::resonanceEnv(FourPoleMultiMode* poFilter) {
    return poFilter->poResonanceEnvelope->emit(poFilter->uLastIndex);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::resonanceModEnv(FourPoleMultiMode* poFilter) {
    Packet::Ptr poPacket   = poFilter->poResonanceModulator->emit(poFilter->uLastIndex)->clone();
    poPacket->modulateWith(poFilter->poResonanceEnvelope->emit(poFilter->uLastIndex));
    return poPacket;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Low Pass Outputs
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @inheritDoc
 */
void FourPoleMultiMode::lowPassFixedCFixedQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->poLastOutputPacket->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)poFilter->fPole4;
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::lowPassVaryingCFixedQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->poLastOutputPacket->afSamples;
    float32 const* afCutoff   = poFilter->cCutoff(poFilter)->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)poFilter->fPole4;
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::lowPassFixedCFVaryingQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->poLastOutputPacket->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)poFilter->fPole4;
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::lowPassVaryingCVaryingQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->poLastOutputPacket->afSamples;
    float32 const* afCutoff    = poFilter->cCutoff(poFilter)->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)poFilter->fPole4;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  High Pass Outputs
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @inheritDoc
 */
void FourPoleMultiMode::hiPassFixedCFixedQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->poLastOutputPacket->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole4 - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::hiPassVaryingCFixedQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->poLastOutputPacket->afSamples;
    float32 const* afCutoff   = poFilter->cCutoff(poFilter)->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole4 - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::hiPassFixedCFVaryingQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->poLastOutputPacket->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole4 - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::hiPassVaryingCVaryingQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->poLastOutputPacket->afSamples;
    float32 const* afCutoff    = poFilter->cCutoff(poFilter)->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole4 - poFilter->fPole1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Band Pass Outputs
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandPassFixedCFixedQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->poLastOutputPacket->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)(poFilter->fPole4 - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandPassVaryingCFixedQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->poLastOutputPacket->afSamples;
    float32 const* afCutoff   = poFilter->cCutoff(poFilter)->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)(poFilter->fPole4 - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandPassFixedCFVaryingQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->poLastOutputPacket->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)(poFilter->fPole4 - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandPassVaryingCVaryingQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->poLastOutputPacket->afSamples;
    float32 const* afCutoff    = poFilter->cCutoff(poFilter)->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)(poFilter->fPole4 - poFilter->fPole1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Band Reject Outputs
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandRejectFixedCFixedQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->poLastOutputPacket->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandRejectVaryingCFixedQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->poLastOutputPacket->afSamples;
    float32 const* afCutoff   = poFilter->cCutoff(poFilter)->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandRejectFixedCFVaryingQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->poLastOutputPacket->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandRejectVaryingCVaryingQ(FourPoleMultiMode* poFilter) {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->poLastOutputPacket->afSamples;
    float32 const* afCutoff    = poFilter->cCutoff(poFilter)->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole1);
    }
}

} // namespace
