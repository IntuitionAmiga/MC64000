#ifndef MC64K_SYNTH_SIGNAL_WAVE_SAW_HPP
    #define MC64K_SYNTH_SIGNAL_WAVE_SAW_HPP

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
 * Downward saw wave
 */
class SawDown : public IWaveform {
    public:
        SawDown();
        ~SawDown();

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
        static inline float32 valueAt(float32 fTime) noexcept {
            fTime += HALF;
            return TWO * ((float32)std::ceil(fTime) - fTime - HALF);
        };

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const noexcept override {
            return valueAt(fTime);
        };

        /**
         * @inheritDoc
         */
        FixedShape getShape() const noexcept override {
            return IWaveform::SAW_DOWN;
        };

        /**
         * @inheritDoc
         */
        bool isDiscontinuous() const noexcept override {
            return true;
        }

        /**
         * @inheritDoc
         */
        bool isAperiodic() const noexcept override {
            return false;
        }
};

/**
 * Downward saw wave
 */
class SawUp : public IWaveform {
    public:
        SawUp();
        ~SawUp();

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
        static inline float32 valueAt(float32 fTime) noexcept {
            fTime += HALF;
            return TWO * (fTime - (float32)std::floor(fTime) - HALF);
        };

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const noexcept override {
            return valueAt(fTime);
        };

        /**
         * @inheritDoc
         */
        FixedShape getShape() const noexcept override {
            return IWaveform::SAW_UP;
        };

        /**
         * @inheritDoc
         */
        bool isDiscontinuous() const noexcept override {
            return true;
        }

        /**
         * @inheritDoc
         */
        bool isAperiodic() const noexcept override {
            return false;
        }
};

}

#endif
