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

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/waveform/sine.hpp>
namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * @inheritDoc
 */
Sine::Sine() {
    //std::fprintf(stderr, "Created Sine at %p\n", this);
}

/**
 * @inheritDoc
 *
 * Branchless techniques used here to improve throughput.
 */
void Sine::map(Packet const* poInput, Packet* poOutput) noexcept {
    float32* pDest      = poOutput->afSamples;
    float32 const* pSrc = poInput->afSamples;

    union {
        int32   iPi;
        float32 fPi;
    };

    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 fTime  = pSrc[i] - HALF;
        float32 fFloor = std::floor(fTime);

        // Branchless selection of pi/-pi
        iPi = PI_IEEE_32|(((int32)fFloor) & 1) << 31; // set sign bit when odd
        pDest[i] = OUTPUT_ADJUST * sinApprox(fPi * (fTime - fFloor - HALF));
    }
}

/**
 * @inheritDoc
 */
Sine::~Sine() {
    //std::fprintf(stderr, "Destroyed SineFast at %p\n", this);
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/waveform/triangle.hpp>
namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * @inheritDoc
 */
Triangle::Triangle() {
    //std::fprintf(stderr, "Created Triangle at %p\n", this);
}

/**
 * @inheritDoc
 */
void Triangle::map(Packet const* poInput, Packet* poOutput) noexcept {
    float32* pDest      = poOutput->afSamples;
    float32 const* pSrc = poInput->afSamples;

    union {
        int32   iTwo;
        float32 fTwo;
    };

    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 fTime  = pSrc[i] - HALF;
        float32 fFloor = std::floor(fTime);
        iTwo = TWO_IEEE_32 | (((int32)fFloor) & 1) << 31;
        pDest[i] = -fTwo * (fTime - fFloor - HALF);
    }
}

/**
 * @inheritDoc
 */
Triangle::~Triangle() {
    //std::fprintf(stderr, "Destroyed Triangle at %p\n", this);
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/waveform/saw.hpp>
namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * @inheritDoc
 */
SawDown::SawDown() {
    //std::fprintf(stderr, "Created SawDown at %p\n", this);
}

/**
 * @inheritDoc
 */
void SawDown::map(Packet const* poInput, Packet* poOutput) noexcept {
    float32* pDest      = poOutput->afSamples;
    float32 const* pSrc = poInput->afSamples;
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 fSrc = pSrc[i] + HALF;
        pDest[i] = TWO * ((float32)std::ceil(fSrc) - fSrc - HALF);
    }
}

/**
 * @inheritDoc
 */
SawDown::~SawDown() {
    //std::fprintf(stderr, "Destroyed SawDown at %p\n", this);
}

/**
 * @inheritDoc
 */
SawUp::SawUp() {
    //std::fprintf(stderr, "Created SawUp at %p\n", this);
}

/**
 * @inheritDoc
 */
void SawUp::map(Packet const* poInput, Packet* poOutput) noexcept {
    float32* pDest      = poOutput->afSamples;
    float32 const* pSrc = poInput->afSamples;
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        float32 fSrc = pSrc[i] + HALF;
        pDest[i] = TWO * (fSrc - (float32)std::floor(fSrc) - HALF);
    }
}

/**
 * @inheritDoc
 */
SawUp::~SawUp() {
    //std::fprintf(stderr, "Destroyed SawUp at %p\n", this);
}

}

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/waveform/square.hpp>
namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * @inheritDoc
 */
Square::Square() {
    //std::fprintf(stderr, "Created Square at %p\n", this);
}

/**
 * @inheritDoc
 *
 * Branchless techniques used here to improve throughput.
 */
void Square::map(Packet const* poInput, Packet* poOutput) noexcept {
    int32* pDest = (int32*)poOutput->afSamples;
    float32 const* pSrc = poInput->afSamples;
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        pDest[i] = ONE_IEEE_32 | ((int32)std::floor(pSrc[i]) & 1) << 31;
    }
}

/**
 * @inheritDoc
 */
Square::~Square() {
   //std::fprintf(stderr, "Destroyed Square at %p\n", this);
}

/**
 * @inheritDoc
 */
FixedPWM::FixedPWM(float32 fWidth) {
    setWidth(fWidth);
    //std::fprintf(stderr, "Created FixedPWM at %p with width %.3f\n", this, this->fWidth);
}
/**
 * @inheritDoc
 */
