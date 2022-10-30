#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_STREAM_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_STREAM_HPP

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

}

#endif
