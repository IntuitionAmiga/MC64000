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

class Monophonic : public IMachine {

    protected:
        //Signal::Operator::LevelAdjust oVoice;

    public:
        Polyphony getNumVoices() const override {
            return ONE_VOICE;
        };
};

} // namespace

#endif