void FixedPWM::map(Packet const* poInput, Packet* poOutput) noexcept {
    int32* pDest = (int32*)poOutput->afSamples;
    float32 const* pSrc = poInput->afSamples;
    union {
        int32   iResult;
        float32 fResult;
    };
    for (unsigned i = 0; i < PACKET_SIZE; ++i) {
        fResult = std::ceil(pSrc[i]) - pSrc[i] - fWidth;
        pDest[i] = ONE_IEEE_32 | (iResult & 0x80000000);
    }
}

/**
 * @inheritDoc
 */
FixedPWM::~FixedPWM() {
   //std::fprintf(stderr, "Destroyed FixedPWM at %p\n", this);
}


ModulatedPWM::ModulatedPWM(IStream& roModulator, float32 fWidth):
    FixedPWM{fWidth},
    poModulator{&roModulator}
{
   std::fprintf(stderr, "Created ModulatedPWM at %p\n", this);
}

ModulatedPWM::ModulatedPWM(IStream::Ptr const& roModulatorPtr, float32 fWidth):
    FixedPWM{fWidth},
    oModulatorPtr{roModulatorPtr},
    poModulator{roModulatorPtr.get()}
{
   std::fprintf(stderr, "Created ModulatedPWM at %p\n", this);
}

ModulatedPWM::~ModulatedPWM() {
   std::fprintf(stderr, "Destroyed ModulatedPWM at %p\n", this);
}

ModulatedPWM* ModulatedPWM::setModulator(IStream& roModulator) noexcept {
    poModulator = &roModulator;
    if (nullptr == oModulationPacketPtr.get()) {
        oModulationPacketPtr = Packet::create();
    }
    return this;
}

ModulatedPWM* ModulatedPWM::setModulator(IStream::Ptr const& roModulatorPtr) noexcept {
    oModulatorPtr = roModulatorPtr;
    poModulator   = roModulatorPtr.get();
    if (poModulator && nullptr == oModulationPacketPtr.get()) {
        oModulationPacketPtr = Packet::create();
    }
    return this;
}

void ModulatedPWM::map(Packet const* poInput, Packet* poOutput) noexcept {
    float32 const* pfInput  = poInput->afSamples;
    int32*         piOutput = (int32*)poOutput->afSamples;
    union {
        int32   iResult;
        float32 fResult;
    };
    if (poModulator) {
        oModulationPacketPtr->scaleAndBiasBy(
            poModulator->emit(Packet::getCurrentIndex()).get(),
            0.5f * fWidth,
            0.5f
        );
        float32* pfWidth  = oModulationPacketPtr->afSamples;

        // branchless
        for (unsigned i = 0; i < PACKET_SIZE; ++i) {
            fResult = std::ceil(pfInput[i]) - pfInput[i] - pfWidth[i];
            piOutput[i] = ONE_IEEE_32 | (iResult & 0x80000000);
        }
    } else {
        for (unsigned i = 0; i < PACKET_SIZE; ++i) {
            fResult = std::ceil(pfInput[i]) - pfInput[i] - fWidth;
            piOutput[i] = ONE_IEEE_32 | (iResult & 0x80000000);
        }
    }
}


} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <random>
#include <synth/signal/waveform/noise.hpp>
#include <synth/signal/waveform/xform.hpp>

namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * 32-bit mersenne
 */
std::mt19937 mt_rand;

WhiteNoise::WhiteNoise() {
    fNormalise = 4.0f / (float64) mt_rand.max();
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        auRandom[u] = (uint32)mt_rand();
    }
}

WhiteNoise::~WhiteNoise() {
    //std::fprintf(stderr, "Destroyed WhiteNoise at %p\n", this);
}

/**
 * @inheritDoc
 *
 * We get one new random value, scale it to floating point then multiply our existing random
 * table by this value. This results in the low order bit preservation after overflow.
 */
void WhiteNoise::map(Packet const* poInput, Packet* poOutput) noexcept {

    constexpr float64 const RAND_SCALE = 1.0f/65536.0f;
    constexpr uint32  const WORD_MASK  = 0x7FFFFFFF;
    float32* afSamples  = poOutput->afSamples;
    float64  fRandom    = RAND_SCALE * (float64)mt_rand();
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        // Update the random buffer and output buffer as we go
        volatile uint32 uNextRandom = (uint32)(auRandom[u] * fRandom) & WORD_MASK;
        auRandom[u]  = uNextRandom;
        afSamples[u] = (float32)(uNextRandom * fNormalise) - 1.0f;
    }

}


