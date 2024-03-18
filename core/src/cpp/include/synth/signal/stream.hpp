#ifndef MC64K_SYNTH_SIGNAL_STREAM_HPP
    #define MC64K_SYNTH_SIGNAL_STREAM_HPP

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

/**
 * IStream interface
 *
 * An IStream implementation produces a sequential stream of Packet instances. This could be
 * a pure generator, e.g. an Oscillator or Envelope, or something that processes the result(s) of
 * one or more other IStream, such as a Filter or a Mixer. Packets are obtained from the emit()
 * method, which accepts an optional index value of the current packet being evaluated. This
 * allows any stream to detect when it has been asked for the same packet again, which may happen
 * when one stream is being used as a data source for several others, e.g. an LFO controlling
 * several other oscillators.
 *
 * Packets returned from emit() have const semantics. If you need to modify a packet from a
 * stream, you will need to clone it. The main motivation for this is to allow an efficient
 * implementation where packet recycling is easier to manage.
 *
 * A stream can be enabled or disabled. A disabled stream will still emit packets when asked
 * but the disabled packet will be silence (all values zero).
 *
 */
class IStream {
    public:
        virtual ~IStream() = default;

        /**
         * Check if a stream is enabled.
         *
         * @return bool
         */
        virtual bool isEnabled() = 0;

        /**
         * Get the current stream position
         *
         * @return size_t
         */
        virtual size_t getPosition() = 0;

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
         * The returned packet is read only.
         *
         * @param  size_t uIndex
         * @return Packet::ConstPtr
         */
        virtual Packet::ConstPtr emit(size_t uIndex = 0) = 0;

        typedef std::shared_ptr<IStream> Ptr;
        typedef std::shared_ptr<IStream const> ConstPtr;

};

/**
 * Common, optional implementation for stream components.
 *
 * Provides a ready implementation of position query, enabling and disabling. Uses a hook
 * to check if a stream can be enabled before enabling. For example, an Oscillator can check
 * that it has an appropriate Waveform before it can be enabled.
 *
 */
class TStreamCommon : public IStream {

    protected:
        Packet::Ptr poOutputPacket;
        size_t      uSamplePosition = 0;
        bool        bEnabled        = false;

        /**
         * Override. Returns true if the stream can be enabled.
         */
        virtual bool canEnable();

    public:
        /**
         * @inheritDoc
         */
        size_t getPosition() {
            return uSamplePosition;
        }

        /**
         * @inheritDoc
         */
        bool isEnabled() {
            return bEnabled;
        }

        /**
         * @inheritDoc
         *
         * Checks the response from canEnable before enabling. Does nothing if the stream
         * is already enabled.
         */
        IStream* enable();

        /**
         * @inheritDoc
         *
         * Disables the stream. Does nothing if the stream is already disabled.
         */
        IStream* disable();
};

}

#endif
