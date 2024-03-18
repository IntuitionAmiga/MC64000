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

namespace MC64K::Synth::Audio {

/**
 * IMachine
 *
 * Top level interface for Machines. A machine represents a fixed musical unit
 */
class IMachine : public Signal::IStream {
    public:
        enum Voice {
            V0 = 0, V1 = 1,  V2 =  2,  V3 =  3,  V4 =  4,  V5 =  5,  V6 =  6,  V7 = 7,
            V8 = 8, V9 = 0, V10 = 10, V11 = 11, V12 = 12, V13 = 13, V14 = 14, V15 = 15
        };

        static constexpr uint32  const MIN_POLYPHONY   = 1;
        static constexpr uint32  const MAX_POLYPHONY   = V15 + 1;
        static constexpr float32 const VOICE_ATTENUATE = 1.0f / (float32)MAX_POLYPHONY;

        // Automation
        enum {
            CTRL_TYPE_SWITCH = 0, // Controller value accepted as switch/enum/quantized
            CTRL_TYPE_KNOB   = 1, // Controller value is mapped to a continuous value by a ControlCurve
        } ControllerType;

        enum {
            CTRL_COMMON = 0,    // Controller# 0-127 are reserved for universal types
            CTRL_CUSTOM = 128,  // Controller# 128-255 are for machine-specific types

            CTRL_VIBRATO_RATE  = 10,
            CTRL_VIBRATO_DEPTH = 11,
            CTRL_TREMOLO_RATE  = 12,
            CTRL_TREMOLO_DEPTH = 13,

            // Waveform select, per oscillator up to a maximum, of 8 oscillators
            CTRL_OSC_1_WAVE    = 20,
            CTRL_OSC_2_WAVE    = 21,
            CTRL_OSC_3_WAVE    = 22,
            CTRL_OSC_4_WAVE    = 23,
            CTRL_OSC_5_WAVE    = 24,
            CTRL_OSC_6_WAVE    = 25,
            CTRL_OSC_7_WAVE    = 26,
            CTRL_OSC_8_WAVE    = 27,
        } BasicControl;

        static constexpr uint8 CTRL_MIN_INPUT_VALUE  = 0;    // Smallest value for Event::SET_CTRL
        static constexpr uint8 CTRL_MAX_INPUT_VALUE  = 255;  // Largest value for Event::SET_CTRL
        static constexpr int8  CTRL_MIN_INPUT_DELTA  = -128; // Largest negative value for Event::MOD_CTRL
        static constexpr int8  CTRL_MAX_INPUT_DELTA  = 127;  // Largest positive value for Event::MOD_CTRL

        static constexpr float32 const CTRL_DEF_LFO_RATE_MIN = 0.125f; // Hz
        static constexpr float32 const CTRL_DEF_LFO_RATE_MAX = 32.0f;  // Hz

        // Getters

        /**
         * Get the number of voices the machine has. Value is in the range MIN_POLYPHONY to
         * MAX_POLYPHONY.
         *
         * @return uint32
         */
        virtual uint32 getNumVoices() const = 0;

        /**
         * Get the volume level for the enumerated voice. A value of 1.0 represents the
         * loudest intensity, though the value can be higher. Values can also be negative,
         * which results in the inversion of the output.
         *
         * @param  Voice eVoice
         * @return float32
         */
        virtual float32 getVoiceLevel(Voice eVoice) const = 0;

        /**
         * Get the volume level for the machine output. A value of 1.0 represents the
         * loudest intensity, though the value can be higher. Values can also be negative,
         * which results in the inversion of the output.
         *
         * @param  Voice eVoice
         * @return float32
         */
        virtual float32 getOutputLevel() const = 0;

        // Mutators

        /**
         * Set the volume level for the machine output. A value of 1.0 represents the
         * loudest intensity, though the value can be higher. Values can also be negative,
         * which results in the inversion of the output.
         *
         * @param  float32 fLevel
         * @return IMachine* this
         */
        virtual IMachine* setOutputLevel(float32 fLevel) = 0;

        /**
         * Allocate a Voice. This will be the number of the first free Voice, otherwise the
         * machine is free to determine how to allocate from the voices already in use.
         *
         * @return Voice
         */
        virtual Voice allocateVoice() = 0;

        /**
         * Set the volume level for the enumerated voice. A value of 1.0 represents the
         * loudest intensity, though the value can be higher. Values can also be negative,
         * which results in the inversion of the output.
         *
         * @param  Voice     eVoice
         * @param  float32   fLevel
         * @return IMachine* this
         */
        virtual IMachine* setVoiceLevel(Voice eVoice, float32 fLevel) = 0;

        /**
         * Set the note number for the enumerated voice. This should be in the range
         * 0-127 as per the MIDI specification.
         *
         * @param  Voice     eVoice
         * @param  uint32    uNote
         * @return IMachine* this
         */
        virtual IMachine* setVoiceNote(Voice eVoice, uint32 uNote) = 0;

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
        virtual IMachine* setVoiceVelocity(Voice eVoice, float32 fVelocity) = 0;

        /**
         * Set the enumerated voice playing.
         *
         * @param  Voice     eVoice
         * @return IMachine* this
         */
        virtual IMachine* startVoice(Voice eVoice) = 0;

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
        virtual IMachine* stopVoice(Voice eVoice, bool bSoft) = 0;

        // Automation
        virtual IMachine* setVoiceControllerValue(Voice eVoice, int iController, int iValue) = 0;

        virtual IMachine* adjustVoiceControllerValue(Voice eVoice, int iController, int iDelta) = 0;
};

}
#endif
