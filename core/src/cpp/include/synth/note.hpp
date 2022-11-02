#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_NOTE_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_NOTE_HPP

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

#include <misc/scalar.hpp>

namespace MC64K::Synth::Audio {

class Note {
    public:
        static constexpr float32 const CENTRE_FREQUENCY = 440.0f;
        static constexpr int32   const CENTRE_REFERENCE = 69; // LMAO: A4
        static constexpr int32   const SEMIS_PER_OCTAVE = 12;
        static constexpr float32 const FACTOR_PER_SEMI  = 1.0f / (float32)SEMIS_PER_OCTAVE;

        static float32 getFrequency(
            int32   iNumber,
            float32 fBendSemis       = 0.0f,
            float32 fScalePerOctave  = 1.0f,
            float32 fCentreValue     = CENTRE_FREQUENCY
        );

        /**
         * Converts fractional semitones to frequency multiplier
         */

#ifdef AUDIO_PITCH_APPROX
        static float32 semisToMultiplier(float32 fSemitones);
#else
        static inline float32 semisToMultiplier(float32 fSemitones) {
            return std::exp2(fSemitones * FACTOR_PER_SEMI);
        }
#endif
};

}
#endif
