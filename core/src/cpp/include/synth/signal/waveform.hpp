#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVEFORM_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVEFORM_HPP

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
class IWaveform /*: public Util::ISometimesShareable */ {
    public:
        virtual ~IWaveform() = default;

        //IWaveform* share() = 0;

        /**
         * Fixed waveform shapes. These use fixed mappings.
         */
        enum FixedShape {
            // Classic shapes
            SINE           = 0,
            TRIANGLE       = 1,
            SAW_DOWN       = 2,
            SAW_UP         = 3,
            SQUARE         = 4,
            PULSE_10       = 5, // PWM, 10% duty
            PULSE_20       = 6,
            PULSE_30       = 7,
            PULSE_40       = 8,
            PULSE_50       = SQUARE, // 50% duty is basically a square wave
            POKEY          = 9,
            NOISE          = 10,

            // Additional shapes for FM
//             SINE_HALF_RECT     = 6,
//             SINE_FULL_RECT     = 7,
//             SINE_SAW           = 8,
//             SINE_SAW_HARD      = 9,
//             SINE_PINCH         = 10,
//             SINE_CUT           = 11,
//             TRIANGLE_HALF_RECT = 12
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
        static Ptr get(FixedShape eShape);

        /**
         * Factory method to obtain a custom width PWM
         */
        static Ptr createPWM(float32 fWidth);
        static Ptr createPWM(IStream::Ptr pWidthModulator);
        static Ptr createXForm(Ptr pSource, float32 const* pMatrix);

        /**
         * Returns the period of this function, i.e. the numeric interval after which it's
         * output cycles.
         *
         * @return float32
         */
        virtual float32 getPeriod() const = 0;

        /**
         * Calculate a Packets worth of output values for a Packets worth of input values
         *
         * @param  Packet::ConstPtr oInput
         * @return Packet::Ptr
         *
         */
        virtual Packet::Ptr map(Packet::ConstPtr oInput) = 0;

        /**
         * Calculate the output for a single input.
         */
        virtual float32 value(float32 fInput) const = 0;

        /**
         * Get a copy (maybe). For stateless implementations, this is just the
         * a self reference. Stateful implementations should actually allocate
         * and populate a proper copy.
         */
        virtual Ptr copy();

};

}
#endif
