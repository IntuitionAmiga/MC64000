#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVE_XFORM_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVE_XFORM_HPP

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

#include "constants.hpp"

namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * Transformer
 */
class XForm : public IWaveform {

    private:
        static constexpr float32 const PERIOD = 4.0f;

        enum {
            PHASE_MUL = 0,
            PHASE_ADD = 1,
            LEVEL_MUL = 2,
            LEVEL_ADD = 3
        };

        float32 aTransform[16] __attribute__ ((aligned (16))) = {
            // phase multiplier, phase displacement, scale multiplier, bias
           1.0f, 0.0f, 1.0f, 0.0f,  // First  Quadrant
           1.0f, 1.0f, 1.0f, 0.0f,  // Second Quadrant
           1.0f, 2.0f, 1.0f, 0.0f,  // Third  Quadrant
           1.0f, 3.0f, 1.0f, 0.0f,  // Fourth Quadrant

        };

        Ptr     pSourceWaveform;
        float32 fPeriodAdjust;

    public:
        XForm(Ptr pSourceWaveform, float32 const* pTransform);
        ~XForm();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const {
            return PERIOD;
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet const* pInput);

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const {
            return fTime; // todo
        };

        /**
         * Returns the enumerated shape identifier for the waveform.
         */
        FixedShape getShape() const {
            return IWaveform::XFORM;
        };

        /**
         * Returns whether or not the wave contains sharp discontinuities.
         */
        bool isDiscontinuous() const {
            return true;
        }

        /**
         * @inheritDoc
         */
        Ptr copy() {
            return Ptr(
                new XForm(
                    pSourceWaveform->copy(),
                    aTransform
                )
            );
        }
};

}

#endif