float32 WhiteNoise::valueAt(float32 fTime) noexcept {
    return 0.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

class NoopDeleter {
    public:
        void operator()(IWaveform* poWaveform) const noexcept {
            //std::fprintf(stderr, "Not yet deleting shared waveform at %p\n", poWaveform);
        }
};

NoopDeleter oNoDelete;

///////////////////////////////////////////////////////////////////////////////////////////////////

// Fixed waveforms that can be allocated once
Sine        oSine;
Triangle    oTriangle;
SawDown     oSawDown;
SawUp       oSawUp;
Square      oSquare;
FixedPWM    oPWM10(0.1f);
FixedPWM    oPWM20(0.2f);
FixedPWM    oPWM30(0.3f);
FixedPWM    oPWM40(0.4f);
WhiteNoise  oNoise;

typedef XForm<0, false> ImmutableXForm2;
typedef XForm<1, false> ImmutableXForm4;
typedef XForm<2, false> ImmutableXForm8;

ImmutableXForm4 oPokey(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), {
        // phase multiplier, phase displacement, scale multiplier, bias
        1.0f, 2.0f, 0.25f, 1.0f,    // First  Quadrant
        1.0f, 1.0f, 0.05f, 0.7f,    // Second Quadrant
        1.0f, 2.0f, -0.25f, -1.0f,  // Third  Quadrant
        1.0f, 1.0f, -0.05f, -0.7f,  // Fourth Quadrant
    }
);


float32 const aHalfRect[8] = {
    // phase multiplier, phase displacement, scale multiplier, bias
    1.0f, 0.0f, 1.33f, -0.33f,  // First  Half
    1.0f, 0.0f, 0.0f, -0.33f,   // Second  Half
};
ImmutableXForm2 oSineHalfRect(IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), aHalfRect);
ImmutableXForm2 oTriangleHalfRect(IWaveform::Ptr(&Waveform::oTriangle, Waveform::oNoDelete), aHalfRect);

ImmutableXForm2 oSineFullRect(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), {
        // phase multiplier, phase displacement, scale multiplier, bias
        1.0f, 0.0f, 1.5f, -0.5f,  // First  Quadrant
        1.0f, 0.0f, 1.5f, -0.5f,  // Fourth Quadrant
    }
);

ImmutableXForm2 oSineCut(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), {
        // phase multiplier, phase displacement, scale multiplier, bias
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, -1.0f,
    }
);


ImmutableXForm4 oSineSaw(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), {
        // phase multiplier, phase displacement, scale multiplier, bias
        1.0f,  0.0f,  2.0f, -1.0f,  // First  Quadrant
        0.33f, 2.0f,  2.0f, 1.0f,  // Second Quadrant
        0.33f, 2.33f, 2.0f, 1.0f,  // Third  Quadrant
        0.33f, 2.66f, 2.0f, 1.0f,  // Fourth Quadrant
    }
);

ImmutableXForm4 oSinePinch(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), {
        // phase multiplier, phase displacement, scale multiplier, bias
        1.0f, 3.0f, 1.0f, 1.0f,  // First  Quadrant
        1.0f, 2.0f, 1.0f, 1.0f,  // Second Quadrant
        1.0f, 1.0f, 1.0f, -1.0f,  // Third  Quadrant
        1.0f, 0.0f, 1.0f, -1.0f,  // Fourth Quadrant
    }
);

ImmutableXForm4 oTX81Z_4(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete),
    { // phase multiplier, phase displacement, scale multiplier, bias
        1.0f, 3.0f, 1.33f, 1.0f,  // First  Quadrant
        1.0f, 2.0f, 1.33f, 1.0f,  // Second Quadrant
        1.0f, 2.0f, 0.0f, -0.33f,  // Third  Quadrant
        1.0f, 3.0f, 0.0f, -0.33f,  // Fourth Quadrant
    }
);

ImmutableXForm4 oTX81Z_5(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), {
        // phase multiplier, phase displacement, scale multiplier, bias
        2.0f, 0.0f, 1.0f, 0.0f,  // First  Quadrant
        2.0f, 2.0f, 1.0f, 0.0f,  // Second Quadrant
        1.0f, 2.0f, 0.0f, 0.0f,  // Third  Quadrant
        1.0f, 3.0f, 0.0f, 0.0f,  // Fourth Quadrant
    }
);

ImmutableXForm8 oTX81Z_6(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), {
        // phase multiplier, phase displacement, scale multiplier, bias
        2.0f, 6.0f, 1.0f, 1.0f,
        2.0f, 4.0f, 1.0f, 1.0f,
        2.0f, 2.0f, 1.0f, -1.0f,
        2.0f, 0.0f, 1.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
    }
);

