#ifndef MC64K_SYNTH_SIGNAL_OSCILLATOR_HPP
    #define MC64K_SYNTH_SIGNAL_OSCILLATOR_HPP

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

#include <cmath>
#include "stream.hpp"
#include "waveform.hpp"

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;

/**
 * IOscillator
 *
 * Abstract Base Class for Oscillator types
 */
class IOscillator : public TStreamCommon, public TOutputStream, protected TPacketIndexAware {

    protected:
        static constexpr uint64 const SAMPLE_COUNTER_MASK = 0xFFF;

        IWaveform::Ptr oWaveformPtr;
        Packet::Ptr    oWaveInputPacketPtr;
        IWaveform*     poWaveform;
        float64        fTimeStep;
        float64        fScaleVal;
        float32        fFrequency;
        float32        fCurrentFrequency;
        float32        fPhaseOffset;
        float32        fPhaseCorrection;
        float32        fWaveformPeriod;
        bool           bAperiodic;

        /**
         * @inheritDoc
         *
         * Overridden to return false if we don't have a waveform set.
         */
        bool           canEnable() const noexcept override;

        /**
         * Limit the frequency. This is intended to be overridden
         * by the implementaiton to clamp the frequency into a viable range.
         *
         * @param float32
         * @return float32
         */
        virtual float32 clampFrequency(float32 fFrequency) const noexcept {
            return fFrequency;
        };

        /**
         * Generate a new packet. This is called by emit() when it is determined that
         * the packet we are being asked for is not the one we last calculated.
         *
         * @return Packet::ConstPtr
         */
        virtual Packet::ConstPtr emitNew() noexcept = 0;

        /**
         * Retuns the sample position, modulo the counter size. This value is guaranteed
         * to remain representable in IEEE-754 32-bit floating point.
         *
         * @return uint32
         */
        uint32 getCyclicSampleCounter() const noexcept {
            return (uint32)(uSamplePosition & SAMPLE_COUNTER_MASK);
        }

        void handleCyclicSampleCounterReset(float32 fLastSample) noexcept {
            if (!(uSamplePosition & SAMPLE_COUNTER_MASK)) {
                fPhaseCorrection = (float32)(
                    std::fmod(
                        fLastSample,
                        fWaveformPeriod
                    ) + fScaleVal
                );
            }
        }

    public:
        /**
         * @param IWaveform::Ptr roWaveformPtr
         * @param float32        fFrequency
         * @param float32        fPhase
         */
        IOscillator(
            IWaveform::Ptr const& roWaveformPtr,
            float32 fFrequency = 0.0f,
            float32 fPhase     = 0.0f
        );

        /**
         * Set the waveform to use. Passing null disables the oscillator (emits silence).
         *
         * Implementations may clone the waveform instance passed to them so do not rely on getWaveform() returning
         * the same instance.
         *
         * @param  IWaveform::Ptr poWaveform
         * @return this
         */
        IOscillator* setWaveform(IWaveform::Ptr const& roWaveformPtr) noexcept;

        IOscillator* setWaveformUnmanaged(IWaveform& roWaveform) noexcept {
            poWaveform      = &roWaveform;
            fWaveformPeriod = poWaveform->getPeriod();
            fTimeStep       = fWaveformPeriod * SAMPLE_PERIOD;
            fScaleVal       = fTimeStep * fFrequency;
            bAperiodic      = false;
            return this;
        }

        /**
         * Set the baseline frequency to emit.
         *
         * @param  float fFrequency
         * @return this
         */
        IOscillator* setFrequency(float32 fFrequency) noexcept;

        /**
         * @inheritDoc
         */
        IOscillator* reset() noexcept override;

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex = 0) noexcept override;

        typedef std::shared_ptr<IOscillator> Ptr;
        typedef std::shared_ptr<IOscillator const> ConstPtr;
};

}
#endif
