#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_OSCILLATOR_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_OSCILLATOR_HPP

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

#include "stream.hpp"
#include "waveform.hpp"

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;

/**
 * Abstract Base Class for Oscillator types
 */
class IOscillator : public TStreamCommon, private TPacketIndexAware {

    protected:
        IWaveform::Ptr pWaveform;
        Packet::Ptr    pLastOutput;
        float32 fFrequency;
        float32 fCurrentFrequency;
        float32 fPhaseOffset;
        float32 fPhaseCorrection;
        float32 fWaveformPeriod;
        float32 fTimeStep;
        float32 fScaleVal;
        bool    bAperiodic;

        virtual float32 clampFrequency(float32 fFrequency) {
            return fFrequency;
        };

        virtual Packet::Ptr emitNew() = 0;


    public:
        IOscillator(
            IWaveform::Ptr pWaveform,
            float32 fFrequency = 0.0f,
            float32 fPhase     = 0.0f
        );

        /**
         * Set the waveform to use. Passing null disables the oscillator (emits silence).
         *
         * Implementations may clone the waveform instance passed to them so do not rely on getWaveform() returning
         * the same instance.
         *
         * @param  IWaveform::Ptr pWaveform
         * @return this
         */
        IOscillator* setWaveform(IWaveform::Ptr pWaveform);

        /**
         * Set the baseline frequency to emit.
         *
         * @param  float fFrequency
         * @return this
         */
        IOscillator* setFrequency(float32 fFrequency);

        IOscillator* reset();

        Packet::Ptr emit(size_t uIndex = 0);
};

}
#endif
