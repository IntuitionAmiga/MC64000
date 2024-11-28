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
        float32 getPeriod() const noexcept override {
            return TWO;
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
        float32 value(float32 fTime) const noexcept override {
            return valueAt(fTime);
        };

        /**
         * @inheritDoc
         */
        FixedShape getShape() const noexcept override {
            return IWaveform::SQUARE;
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

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Basic fixed duty cycle PWM
 */
class FixedPWM : public IWaveform {
    protected:
        float32 fWidth;

    public:
        static constexpr float32 const MIN_WIDTH = (float32)(1.0/16.0);
        static constexpr float32 const MAX_WIDTH = (float32)(15.0/16.0);

        ~FixedPWM();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const noexcept override {
            return ONE;
        }

        /**
         * @inheritDoc
         */
        float32 getWidth() const noexcept {
            return fWidth;
        }

        /**
         * Sets the new PWM duty cycle
         *
         * @param float32 fNewWidth
         */
        void setWidth(float32 fNewWidth) noexcept {
            fWidth = fNewWidth < MIN_WIDTH ? MIN_WIDTH : (fNewWidth > MAX_WIDTH ? MAX_WIDTH : fNewWidth);
        }

        /**
         * @inheritDoc
         */
        void map(Packet const* poInput, Packet* poOutput) noexcept override;

        /**
         * Static version of the value function that can be called and inlined explicitly from
         * anywhere that has checked getShape()
         */
        static inline float32 valueAt(float32 fTime, float32 fWidth) noexcept {
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
        float32 value(float32 fTime) const noexcept override {
            return valueAt(fTime, fWidth);
        };

        /**
         * @inheritDoc
         *
         * Note that fixed variants, e.g. PULSE_10 will still return PULSE
         */
        FixedShape getShape() const noexcept override {
            return IWaveform::PULSE;
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
 * Modulatable duty cycle PWM
 */
class ModulatedPWM : public FixedPWM {

    private:
        Packet::Ptr  oModulationPacketPtr;
        IStream::Ptr oModulatorPtr;
        IStream*     poModulator;

    public:
        ModulatedPWM(IStream& roModulator, float32 fWidth);
        ModulatedPWM(IStream::Ptr const& roModulatorPtr, float32 fWidth);
        ~ModulatedPWM();

        ModulatedPWM* setModulator(IStream& roModulator) noexcept;
        ModulatedPWM* setModulator(IStream::Ptr const& roModulatorPtr) noexcept;

        /**
         * @inheritDoc
         */
        void map(Packet const* poInput, Packet* poOutput) noexcept override;

        Ptr copy() {
            auto poWave = new ModulatedPWM(oModulatorPtr, fWidth);
            poWave->poModulator = poModulator;
            return Ptr(poWave);
        }
    };
}

#endif
