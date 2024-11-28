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

namespace MC64K::Synth::Audio::Signal {

/**
 * @inheritDoc
 *
 * Ensure the reset signal is propagated to the inputs.
 */
IFilter* IFilter::reset() noexcept {
    uSamplePosition = 0;
    uLastIndex      = 0;

    if (poInputStream) {
        poInputStream->reset();
    }
    if (poCutoffModulator) {
        poCutoffModulator->reset();
    }
    if (poCutoffEnvelope) {
        poCutoffEnvelope->reset();
    }
    if (poResonanceModulator) {
        poResonanceModulator->reset();
    }
    if (poResonanceEnvelope) {
        poResonanceEnvelope->reset();
    }
    return this;
}

bool IFilter::canEnable() const noexcept {
    return poInputStream != nullptr;
}

/**
 * @inheritDoc
 *
 * Ensure the enable signal is propagated to the inputs.
 */
IFilter* IFilter::enable() noexcept {
    TStreamCommon::enable();
    if (isEnabled()) {
        std::fprintf(stderr, "IFilter - enabling input streams...\n");
        // Turn on all the inputs
        if (poInputStream) {
            poInputStream->enable();
        }
        if (poCutoffModulator) {
            poCutoffModulator->enable();
        }
        if (poCutoffEnvelope) {
            poCutoffEnvelope->enable();
        }
        if (poResonanceModulator) {
            poResonanceModulator->reset();
        }
        if (poResonanceEnvelope) {
            poResonanceEnvelope->reset();
        }
    } else {
        std::fprintf(stderr, "IFilter - cannot enable input streams...\n");
    }
    return this;
}

}
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/filter/4polemulti.hpp>
namespace MC64K::Synth::Audio::Signal::Filter {
using namespace MC64K::StandardTestHost::Audio::IConfig;

FourPoleMultiMode::FourPoleMultiMode(IStream& roInput, Mode eMode, float32 fCutoff, float32 fResonance) :
    IFilter(roInput, fCutoff, fResonance),
    eMode{eMode}
{
    std::fprintf(stderr, "Created FourPoleMultiMode at %p\n", this);
    configure();
}


FourPoleMultiMode::FourPoleMultiMode(IStream::Ptr const& roInputPtr, Mode eMode, float32 fCutoff, float32 fResonance) :
    IFilter(roInputPtr, fCutoff, fResonance),
    eMode{eMode}
{
    std::fprintf(stderr, "Created FourPoleMultiMode at %p\n", this);
    configure();
}

FourPoleMultiMode::~FourPoleMultiMode() {
    std::fprintf(stderr, "Destroyed FourPoleMultiMode at %p\n", this);
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::filterSample(float64 fInput, float64 fCutoff, float64 fResonance) noexcept {
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
Packet::ConstPtr FourPoleMultiMode::emit(uint64 uIndex) noexcept {
    if (!bEnabled) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return oOutputPacketPtr;
    }
    cProcess(this);
    return oOutputPacketPtr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Input controls
//
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::cutoffMod(FourPoleMultiMode* poFilter) noexcept {
    return poFilter->poCutoffModulator->emit(poFilter->uLastIndex);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::cutoffEnv(FourPoleMultiMode* poFilter) noexcept {
    return poFilter->poCutoffEnvelope->emit(poFilter->uLastIndex);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::cutoffModEnv(FourPoleMultiMode* poFilter) noexcept {
    Packet::Ptr poPacket   = poFilter->poCutoffModulator->emit(poFilter->uLastIndex)->clone();
    poPacket->modulateWith(poFilter->poCutoffEnvelope->emit(poFilter->uLastIndex));
    return poPacket;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::resonanceMod(FourPoleMultiMode* poFilter) noexcept {
    return poFilter->poResonanceModulator->emit(poFilter->uLastIndex);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::resonanceEnv(FourPoleMultiMode* poFilter) noexcept {
    return poFilter->poResonanceEnvelope->emit(poFilter->uLastIndex);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FourPoleMultiMode::resonanceModEnv(FourPoleMultiMode* poFilter) noexcept {
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
void FourPoleMultiMode::lowPassFixedCFixedQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->oOutputPacketPtr->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)poFilter->fPole4;
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::lowPassVaryingCFixedQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->oOutputPacketPtr->afSamples;
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
void FourPoleMultiMode::lowPassFixedCFVaryingQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->oOutputPacketPtr->afSamples;
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
void FourPoleMultiMode::lowPassVaryingCVaryingQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->oOutputPacketPtr->afSamples;
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
void FourPoleMultiMode::hiPassFixedCFixedQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->oOutputPacketPtr->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole4);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::hiPassVaryingCFixedQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->oOutputPacketPtr->afSamples;
    float32 const* afCutoff   = poFilter->cCutoff(poFilter)->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance);
        //afOutput[u] = (float32)(afInput[u] - poFilter->fPole4 - poFilter->fPole1);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole4);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::hiPassFixedCFVaryingQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->oOutputPacketPtr->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole4);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::hiPassVaryingCVaryingQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->oOutputPacketPtr->afSamples;
    float32 const* afCutoff    = poFilter->cCutoff(poFilter)->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole4);
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
void FourPoleMultiMode::bandPassFixedCFixedQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->oOutputPacketPtr->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)(poFilter->fPole4 - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandPassVaryingCFixedQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->oOutputPacketPtr->afSamples;
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
void FourPoleMultiMode::bandPassFixedCFVaryingQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->oOutputPacketPtr->afSamples;
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
void FourPoleMultiMode::bandPassVaryingCVaryingQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->oOutputPacketPtr->afSamples;
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
void FourPoleMultiMode::bandRejectFixedCFixedQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->oOutputPacketPtr->afSamples;
    float64        fResonance = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], poFilter->fFixedCutoff, fResonance);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole1);
    }
}

