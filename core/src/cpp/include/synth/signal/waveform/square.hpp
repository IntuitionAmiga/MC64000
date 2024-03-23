#ifndef MC64K_SYNTH_SIGNAL_WAVE_SQUARE_HPP
    #define MC64K_SYNTH_SIGNAL_WAVE_SQUARE_HPP

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
 * Basic square wave
 */
class Square : public IWaveform {
    public:
        Square();
        ~Square();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const override {
            return TWO;
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet const* poInput) override;

        /**
         * Static version of the value function that can be called and inlined explicitly from
         * anywhere that has checked getShape()
         */
        static inline float32 valueAt(float32 fTime) {
            union {
                int32   iResult;
                float32 fResult;
            };
            iResult = ONE_IEEE_32 | ((int32)std::floor(fTime) & 1) << 31;
            return fResult;
        };

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const override {
            return valueAt(fTime);
        };

        /**
         * @inheritDoc
         */
        FixedShape getShape() const override {
            return IWaveform::SQUARE;
        };

        /**
         * @inheritDoc
         */
        bool isDiscontinuous() const override {
            return true;
        }

        /**
         * @inheritDoc
         */
        bool isAperiodic() const override {
            return false;
        }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Basic fixed duty cycle PWM
 */
class FixedPWM : public IWaveform {
    private:
        float32 fWidth;

    public:
        static constexpr float32 const MIN_WIDTH = (float32)(1.0/16.0);
        static constexpr float32 const MAX_WIDTH = (float32)(15.0/16.0);

        ~FixedPWM();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const override {
            return ONE;
        }

        /**
         * @inheritDoc
         */
        float32 getWidth() const {
            return fWidth;
        }

        /**
         * Sets the new PWM duty cycle
         *
         * @param float32 fNewWidth
         */
        void setWidth(float32 fNewWidth) {
            fWidth = fNewWidth < MIN_WIDTH ? MIN_WIDTH : (fNewWidth > MAX_WIDTH ? MAX_WIDTH : fNewWidth);
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet const* poInput) override;

        /**
         * Static version of the value function that can be called and inlined explicitly from
         * anywhere that has checked getShape()
         */
        static inline float32 valueAt(float32 fTime, float32 fWidth) {
            union {
                int32   iResult;
                float32 fResult;
            };
            fResult = std::ceil(fTime) - fTime - fWidth;
            iResult = ONE_IEEE_32 | (iResult & 0x80000000);
            return fResult;
        };

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const override {
            return valueAt(fTime, fWidth);
        };

        /**
         * @inheritDoc
         *
         * Note that fixed variants, e.g. PULSE_10 will still return PULSE
         */
        FixedShape getShape() const override {
            return IWaveform::PULSE;
        };

        /**
         * @inheritDoc
         */
        bool isDiscontinuous() const override {
            return true;
        }

        /**
         * @inheritDoc
         */
        bool isAperiodic() const override {
            return false;
        }

        /**
         * Constructor
         */
        FixedPWM(float32 fWidth);

        /**
         * @inheritDoc
         */
        Ptr copy() {
            return Ptr(new FixedPWM(fWidth));
        }
};

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Basic fixed duty cycle PWM
 */
class ModulatedPWM : public IWaveform {

    private:
        IStream::Ptr poWidthModulator;
        float32      fModDepth;
        float32      fModBias;
    public:
        ModulatedPWM(IStream::Ptr const& poModulator, float32 fModDepth, float32 fModBias);
        ~ModulatedPWM();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const override {
            return ONE;
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet const* poInput) override;

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const;

        /**
         * @inheritDoc
         */
        FixedShape getShape() const override {
            return IWaveform::PULSE;
        };

        /**
         * @inheritDoc
         */
        bool isDiscontinuous() const override {
            return true;
        }

        /**
         * @inheritDoc
         */
        bool isAperiodic() const override {
            return false;
        }

        /**
         * @inheritDoc
         */
        Ptr copy();
    };
}

#endif