ImmutableXForm4 oTX81Z_7(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), {
        // phase multiplier, phase displacement, scale multiplier, bias
        2.0f, 0.0f, 1.33f, -0.33f,
        2.0f, 0.0f, 1.33f, -0.33f,
        1.0f, 2.0f, 0.0f, -0.33f,
        1.0f, 3.0f, 0.0f, -0.33f,
    }
);

ImmutableXForm8 oTX81Z_8(
    IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete), {
        // phase multiplier, phase displacement, scale multiplier, bias
        2.0f, 6.0f, 1.0f, 1.0f,
        2.0f, 4.0f, 1.0f, 1.0f,
        2.0f, 6.0f, 1.0f, 1.0f,
        2.0f, 4.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f,
    }
);


} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////


namespace MC64K::Synth::Audio::Signal {

IWaveform::Ptr IWaveform::copy() {
    return Ptr(this, Waveform::oNoDelete);
}

IWaveform::Ptr IWaveform::get(IWaveform::FixedShape eShape) noexcept {

    switch (eShape) {
        default:
        case IWaveform::SINE:
        case IWaveform::TX81Z_1:
            return IWaveform::Ptr(&Waveform::oSine, Waveform::oNoDelete);

        case IWaveform::TRIANGLE:
            return IWaveform::Ptr(&Waveform::oTriangle, Waveform::oNoDelete);

        case IWaveform::SAW_DOWN:
            return IWaveform::Ptr(&Waveform::oSawDown, Waveform::oNoDelete);

        case IWaveform::SAW_UP:
            return IWaveform::Ptr(&Waveform::oSawUp, Waveform::oNoDelete);

        case IWaveform::SQUARE:
        case IWaveform::PULSE_50:
            return IWaveform::Ptr(&Waveform::oSquare,  Waveform::oNoDelete);

        case IWaveform::PULSE_10:
            return IWaveform::Ptr(&Waveform::oPWM10,  Waveform::oNoDelete);

        case IWaveform::PULSE_20:
            return IWaveform::Ptr(&Waveform::oPWM20,  Waveform::oNoDelete);

        case IWaveform::PULSE_30:
            return IWaveform::Ptr(&Waveform::oPWM30,  Waveform::oNoDelete);

        case IWaveform::PULSE_40:
            return IWaveform::Ptr(&Waveform::oPWM40,  Waveform::oNoDelete);

        case IWaveform::POKEY:
            return IWaveform::Ptr(&Waveform::oPokey,  Waveform::oNoDelete);

        case IWaveform::NOISE:
            return IWaveform::Ptr(&Waveform::oNoise,  Waveform::oNoDelete);

        case IWaveform::SINE_HALF_RECT:
        case IWaveform::TX81Z_3:
            return IWaveform::Ptr(&Waveform::oSineHalfRect, Waveform::oNoDelete);

        case IWaveform::SINE_FULL_RECT:
            return IWaveform::Ptr(&Waveform::oSineFullRect, Waveform::oNoDelete);

        case IWaveform::SINE_SAW:
            return IWaveform::Ptr(&Waveform::oSineSaw, Waveform::oNoDelete);

        case IWaveform::SINE_PINCH:
        case IWaveform::TX81Z_2:
            return IWaveform::Ptr(&Waveform::oSinePinch, Waveform::oNoDelete);

        case IWaveform::SINE_CUT:
            return IWaveform::Ptr(&Waveform::oSineCut, Waveform::oNoDelete);

        case IWaveform::TX81Z_4:
            return IWaveform::Ptr(&Waveform::oTX81Z_4, Waveform::oNoDelete);

        case IWaveform::TX81Z_5:
            return IWaveform::Ptr(&Waveform::oTX81Z_5, Waveform::oNoDelete);

        case IWaveform::TX81Z_6:
            return IWaveform::Ptr(&Waveform::oTX81Z_6, Waveform::oNoDelete);

        case IWaveform::TX81Z_7:
            return IWaveform::Ptr(&Waveform::oTX81Z_7, Waveform::oNoDelete);

        case IWaveform::TX81Z_8:
            return IWaveform::Ptr(&Waveform::oTX81Z_8, Waveform::oNoDelete);

        case IWaveform::TRIANGLE_HALF_RECT:
            return IWaveform::Ptr(&Waveform::oTriangleHalfRect, Waveform::oNoDelete);
    }
}

IWaveform::Ptr IWaveform::createPWM(float32 fWidth) {
    return IWaveform::Ptr(new Waveform::FixedPWM(fWidth));
}


}


