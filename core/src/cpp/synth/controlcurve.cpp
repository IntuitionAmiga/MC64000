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
#include <synth/controlcurve.hpp>

#include <synth/controlcurve/flat.hpp>
#include <synth/controlcurve/linear.hpp>
#include <synth/controlcurve/gamma.hpp>
#include <synth/controlcurve/octave.hpp>

namespace MC64K::Synth::Audio::ControlCurve {

///////////////////////////////////////////////////////////////////////////////////////////////////

Flat::Flat(float32 fValue): fFixed{fValue} {
    std::fprintf(stderr, "Created CC Flat at %p with value %.5f\n", this, fFixed);
}

Flat::~Flat() {
    std::fprintf(stderr, "Destoyed CC Flat at %p\n", this);
}

float32 Flat::map(float32 const) const {
    return fFixed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Linear::Linear(
    float32 fOutMin,
    float32 fOutMax,
    float32 fInRangeMin,
    float32 fInRangeMax
) : fInBase{fInRangeMin}, fOutBase{fOutMin} {
    fOutScale = fOutMax - fOutMin;
    fInScale  = 1.0f / (fInRangeMax - fInRangeMin);
    std::fprintf(
        stderr,
        "Created CC Linear at %p mapping [%.5f ... %.5f] to [%.5f ... %.5f]\n",
        this,
        fInRangeMin,
        fInRangeMax,
        fOutMin,
        fOutMax
    );
}

Linear::~Linear() {
    std::fprintf(stderr, "Destoyed CC Linear at %p\n", this);
}

/**
 * @inheritDoc
 */
float32 Linear::map(float32 const fInput) const {
    float32 fValue = fInScale * (fInput - fInBase);
    return fOutBase + fValue * fOutScale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Gamma::Gamma(
    float32 fOutMin,
    float32 fOutMax,
    float32 fGamma,
    float32 fInRangeMin,
    float32 fInRangeMax
) : fInBase{fInRangeMin}, fOutBase{fOutMin}, fGamma{fGamma} {
    fOutScale = fOutMax - fOutMin;
    fInScale  = 1.0f / (fInRangeMax - fInRangeMin);
    std::fprintf(
        stderr,
        "Created CC Gamma at %p mapping [%.5f ... %.5f] to [%.5f ... %.5f] with gamma %.5f\n",
        this,
        fInRangeMin,
        fInRangeMax,
        fOutMin,
        fOutMax,
        fGamma
    );
}

Gamma::~Gamma() {
    std::fprintf(stderr, "Destoyed CC Gamma at %p\n", this);
}

/**
 * @inheritDoc
 */
float32 Gamma::map(float32 const fInput) const {
    float32 fValue = fInScale * (fInput - fInBase);
    return fOutBase + (float32)std::pow(fValue, fGamma) * fOutScale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

Octave::Octave(
    float32 fInitialCentreOutput,
    float32 fInitialScalePerOctave,
    float32 fInitialStepsPerOctave,
    float32 fInitialCentrePosition
):  fCentreOutput{fInitialCentreOutput},
    fCentrePosition{fInitialCentrePosition}
{
    fScalePerOctave = fInitialScalePerOctave / fInitialStepsPerOctave;
    fScalePerOctave = fScalePerOctave >= 0.0f ? fScalePerOctave : -1.0f / fScalePerOctave;
    std::fprintf(stderr, "Created CC Octave at %p\n", this);
}

Octave::~Octave() {
    std::fprintf(stderr, "Destoyed CC Octave at %p\n", this);
}

/**
 * @inheritDoc
 */
float32 Octave::map(float32 const fInput) const {
    float32 fControlValue = fInput - fCentrePosition;
    return fCentreOutput * (float32)std::exp2(fScalePerOctave * fControlValue);
}

} // namespace
