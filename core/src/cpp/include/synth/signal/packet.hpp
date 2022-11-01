#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_PACKET_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_PACKET_HPP

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

#include <cstring>
#include <memory>
#include <misc/scalar.hpp>
#include <host/audio/config.hpp>

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;
/**
 * Packet class.
 *
 * Represents the smallest processable unit of audio.
 */

class Packet; // Forwards reference

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
         * @return Ptr
         */
        static Ptr create();

        static Ptr getSilence();

        Ptr clone() const {
            Ptr pCopy = create();
            std::memcpy(pCopy->aSamples, aSamples, sizeof(aSamples));
            return pCopy;
        }

        /**
         * Get the next index for packet index aware operations
         */
        static size_t getNextIndex() {
            return ++uNextIndex;
        }

        Packet* fillWith(float32 fValue);

        Packet* scaleBy(float32 fValue);

        Packet* biasBy(float32 fValue);

        Packet* scaleAndBiasBy(float32 fScale, float32 fBias);

        Packet* sumWith(Packet const* pPacket);

//         Packet* sumWith(Ptr pPacket) {
//             return sumWith(pPacket.get());
//         }

        Packet* sumWith(ConstPtr pPacket) {
            return sumWith(pPacket.get());
        }

        Packet* modulateWith(Packet const* pPacket);

//         Packet* modulateWith(Ptr pPacket) {
//             return modulateWith(pPacket.get());
//         }

        Packet* modulateWith(ConstPtr pPacket) {
            return modulateWith(pPacket.get());
        }

        Packet* accumulate(Packet const* pPacket, float32 fValue);

        Packet* accumulate(ConstPtr pPacket, float32 fValue) {
            return accumulate(pPacket.get(), fValue);
        }

        static void dumpStats();

    private:
        static size_t uNextIndex;

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

/**
 * TPacketIndexAware
 *
 * Utility trait for Packet generators to determine whether or not they need to calculate a new Packet or not.
 */
class TPacketIndexAware {
    protected:
        size_t uLastIndex;
        bool useLast(size_t uIndex);
        TPacketIndexAware() : uLastIndex(0) {}
};

}

#endif
