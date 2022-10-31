#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_OSC_BASE_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_OSC_BASE_HPP

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

#include <synth/signal.hpp>

namespace MC64K::Synth::Audio::Signal::Oscillator {

class LFO : public IOscillator {

    protected:
        float32 fDepth;
        float32 fBias;
        bool    bRetrigger;

        float32 clampFrequency(float32 fFrequency);
        Packet::Ptr emitNew();

    public:
        static constexpr float32 const MIN_FREQUENCY = 1.0f/60.0f;
        static constexpr float32 const DEF_FREQUENCY = 2.0f;
        static constexpr float32 const MAX_FREQUENCY = 64.0f;

        LFO(
            IWaveform::Ptr pWaveform,
            float32 fFrequency = 0.0f,
            float32 fDepth     = 0.5f,
            bool    bRetrigger = false
        ) : IOscillator(pWaveform, fFrequency, 0.0f), fDepth(fDepth), fBias(0.0f), bRetrigger(bRetrigger) {}

        ~LFO();

        LFO* enableRetrigger() {
            bRetrigger = true;
            return this;
        }

        LFO* disableRetrigger() {
            bRetrigger = true;
            return this;
        }

        LFO* reset() {
            if (bRetrigger) {
                IOscillator::reset();
            }
            return this;
        }

};

}

#endif
