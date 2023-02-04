#ifndef MC64K_SYNTH_CTRL_CURVE_OCTAVE_HPP
    #define MC64K_SYNTH_CTRL_CURVE_OCTAVE_HPP

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

#include <misc/scalar.hpp>
#include <synth/note.hpp>
#include <synth/controlcurve.hpp>

namespace MC64K::Synth::Audio::ControlCurve {

/**
 * Flat control curve. Ignores whatever value is passed and returns the fixed value passed
 * on construction.
 */
class Octave : public IControlCurve {
    private:
        float32 fCentreOutput;
        float32 fScalePerOctave;
        float32 fCentrePosition;

    public:
        Octave(
            float32 fCentre,
            float32 fScalePerOctave = 1.0f,
            float32 fStepsPerOctave = (float32)Note::SEMIS_PER_OCTAVE,
            float32 fCentrePosition = (float32)Note::CENTRE_REFERENCE
        );
        ~Octave();

        /**
         * @inheritDoc
         */
        float32 map(float32 const);
};

}
#endif
