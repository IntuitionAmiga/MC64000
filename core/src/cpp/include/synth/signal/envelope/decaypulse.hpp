#ifndef MC64K_SYNTH_SIGNAL_ENV_DECAY_HPP
    #define MC64K_SYNTH_SIGNAL_ENV_DECAY_HPP

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
#include <synth/signal.hpp>

namespace MC64K::Synth::Audio::Signal::Envelope {

class DecayPulse : public IEnvelope {

    protected:
        float32     fInitial;
        float32     fHalflife;
        float32     fTarget;
        float64     fCurrent;
        float64     fDecayPerSample;

        void recalculateDecay() noexcept;

    public:
        DecayPulse(float32 fInitial, float32 fHalfLife, float fTarget = 0.0f);
        ~DecayPulse();

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex) noexcept override;

        /**
         * @param  float32 fInitial
         * @return this
         */
        DecayPulse* setInitial(float32 fInitial) noexcept;

        /**
         * @param  float32 fTarget
         * @return this
         */
        DecayPulse* setTarget(float32 fTarget) noexcept;

        /**
         * @param  float32 fTarget
         * @return this
         */
        DecayPulse* setHalflife(float32 fHalflife) noexcept;

};

} // namespace

#endif
