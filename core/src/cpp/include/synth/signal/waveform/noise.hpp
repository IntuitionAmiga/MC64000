#ifndef MC64K_SYNTH_SIGNAL_WAVE_NOISE_HPP
    #define MC64K_SYNTH_SIGNAL_WAVE_NOISE_HPP

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
 * Sine
 *
 * Uses a Taylor approximation for input values between -pi and pi. The stated waveform period is
 * 2.0 as we transform the linear input period steps values to a triangle wave that ramps between
 * -pi and pi, to be fed to the approximation.
 */
class WhiteNoise : public IWaveform {

    private:
        uint32  auRandom[PACKET_SIZE];
        float64 fNormalise;

    public:
        WhiteNoise();
        ~WhiteNoise();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const noexcept override {
            return ONE;
        }

        /**
         * @inheritDoc
         */
        void map(Packet const* poInput, Packet* poOutput) noexcept override;

        /**
         * Static version of the value function that can be called and inlined explicitly from
         * anywhere that has checked getShape()
         */
        static float32 valueAt(float32 fTime) noexcept;

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const noexcept override {
            return valueAt(fTime);
        }

        /**
         * @inheritDoc
         */
        FixedShape getShape() const noexcept override {
            return IWaveform::NOISE;
        };

        /**
         * @inheritDoc
         */
        bool isDiscontinuous() const noexcept override {
            return false;
        }

        /**
         * @inheritDoc
         */
        bool isAperiodic() const noexcept override {
            return true;
        }
};

}

#endif