/**
 * @inheritDoc
 */
void FourPoleMultiMode::bandRejectVaryingCFixedQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput    = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput   = poFilter->oOutputPacketPtr->afSamples;
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
void FourPoleMultiMode::bandRejectFixedCFVaryingQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->oOutputPacketPtr->afSamples;
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
void FourPoleMultiMode::bandRejectVaryingCVaryingQ(FourPoleMultiMode* poFilter) noexcept {
    float32 const* afInput     = poFilter->poInputStream->emit(poFilter->uLastIndex)->afSamples;
    float32*       afOutput    = poFilter->oOutputPacketPtr->afSamples;
    float32 const* afCutoff    = poFilter->cCutoff(poFilter)->afSamples;
    float32 const* afResonance = poFilter->cResonance(poFilter)->afSamples;
    float64        fResonance  = poFilter->fFixedResonance * SCALE_MAX_Q;
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        poFilter->filterSample(afInput[u], afCutoff[u] * poFilter->fFixedCutoff, fResonance * afResonance[u]);
        afOutput[u] = (float32)(afInput[u] - poFilter->fPole1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void FourPoleMultiMode::configure() noexcept {
    // Lookup table of cutoff input control callbacks
    static constexpr Control const acCutoffOpts[4]    = {
        0,
        cutoffMod,
        cutoffEnv,
        cutoffModEnv
    };
    // Lookup table of resonance input control callbacks
    static constexpr Control const acResonanceOpts[4] = {
        0,
        resonanceMod,
        resonanceEnv,
        resonanceModEnv
    };
    // Lookup table of process callbacks
    static constexpr Process const acProcessOpts[16] = {
        lowPassFixedCFixedQ,
        lowPassVaryingCFixedQ,
        lowPassFixedCFVaryingQ,
        lowPassVaryingCVaryingQ,
        hiPassFixedCFixedQ,
        hiPassVaryingCFixedQ,
        hiPassFixedCFVaryingQ,
        hiPassVaryingCVaryingQ,
        bandPassFixedCFixedQ,
        bandPassVaryingCFixedQ,
        bandPassFixedCFVaryingQ,
        bandPassVaryingCVaryingQ,
        bandRejectFixedCFixedQ,
        bandRejectVaryingCFixedQ,
        bandRejectFixedCFVaryingQ,
        bandRejectVaryingCVaryingQ,
    };

    cCutoff = acCutoffOpts[
        ((poCutoffModulator ? 1 : 0) | (poCutoffEnvelope ? 2 : 0))
    ];

    cResonance = acResonanceOpts[
        ((poResonanceModulator ? 1 : 0) | (poResonanceEnvelope ? 2 : 0))
    ];

    cProcess = acProcessOpts[
        (eMode << 2) | (cResonance ? 2 : 0) | (cCutoff ? 1 : 0)
    ];

    static char const* asControl[4] = {
        "--- ---",
        "Mod ---",
        "--- Env",
        "Mod Env",
    };

    static char const* asMode[4] = {
        "Low Pass",
        "High Pass",
        "Band Pass",
        "Band Reject",
    };

    std::fprintf(
        stderr,
        "Filter inputs\n"
        "\tInput     : %p\n"
        "\tCutoff Mod: %p\n"
        "\tCutoff Env: %p\n"
        "\tReso   Mod: %p\n"
        "\tReso   Env: %p\n",
        poInputStream,
        poCutoffModulator,
        poCutoffEnvelope,
        poResonanceModulator,
        poResonanceEnvelope
    );

    std::fprintf(
        stderr,
        "Filter configuration: %s [Cut: %s %.5f] [Res: %s %.5f]\n",
        asMode[eMode],
        asControl[((poCutoffModulator    ? 1 : 0) | (poCutoffEnvelope ?  2 : 0))],
        fFixedCutoff,
        asControl[((poResonanceModulator ? 1 : 0) | (poResonanceEnvelope ? 2 : 0))],
        fFixedResonance
    );

}

} // namespace
