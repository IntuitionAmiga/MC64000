#ifndef MC64K_SYNTH_SIGNAL_WAVEFORM_HPP
    #define MC64K_SYNTH_SIGNAL_WAVEFORM_HPP

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

#include "packet.hpp"

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * IWaveform interface
 *
 * Waveform types define their period and a function for mapping input time values (expressed in
 * periods) into output values in the range -1.0 to 1.0
 */
class IWaveform {
    public:
        virtual ~IWaveform() = default;

        /**
         * Fixed waveform shapes. These use fixed mappings.
         */
        enum FixedShape {
            // Classic shapes
            SINE               = 0,
            TRIANGLE           = 1,
            SAW_DOWN           = 2,
            SAW_UP             = 3,
            SQUARE             = 4,
            PULSE              = 5, // PWM (any)
            PULSE_10           = 6, // Presets for PWM
            PULSE_20           = 7,
            PULSE_30           = 8,
            PULSE_40           = 9,
            PULSE_50           = 10, // 50% duty is basically a square wave
            POKEY              = 11,
            NOISE              = 12,

            // Additional shapes for FM
            SINE_HALF_RECT     = 13,
            SINE_FULL_RECT     = 14,
            SINE_SAW           = 15,
            SINE_SAW_HARD      = 16, // todo
            SINE_PINCH         = 17,
            SINE_CUT           = 18, // todo
            TX81Z_1            = 19, // sine
            TX81Z_2            = 20, // sine pinch
            TX81Z_3            = 21, // sine half rect
            TX81Z_4            = 22,
            TX81Z_5            = 23,
            TX81Z_6            = 24,
            TX81Z_7            = 25,
            TX81Z_8            = 26,

            TRIANGLE_HALF_RECT = 27,
            XFORM              = 28,
            CUSTOM             = 29
        };

        /**
         * Using shared_ptr based references allows some degree of fire and forget
         * in the implementation of consumes. For fixed waveform shapes, the pointer
         * returned, while still reference counted does not have deletion semantics
         * since it's a reference to a single instance.
         *
         * Pointers returned from create methods return a deletable instance.
         */
        typedef std::shared_ptr<IWaveform> Ptr;
        typedef std::shared_ptr<IWaveform const> ConstPtr;

        /**
         * Factory method to obtain common shapes
         */
        static Ptr get(FixedShape eShape) noexcept;

        /**
         * Factory method to obtain a custom width PWM
         */
        static Ptr createPWM(float32 fWidth);

        /**
         * Returns the period of this function, i.e. the numeric interval after which it's
         * output cycles.
         *
         * @return float32
         */
        virtual float32 getPeriod() const noexcept = 0;

        /**
         * Calculate a Packets worth of output values for a Packets worth of input values
         *
         * @param  Packet const* poInput
         */
        virtual void map(Packet const* poInput, Packet* poOutput) noexcept = 0;

        /**
         * Calculate a Packets worth of output values for a Packets worth of input values
         *
         * @param  Packet::ConstPtr poInput
         */
        void map(Packet::ConstPtr const& roInput, Packet::Ptr& roOutput) noexcept {
            map(roInput.get(),roOutput.get());
        }

        /**
         * Calculate the output for a single input.
         */
        virtual float32 value(float32 fInput) const noexcept = 0;

        /**
         * Returns the enumerated shape identifier for the waveform.
         */
        virtual FixedShape getShape() const noexcept = 0;

        /**
         * Returns whether or not the wave contains sharp discontinuities.
         */
        virtual bool isDiscontinuous() const noexcept = 0;

        /**
         * Returns whether or not the waveform is aperiodic, i.e. does not
         * depend on input time, e.g. noise sources.
         */
        virtual bool isAperiodic() const noexcept = 0;

        /**
         * Get a copy (maybe). For stateless implementations, this is just the
         * a self reference. Stateful implementations should actually allocate
         * and populate a proper copy.
         */
        virtual Ptr copy();

};

}
#endif
