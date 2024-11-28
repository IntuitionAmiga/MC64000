#ifndef MC64K_SYNTH_SIGNAL_PACKET_RELAY_HPP
    #define MC64K_SYNTH_SIGNAL_PACKET_RELAY_HPP

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


#include <synth/signal.hpp>

namespace MC64K::Synth::Audio::Signal::Operator {

/**
 * PacketRelay
 *
 * Adapter that allows a non-stream immediate source of packets to be used with an IStream consumer.
 */
class PacketRelay : public TStreamCommon, public TOutputStream {

    public:
        PacketRelay();
        ~PacketRelay();

        /**
         * Set a packet to relay
         */
        PacketRelay* setPacket(Packet::Ptr const& roPacketPtr) {
            oOutputPacketPtr = roPacketPtr;
            return this;
        }

        PacketRelay* reset() noexcept override {
            uSamplePosition = 0;
            return this;
        }

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex) noexcept override;
};

}

#endif
