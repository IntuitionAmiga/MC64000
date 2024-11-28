#ifndef MC64K_SYNTH_MACHINE_POLYPHONIC_HPP
    #define MC64K_SYNTH_MACHINE_POLYPHONIC_HPP

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

#include <synth/machine.hpp>
#include <synth/signal/operator/mixer.hpp>

namespace MC64K::Synth::Audio::Machine {

class Polyphonic : public Signal::TStreamCommon, public virtual IMachine {

    protected:
        Signal::Operator::FixedMixer oMixer;

        float32 fOutputLevel;

        Polyphony ePolyphony;

        Polyphonic(Polyphony ePolyphony);

        void setVoiceSource(Voice eVoice, IStream& roSource, float32 fLevel) noexcept {
            oMixer.setInputStream((uint32)eVoice, roSource, fLevel);
        }

        void setVoiceSource(Voice eVoice, IStream::Ptr const& poSource, float32 fLevel) noexcept {
            oMixer.setInputStream((uint32)eVoice, poSource, fLevel);
        }

    public:

        Polyphony getNumVoices() const noexcept override {
            return ePolyphony;
        }

        float32 getVoiceLevel(Voice eVoice) const noexcept override {
            return oMixer.getInputLevel((uint32)eVoice);
        }

        float32 getOutputLevel() const noexcept override {
            return fOutputLevel;
        }

        size_t getPosition() const noexcept override {
            return oMixer.getPosition();
        }

        Polyphonic* reset() noexcept override {
            oMixer.reset();
            return this;
        }

        Polyphonic* setVoiceLevel(Voice eVoice, float32 fLevel) noexcept override {
            oMixer.setInputLevel((uint32)eVoice, fLevel);
            return this;
        }

        Polyphonic* setOutputLevel(float32 fLevel) noexcept override {
            fOutputLevel = fLevel;
            oMixer.setOutputLevel(VOICE_ATTENUATE * fLevel);
            return this;
        }

        Signal::Packet::ConstPtr emit(size_t uIndex) noexcept override {
            return Signal::Packet::getSilence();
        }
};

} // namespace

#endif
