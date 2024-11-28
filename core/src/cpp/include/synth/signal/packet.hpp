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

        class RecyclePool;

        /**
         * Obtain a new instance
         *
         * @return Ptr
         */
        static Ptr create();

        static Ptr createUnpooled();

        /**
         * Obtain a common reference to silence.
         *
         * @return ConstPtr
         */
        static ConstPtr getSilence() noexcept;

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
        Packet* clear() noexcept {
            std::memset(afSamples, 0, sizeof(afSamples));
            return this;
        }

        /**
         * Get the next index for packet index aware operations
         *
         * @return size_t
         */
        static size_t getNextIndex() noexcept {
            return ++uNextIndex;
        }

        static size_t getCurrentIndex() noexcept {
            return uNextIndex;
        }

        /**
         * Fill a packet with the given value.
         *
         * @param  float32 fValue
         * @return this
         */
        Packet* fillWith(float32 fValue) noexcept;

        /**
         * Scale a packet by the given value.
         *
         * @param  float32 fValue
         * @return this
         */
        Packet* scaleBy(float32 fValue) noexcept;

        /**
         * Fill the packet with the scaled values from another.
         */
        Packet* scaleBy(Packet const* poPacket, float32 fValue) noexcept;

        Packet* scaleBy(ConstPtr const& roPacket, float32 fScale, float32 fBias) noexcept {
            return scaleBy(roPacket.get(), fScale);
        }


        /**
         * Bias a packet with the given value.
         *
         * @param  float32 fValue
         * @return this
         */
        Packet* biasBy(float32 fValue) noexcept;

        /**
         * Scale and adjust a packet by the given values
         *
         * @param  float32 fScale
         * @param  float32 fBias
         * @return this
         */
        Packet* scaleAndBiasBy(float32 fScale, float32 fBias) noexcept;

        /**
         * Fill a packet with the scaled and adjusted content of the input packet.
         *
         * @param  float32 fScale
         * @param  float32 fBias
         * @return this
         */

        Packet* scaleAndBiasBy(Packet const* poPacket, float32 fScale, float32 fBias) noexcept;

        Packet* scaleAndBiasBy(ConstPtr const& roPacket, float32 fScale, float32 fBias) noexcept {
            return scaleAndBiasBy(roPacket.get(), fScale, fBias);
        }


        /**
         * Sum with the values of another packet
         *
         * @param  Packet const* poPacket
         * @return this
         */
        Packet* sumWith(Packet const* poPacket) noexcept;

        /**
         * Sum with the values of another packet
         *
         * @param  ConstPtr poPacket
         * @return this
         */
        Packet* sumWith(ConstPtr const& poPacket) noexcept {
            return sumWith(poPacket.get());
        }

        /**
         * Populate with the sum of two other packets
         */
        Packet* sum(Packet const* poPacketA, Packet const* poPacketB) noexcept;

        Packet* sum(ConstPtr const& roPacketA, ConstPtr const& roPacketB) noexcept {
            return sum(roPacketA.get(), roPacketB.get());
        }


        /**
         * Multiply with the values of another packet
         *
         * @param  Packet const* poPacket
         * @return this
         */
        Packet* modulateWith(Packet const* poPacket) noexcept;

        /**
         * Multiply with the values of another packet
         *
         * @param  ConstPtr poPacket
         * @return this
         */
        Packet* modulateWith(ConstPtr const& poPacket) noexcept {
            return modulateWith(poPacket.get());
        }

        /**
         * Accumulate with the scaled values of another packet
         *
         * @param  Packet const* poPacket
         * @param  float32 fScale
         * @return this
         */
        Packet* accumulate(Packet const* poPacket, float32 fScale) noexcept;

        /**
         * Accumulate with the scaled values of another packet
         *
         * @param  ConstPtr poPacket
         * @param  float32 fScale
         * @return this
         */
        Packet* accumulate(ConstPtr const& poPacket, float32 fScale) noexcept {
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
        Packet() noexcept {
            //std::fprintf(stderr, "NP: %p\n", this);
        }
        ~Packet() noexcept {
            //std::fprintf(stderr, "DP: %p\n", this);
        }

        /**
         * Destroy an instance
         */
        static void destroy(Packet* poPacket) noexcept;

        /**
         * Helper class for Ptr, ensures that deletion routes back to destroy()
         * for proper handling.
         */
        class Deleter;
        friend class RecyclePool;
};

/**
 * TPacketIndexAware
 *
 * Utility trait for Packet generators to determine whether or not they need to calculate a new Packet or not.
 */
class TPacketIndexAware {
    protected:
        size_t uLastIndex;
        bool useLast(size_t uIndex) noexcept;
        TPacketIndexAware() : uLastIndex(0) {}
};

}

#endif
