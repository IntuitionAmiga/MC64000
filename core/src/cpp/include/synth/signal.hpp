#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_HPP

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

#include <memory>
#include <misc/scalar.hpp>
#include <host/audio/config.hpp>

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;

class Packet;

///////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * Packet class.
 *
 * Represents the smallest processable unit of audio.
 */
class Packet {
    public:
        float32 aSamples[PACKET_SIZE] __attribute__ ((aligned (16)));

        /**
         * Shared pointer types for passing around.
         */
        typedef std::shared_ptr<Packet> Ptr;
        typedef std::shared_ptr<Packet const> ConstPtr;

        /**
         * Obtain a new instance
         *
         * @return PacketPtr
         */
        static Ptr create();

        static void dumpStats();

    private:
        /**
         * Allocator stats
         */
        static uint64 uPacketsCreated;
        static uint64 uPacketsDestroyed;
        static uint64 uPeakPacketsInUse;
        /**
         * Forbid explicit creation and deletion
         */
        Packet() {}
        ~Packet() {}

        /**
         * Destroy an instance.
         *
         * todo: Implement a fast recycle path with a pool of Packets
         */
        static void destroy(Packet* pPacket);

        /**
         * Helper class for Ptr, ensures that deletion routes back to destroy()
         * for proper handling.
         */
        class Deleter;
};



///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * IStream interface
 *
 * Main interface for sequential packet producers.
 */
class IStream {
    public:
        virtual ~IStream() = default;
        /**
         * Enable a stream.
         *
         * @return this
         */
        virtual IStream* enable() = 0;

        /**
         * Disable a stream. A disabled stream will emit silence packets if invoked.
         *
         * @return this
         */
        virtual IStream* disable() = 0;

        /**
         * Check if a stream is enabled.
         *
         * @return bool
         */
        virtual bool isEnabled() = 0;

        /**
         * Get the current stream position
         *
         * @return int32
         */
        virtual int32 getPosition() = 0;

        /**
         * Reset the stream
         *
         * @return IStream*
         */
        virtual IStream* reset() = 0;

        /**
         * Emit a Packet. An optional index parameter allows the stream to ascertain if it is being asked repeatedly for
         * the last generated Packet of data and if so, return it. This becomes necessary in complex signal routing where
         * one IStream implementation's output is consumed by multiple inputs.
         *
         * @param  int32 iIndex
         * @return Packet
         */
        virtual Packet* emit(int32 iIndex = 0) = 0;

};

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
         * Shared pointer types for passing around
         */
        typedef std::shared_ptr<IWaveform> Ptr;
        typedef std::shared_ptr<IWaveform const> ConstPtr;

        /**
         * Factory method to obtain
         */
        static Ptr get(FixedShape eShape);

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

};


///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Interface for oscillator types.
 */
class IOscillator : public IStream {
    public:
        /**
         * Set the waveform to use. Passing null disables the oscillator (emits silence).
         *
         * Implementations may clone the waveform instance passed to them so do not rely on getWaveform() returning
         * the same instance.
         *
         * @param  IWaveform|null oWaveform
         * @return this
         */
        virtual IOscillator* setWaveform(IWaveform* oWaveform) = 0;

        virtual IWaveform* getWaveform() = 0;

        /**
         * Set the baseline frequency to emit.
         *
         * @param  float fFrequency
         * @return this
         */
        virtual IOscillator* setFrequency(float32 fFrequency) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////


class IEnvelope : public IStream /*, Util::ISometimesShareable */ {

    public:
        static constexpr float32 const MIN_TIME_SCALE = 0.01f;

        /**
         * Covariant implementation of ISometimesShareable
         */
        virtual IEnvelope* share() = 0;

        /**
         * Set a scaling factor for envelope timing. A smaller value results in a faster envelope. Use to simlulate the
         * effects of higher notes decaying faster, etc. This should be set whenever we start a new note.
         *
         * @param  float32 fTimeScale
         * @return this
         */
        virtual IEnvelope* setTimeScale(float32 fTimeScale) = 0;

        /**
         * Set a scaling factor for envelope levels. A smaller value results in a quieter envelope. Use to simlulate  the
         * effects of higher notes having lower overall energy, or higher velocities having greater, etc. This should be
         * set whenever we start a new note.
         *
         * @param  float32 fLevelScale
         * @return this
         */
        virtual IEnvelope* setLevelScale(float32 fLevelScale) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////


class IFilter : public IStream {

    public:
        // Cutoff range is normalised
        static constexpr float32 const MIN_CUTOFF    = 0.001f;
        static constexpr float32 const DEF_CUTOFF    = 0.5f;
        static constexpr float32 const MAX_CUTOFF    = 1.0f;

        // Resonance range is normalised
        static constexpr float32 const MIN_RESONANCE = 0.0f;
        static constexpr float32 const DEF_RESONANCE = 0.0f;
        static constexpr float32 const MAX_RESONANCE = 1.0f;

        /**
         * Set the baseline cutoff level. In the absence of a cutoff controller, this is the fixed cutoff. Otherwise it is
         * the cutoff value when the control signal level is 1.0. Values sould be in the range MIN_CUTOFF to MAX_CUTOFF.
         * Note that values above MAX_CUTOFF may be tolerated depending on the filter type.
         *
         * @param  float32 fCutoff
         * @return this
         */
        virtual IFilter* setCutoff(float32 fCutoff) = 0;

        /**
         * Set a control stream (envelope, LFO etc) for the cutoff control. Setting null clears any existing control.
         *
         * @param  IStream|null oCutoffControl
         * @return this
         */
        virtual IFilter* setCutoffControl(IStream* oCutoffControl) = 0;

        /**
         * Set the baseline resonance level. In the absence of a resonance controller, this is the fixed resonance.
         * Otherwise it is the resonance value when the control signal level is 1.0. Values should be in the range
         * MIN_RESONANCE to MAX_RESONANCE. Note that values above MAX_RESONANCe may be tolerated depending on the filter
         * type.
         *
         * @param  float fResonance
         * @return this
         */
        virtual IFilter* setResonance(float32 fResonance) = 0;

        /**
         * Set a control stream (envelope, LFO etc) for the resonance control. Setting null clears any existing control.
         *
         * @param  IStream|null oResonanceControl
         * @return this
         */
        virtual IFilter* setResonanceControl(IStream* oResonanceControl) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * IInsert
 *
 * Interface for stream insertion classes. These consume a stream and apply some form of insertion effect.
 */
class IInsert : public IStream {
    public:
        /**
         * Obtain the InputStream for this Insert. Returns null if not set yet.
         *
         * @return IStream|null
         */
        virtual IStream* getInputStream() const = 0;

        /**
         * Set the InputStream for this Insert. Until an input stream is set, the insert is disable()d.
         *
         * @param  IStream|null $oInputStream
         * @return this
         */
        virtual IInsert* setInputStream(IStream* oInputStream) = 0;

        /**
         * Get the dry signal level for this Insert.
         *
         * @return float32
         */
        virtual float32 getDryLevel() const = 0;

        /**
         * Set the dry signal level for this Insert. Should be in the range 0.0 ... 1.0 but this is not a strict requirement.
         * Values over 1 serve as amplification, values lower than one attenuation. Negative values invert the sign.
         *
         * @param  float32 fDryLevel
         * @return this
         */
        virtual IInsert* setDryLevel(float32 fDryLevel);
};

}
#endif
