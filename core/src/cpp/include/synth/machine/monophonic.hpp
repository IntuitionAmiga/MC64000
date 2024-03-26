#ifndef MC64K_SYNTH_MACHINE_MONOPHONIC_HPP
    #define MC64K_SYNTH_MACHINE_MONOPHONIC_HPP

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
#include <synth/signal/operator/leveladjust.hpp>

namespace MC64K::Synth::Audio::Machine {

class Monophonic : public Signal::TStreamCommon, public virtual IMachine {

    protected:
        Signal::Operator::LevelAdjust oVoice;

        float32 fOutputLevel;
        float32 fVoiceLevel;

        Monophonic();

        void applyLevel() {
            oVoice.setOutputLevel(VOICE_ATTENUATE * fOutputLevel * fVoiceLevel);
        }

        void setVoiceSource(IStream& roSource, float32 fLevel) {
            fVoiceLevel = fLevel;
            oVoice.setSourceInput(roSource);
            applyLevel();
        }

        void setVoiceSource(IStream::Ptr const& poSource, float32 fLevel) {
            fVoiceLevel = fLevel;
            oVoice.setSourceInput(poSource);
            applyLevel();
        }

    public:

        Polyphony getNumVoices() const override {
            return ONE_VOICE;
        }

        float32 getVoiceLevel(Voice eVoice) const override {
            return fVoiceLevel;
        }

        float32 getOutputLevel() const override {
            return fOutputLevel;
        }

        size_t getPosition() const override {
            return oVoice.getPosition();
        }

        Monophonic* reset() override {
            oVoice.reset();
            return this;
        }

        Monophonic* setVoiceLevel(Voice eVoice, float32 fLevel) override {
            fVoiceLevel = fLevel;
            applyLevel();
            return this;
        }

        Monophonic* setOutputLevel(float32 fLevel) override {
            fOutputLevel = fLevel;
            applyLevel();
            return this;
        }

        Signal::Packet::ConstPtr emit(size_t uIndex) override {
            return oVoice.emit(uIndex);
        }
};

} // namespace

#endif
