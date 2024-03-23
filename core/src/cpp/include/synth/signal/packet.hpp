#ifndef MC64K_SYNTH_SIGNAL_PACKET_HPP
    #define MC64K_SYNTH_SIGNAL_PACKET_HPP

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
        float32 afSamples[PACKET_SIZE] __attribute__ ((aligned (16)));

        /**
         * Shared pointer types for passing around.
         */
        typedef std::shared_ptr<Packet> Ptr;
        typedef std::shared_ptr<Packet const> ConstPtr;

        class Pool;

        /**
         * Obtain a new instance
         *
         * @return Ptr
         */
        static Ptr create();

        /**
         * Obtain a common reference to silence.
         *
         * @return ConstPtr
         */
        static ConstPtr getSilence();

        /**
         * Clone a packet and it's data
         *
         * @return Ptr
         */
        Ptr clone() const {
            Ptr pCopy = create();
            std::memcpy(pCopy->afSamples, afSamples, sizeof(afSamples));
            return pCopy;
        }

        /**
         * Clear the packet (special case of fill)
         *
         * @return Ptr
         */
        Packet* clear() {
            std::memset(afSamples, 0, sizeof(afSamples));
            return this;
        }

        /**
         * Get the next index for packet index aware operations
         *
         * @return size_t
         */
        static size_t getNextIndex() {
            return ++uNextIndex;
        }

        /**
         * Fill a packet with the given value.
         *
         * @param  float32 fValue
         * @return this
         */
        Packet* fillWith(float32 fValue);

        /**
         * Scale a packet by the given value.
         *
         * @param  float32 fValue
         * @return this
         */
        Packet* scaleBy(float32 fValue);

        /**
         * Bias a packet with the given value.
         *
         * @param  float32 fValue
         * @return this
         */
        Packet* biasBy(float32 fValue);

        /**
         * Scale and adjust a packet by the given values
         *
         * @param  float32 fScale
         * @param  float32 fBias
         * @return this
         */
        Packet* scaleAndBiasBy(float32 fScale, float32 fBias);

        /**
         * Fill a packet with the scaled and adjusted content of the input packet.
         *
         * @param  float32 fScale
         * @param  float32 fBias
         * @return this
         */

        Packet* scaleAndBiasBy(Packet const* poPacket, float32 fScale, float32 fBias);

        Packet* scaleAndBiasBy(ConstPtr const& poPacket, float32 fScale, float32 fBias) {
            return scaleAndBiasBy(poPacket.get(), fScale, fBias);
        }


        /**
         * Sum with the values of another packet
         *
         * @param  Packet const* poPacket
         * @return this
         */
        Packet* sumWith(Packet const* poPacket);

        /**
         * Sum with the values of another packet
         *
         * @param  ConstPtr poPacket
         * @return this
         */
        Packet* sumWith(ConstPtr const& poPacket) {
            return sumWith(poPacket.get());
        }

        /**
         * Multiply with the values of another packet
         *
         * @param  Packet const* poPacket
         * @return this
         */
        Packet* modulateWith(Packet const* poPacket);

        /**
         * Multiply with the values of another packet
         *
         * @param  ConstPtr poPacket
         * @return this
         */
        Packet* modulateWith(ConstPtr const& poPacket) {
            return modulateWith(poPacket.get());
        }

        /**
         * Accumulate with the scaled values of another packet
         *
         * @param  Packet const* poPacket
         * @param  float32 fScale
         * @return this
         */
        Packet* accumulate(Packet const* poPacket, float32 fScale);

        /**
         * Accumulate with the scaled values of another packet
         *
         * @param  ConstPtr poPacket
         * @param  float32 fScale
         * @return this
         */
        Packet* accumulate(ConstPtr const& poPacket, float32 fScale) {
            return accumulate(poPacket.get(), fScale);
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
        static void destroy(Packet* poPacket);

        /**
         * Helper class for Ptr, ensures that deletion routes back to destroy()
         * for proper handling.
         */
        class Deleter;
        friend class Pool;
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
