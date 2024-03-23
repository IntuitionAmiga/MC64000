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
#include <memory>

#include <synth/machine/voice.hpp>

using MC64K::Synth::Audio::Machine::Voice;

namespace MC64K::Synth::Audio::Control {

/**
 * Aliases for controller number and controller value.
 *
 * Currently 8-bit, but we may change that.
 */
typedef uint8 Value;
typedef uint8 Number;

/**
 * Absolute base class for controller
 */
class Definition {
    public:
        virtual ~Definition() = default;

        typedef std::shared_ptr<Definition> Ptr;
        typedef std::shared_ptr<Definition const> ConstPtr;

};

/**
 * Simple knob type control, mapping Value to some continuous range.
 *
 * Machine provides the end implementation that adds the update() behaviour.
 */
class Knob : public Definition {

    private:
        float32 fMinOutput;
        float32 fMaxOutput;
        Number  uControllerNumber;
        Value   uInitial;

    public:
        static constexpr float32 const SCALE_UINT8_FIXED_POINT_MAX = 255.0f/256.0f;

        Knob(
            Number  uControllerNumber,
            Value   uInitial,
            float32 fMinOutput = 0.0f,
            float32 fMaxOutput = 1.0f
        ): fMinOutput{fMinOutput},
           fMaxOutput{fMaxOutput},
           uControllerNumber{uControllerNumber},
           uInitial{uInitial} {}

        virtual void update(Voice eVoice, float32 fValue) = 0;
};

/**
 * Simple switch type control, treating value as an integer position.
 *
 * Machine provides the end implementation that adds the update() behaviour.
 */
class Switcher : public Definition {
    private:
        Number  uControllerNumber;
        Value   uInitial;

    public:
        Switcher(
            Number  uControllerNumber,
            Value   uInitial
        ): uControllerNumber{uControllerNumber},
           uInitial{uInitial} {}

        virtual void update(Voice eVoice, int32 iValue) = 0;

};

class IAutomatable {
    public:
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


};

}

#endif
