#ifndef MC64K_SYNTH_CTRL_CURVE_LINEAR_HPP
    #define MC64K_SYNTH_CTRL_CURVE_LINEAR_HPP

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
#include <synth/controlcurve.hpp>

namespace MC64K::Synth::Audio::ControlCurve {

/**
 * Linear
 *
 * Linearly interpolates the input value between the defined output range
 */
class Linear : public IControlCurve {
    private:
        float32 fInBase;
        float32 fInScale;
        float32 fOutBase;
        float32 fOutScale;

    public:
        Linear(
            float32 fOutMin,
            float32 fOutMax,
            float32 fInRangeMin = DEF_RANGE_MIN,
            float32 fInRangeMax = DEF_RANGE_MAX
        );
        ~Linear();

        /**
         * @inheritDoc
         */
        float32 map(float32 const) const override;
};

}
#endif
