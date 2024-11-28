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
#include <cstdio>
#include <synth/signal.hpp>

//#define DEBUG_PACKET_USED_AFTER_FREE

#ifdef DEBUG_PACKET_USED_AFTER_FREE
#include <synth/signal/waveform.hpp>
#endif

namespace MC64K::Synth::Audio::Signal {

size_t Packet::uNextIndex        = 0;
uint64 Packet::uPacketsCreated   = 0;
uint64 Packet::uPacketsDestroyed = 0;
uint64 Packet::uPeakPacketsInUse = 0;

Packet* Packet::fillWith(float32 fValue) noexcept {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] = fValue;
    }
    return this;
}

Packet* Packet::scaleBy(float32 fValue) noexcept {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] *= fValue;
    }
    return this;
}

Packet* Packet::scaleBy(Packet const* poPacket, float32 fValue) noexcept {
    if (poPacket) {
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] = poPacket->afSamples[u] * fValue;
        }
    } else {
        clear();
    }
    return this;
}


Packet* Packet::biasBy(float32 fValue) noexcept {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] += fValue;
    }
    return this;
}

Packet* Packet::scaleAndBiasBy(float32 fScale, float32 fBias) noexcept {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] = afSamples[u] * fScale + fBias;
    }
    return this;
}

Packet* Packet::scaleAndBiasBy(Packet const* poPacket, float32 fScale, float32 fBias) noexcept {
    if (poPacket) {
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] = poPacket->afSamples[u] * fScale + fBias;
        }
    } else {
        fillWith(fBias);
    }
    return this;
}

Packet* Packet::sumWith(Packet const* poPacket) noexcept {
    if (poPacket) {
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] += poPacket->afSamples[u];
        }
    }
    return this;
}

Packet* Packet::sum(Packet const* poPacketA, Packet const* poPacketB) noexcept {
    if (poPacketA) {
        if (poPacketB) {
            for (unsigned u = 0; u < PACKET_SIZE; ++u) {
                afSamples[u] = poPacketA->afSamples[u] + poPacketB->afSamples[u];
            }
        } else {
            std::memcpy(afSamples, poPacketA->afSamples, sizeof(afSamples));
        }
    } else {
        if (poPacketB) {
            std::memcpy(afSamples, poPacketB->afSamples, sizeof(afSamples));
        } else {
            clear();
        }
    }
    return this;
}


Packet* Packet::modulateWith(Packet const* poPacket) noexcept {
    if (poPacket) {
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] *= poPacket->afSamples[u];
        }
    }
    return this;
}

Packet* Packet::accumulate(Packet const* poPacket, float32 fValue) noexcept {
    if (poPacket) {
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] += poPacket->afSamples[u] * fValue;
        }
    }
    return this;
}

/**
 * Deleter hook for shared_ptr.
 *
 * Delegates packet destruction back to the Packet class as we intend to
 * recycle them.
 *
 */
class Packet::Deleter {
    public:
        void operator()(Packet* poPacket) const noexcept {
            std::fprintf(stderr, "Deleting unpooled Packet at %p\n", poPacket);
            delete poPacket;
        }
};

/**
 * Custom deleter for the shared pointer that maintains a most recent N freed packets for
 * immediate recycling
 */
class Packet::RecyclePool {
    public:
        static Packet* apPool[64];
        static uint64  uFreedMask;

        void operator()(Packet* poPacket) const noexcept {
            Packet::destroy(poPacket);
        }

        static Packet* allocate() {
            if (!uFreedMask) {
                // If the freed mask is zero, we just have to return a new instance.
                return new Packet();
            }

            // Otherwise get one of the recently freed.
            int iFreed = __builtin_ffsll(uFreedMask) - 1;

            Packet* poPacket = apPool[iFreed];
            apPool[iFreed] = nullptr;
            uFreedMask     &= ~(1 << iFreed);
            //std::fprintf(stderr, "Using recycled Packet at %p [pool index %d]\n", poPacket, iFreed);
            return poPacket;

        }

        static void deallocate(Packet* poPacket) noexcept {

            // Invert the mask when looking for where to put the Packet for recycling
            uint64 uInvMask = ~uFreedMask;

            if (uInvMask) {
                int iRecycle = __builtin_ffsll(uInvMask) - 1;
                apPool[iRecycle] = poPacket;

#ifdef DEBUG_PACKET_USED_AFTER_FREE
                IWaveform::get(IWaveform::NOISE)->map(poPacket, poPacket);
#endif
                uFreedMask |= 1 << iRecycle;
            } else {
                std::fprintf(stderr, "Can't recycle Packet at %p, pool is full\n", poPacket);
                // The freed pool was already full, so we can't recycle the deleted packet
                delete poPacket;
            }
        }

        class Cleanup {
            public:
                ~Cleanup() noexcept {
                    for (int i = 0; i < 64; ++i) {
                        if (RecyclePool::apPool[i]) {
                            std::fprintf(stderr, "Deleting pool Packet at %p [pool index %d]\n", apPool[i], i);
                            delete apPool[i];
                        }
                    }
                }
        };

};

Packet* Packet::RecyclePool::apPool[64] = { nullptr };
uint64  Packet::RecyclePool::uFreedMask = 0;

// Allow global startup and shutdown to alloc/free the pool
static Packet::RecyclePool::Cleanup oRecyclePoolCleaner;

Packet::Ptr Packet::create() {
    Packet* poPacket = RecyclePool::allocate();

    // Tracking
    uint64 uPacketsInUse = ++uPacketsCreated - uPacketsDestroyed;
    if (uPacketsInUse > uPeakPacketsInUse) {
        uPeakPacketsInUse = uPacketsInUse;
    }
    return Ptr(poPacket, RecyclePool());
}

Packet::Ptr Packet::createUnpooled() {
    return Ptr(new Packet, Deleter());
}

/**
 * Free a Packet instance
 */
void Packet::destroy(Packet* poPacket) noexcept {
    if (poPacket) {
        ++uPacketsDestroyed;
        RecyclePool::deallocate(poPacket);
    }
}

/**
 * @inheritDoc
 */
Packet::ConstPtr Packet::getSilence() noexcept {
    static Packet::Ptr pSilence;
    if (!pSilence.get()) {
        pSilence = Packet::create();
        pSilence->clear();
    }
    return pSilence;
}


/**
 * Report statistics
 */
void Packet::dumpStats() {
    std::printf(
        "Packet statistics:\n"
        "\tCreated     : %lu\n"
        "\tDestroyed   : %lu\n"
        "\tPeak In Use : %lu\n",
        uPacketsCreated,
        uPacketsDestroyed,
        uPeakPacketsInUse
    );
}

bool TPacketIndexAware::useLast(size_t uIndex) noexcept {
    if (!uIndex) {
        uLastIndex = Packet::getNextIndex();
        return false;
    } else if (uLastIndex != uIndex) {
        uLastIndex = uIndex;
        return false;
    }
    return true;
}

}
