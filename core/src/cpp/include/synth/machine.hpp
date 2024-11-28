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
#include "signal.hpp"

#include "machine/voice.hpp"

using MC64K::Synth::Audio::Machine::Voice;
using MC64K::Synth::Audio::Machine::Polyphony;

namespace MC64K::Synth::Audio {

/**
 * IMachine
 *
 * Top level interface for Machines. A machine represents a fixed musical unit
 */
class IMachine : public virtual Signal::IStream {
    public:

        static constexpr float32 const VOICE_ATTENUATE = 1.0f / (float32)Polyphony::MAX_VOICES;

        // Getters

        /**
         * Get the number of voices the machine has.
         *
         * @return Polyphony
         */
        virtual Polyphony getNumVoices() const noexcept = 0;

        /**
         * Get the volume level for the enumerated voice. A value of 1.0 represents the
         * loudest intensity, though the value can be higher. Values can also be negative,
         * which results in the inversion of the output.
         *
         * @param  Voice eVoice
         * @return float32
         */
        virtual float32 getVoiceLevel(Voice eVoice) const noexcept = 0;

        /**
         * Get the volume level for the machine output. A value of 1.0 represents the
         * loudest intensity, though the value can be higher. Values can also be negative,
         * which results in the inversion of the output.
         *
         * @return float32
         */
        virtual float32 getOutputLevel() const noexcept = 0;

        // Mutators

        /**
         * Set the volume level for the machine output. A value of 1.0 represents the
         * loudest intensity, though the value can be higher. Values can also be negative,
         * which results in the inversion of the output.
         *
         * @param  float32 fLevel
         * @return IMachine* this
         */
        virtual IMachine* setOutputLevel(float32 fLevel) noexcept = 0;

        /**
         * Set the volume level for the enumerated voice. A value of 1.0 represents the
         * loudest intensity, though the value can be higher. Values can also be negative,
         * which results in the inversion of the output.
         *
         * @param  Voice     eVoice
         * @param  float32   fLevel
         * @return IMachine* this
         */
        virtual IMachine* setVoiceLevel(Voice eVoice, float32 fLevel) noexcept = 0;

        /**
         * Set the note number for the enumerated voice. This should be in the range
         * 0-127 as per the MIDI specification.
         *
         * @param  Voice     eVoice
         * @param  uint32    uNote
         * @return IMachine* this
         */
        virtual IMachine* setVoiceNote(Voice eVoice, uint32 uNote) noexcept = 0;

        /**
         * Set the velocity level for the enumerated voice. Notionally this is in the range
         * 0-127 as per the MIDI standard, but accepts a floating point value. The expectation
         * is that the value will be mapped via one or more Control Curve implementation to
         * velocity sensitive attributes of the machine's sound generator.
         *
         * @param  Voice     eVoice
         * @param  float32   fVelocity
         * @return IMachine* this
         */
        virtual IMachine* setVoiceVelocity(Voice eVoice, float32 fVelocity) noexcept = 0;

        /**
         * Set the enumerated voice playing.
         *
         * @param  Voice     eVoice
         * @return IMachine* this
         */
        virtual IMachine* startVoice(Voice eVoice) noexcept = 0;

        /**
         * Stops the enumerated voice. The stop can be either hard, or soft:
         *
         * Hard: Immediately ceases the voice.
         * Soft: Signals that the voice should stop gracefully, following any release envelope
         *       etc.
         *
         * @param  Voice     eVoice
         * @param  bool      bSoft
         * @return IMachine* this
         */
        virtual IMachine* stopVoice(Voice eVoice, bool bSoft) noexcept = 0;

};

class TSimpleVelocity : public virtual IMachine {

    public:
        static constexpr float32 const MIDI_VEL_SCALE  = 1.0f/127.0f;

        virtual IMachine* setVoiceVelocity(Voice eVoice, float32 fVelocity) noexcept override {
            setVoiceLevel(eVoice, fVelocity * MIDI_VEL_SCALE);
            return this;
        };

};

}
#endif
