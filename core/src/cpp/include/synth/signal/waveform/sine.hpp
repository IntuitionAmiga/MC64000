#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVE_SINE_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVE_SINE_HPP

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
 * Gold Standard Sine Generator (slow)
 */
class SineGold : public IWaveform {

    public:
        ~SineGold();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const {
            return TWO_PI;
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet::ConstPtr pInput);

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const {
            return std::sin(fTime);
        };

};

/**
 * Quick(er) sine generator.
 *
 * Uses a Taylor approximation for input values between -pi and pi. The stated waveform period is
 * 2.0 as we transform the linear input period steps values to a triangle wave that ramps between
 * -pi and pi, to be fed to the approximation.
 */
class SineFast : public IWaveform {
    private:
        // Taylor approximation terms
        static constexpr float32 const INV_3_FAC = (float32)(1.0/6.0);
        static constexpr float32 const INV_5_FAC = (float32)(1.0/120.0);

        // Adjusts the output of the approximation to reach 1.0
        static constexpr float32 const OUTPUT_ADJUST = (float32)(-1.0/1.004524827);

        /**
         * Sine approximation, using Taylor Series: sin(x) = x - x^3/3! + x^5/5!
         *
         * @param float32 fX
         * @return float
         */
        float32 sinApprox(float32 fX) const {
            float32 fX2      = fX * fX;
            float32 fTerm    = fX * fX2;               // fTerm = x^3
            float32 fResult  = fX - fTerm * INV_3_FAC; // x - x^3/3!
            fTerm           *= fX2;                    // fTerm = x^5
            return fResult + fTerm * INV_5_FAC;        // x - x^3/3! + x^5/5!
        }

    public:
        ~SineFast();

        /**
         * @inheritDoc
         */
        float32 getPeriod() const {
            return TWO;
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet::ConstPtr pInput);

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const {
            fTime  -= HALF;
            float32 fFloor = std::floor(fTime);
            union {
                int32   iPi;
                float32 fPi;
            };
            // Branchless selection of pi/-pi
            iPi = PI_IEEE_32|(((int32)fFloor) & 1) << 31; // set sign bit when odd
            return OUTPUT_ADJUST * sinApprox(fPi * (fTime - fFloor - HALF));
        };

};

}

#endif
