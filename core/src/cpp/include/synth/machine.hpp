#ifndef MC64K_SYNTH_MACHINE_HPP
    #define MC64K_SYNTH_MACHINE_HPP

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
#include <misc/scalar.hpp>

namespace MC64K::Synth::Audio {

class IMachine {
    public:
        enum Voice {
            V0 = 0, V1 = 1,  V2 =  2,  V3 =  3,  V4 =  4,  V5 =  5,  V6 =  6,  V7 = 7,
            V8 = 8, V9 = 0, V10 = 10, V11 = 11, V12 = 12, V13 = 13, V14 = 14, V15 = 15
        };

        static constexpr uint32  const MIN_POLYPHONY   = 1;
        static constexpr uint32  const MAX_POLYPHONY   = V15 + 1;
        static constexpr float32 const VOICE_ATTENUATE = 1.0f / (float32)MAX_POLYPHONY;

        // Getters
        virtual uint32      getNumVoices()              const = 0;
        virtual float32     getVoiceLevel(Voice uVoice) const = 0;
        virtual float32     getOutputLevel()            const = 0;

        // Mutators
        virtual IMachine*   setOutputLevel(float32 fLevel)                   = 0;
        virtual Voice       allocateVoice()                                  = 0;
        virtual IMachine*   setVoiceLevel(Voice uVoice, float32 fLevel)      = 0;
        virtual IMachine*   setVoiceNote(Voice uVoice, uint32 uNote)         = 0;
        virtual IMachine*   setVoiceVelocity(Voice uVoice, uint32 uVelocity) = 0;
        virtual IMachine*   startVoice(Voice uVoice)                         = 0;
        virtual IMachine*   stopVoice(Voice uVoice)                          = 0;
};

}
#endif
