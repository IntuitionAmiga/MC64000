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

#include <cstdio>
#include <synth/machine/monophonic.hpp>
#include <synth/machine/polyphonic.hpp>

namespace MC64K::Synth::Audio::Machine {

Monophonic::Monophonic(): oVoice{IStream::NONE}, fOutputLevel{1.0f}, fVoiceLevel{1.0f} {
    std::fprintf(stderr, "Constructing monophonic machine at %p\n", this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Polyphonic::Polyphonic(Polyphony ePolyphony):
    oMixer{(uint32)ePolyphony, VOICE_ATTENUATE},
    fOutputLevel{1.f},
    ePolyphony{ePolyphony}
{
    std::fprintf(stderr, "Constructing polyphonic machine at %p\n", this);
}


}
