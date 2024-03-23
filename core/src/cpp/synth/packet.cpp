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

namespace MC64K::Synth::Audio::Signal {

size_t Packet::uNextIndex        = 0;
uint64 Packet::uPacketsCreated   = 0;
uint64 Packet::uPacketsDestroyed = 0;
uint64 Packet::uPeakPacketsInUse = 0;

Packet* Packet::fillWith(float32 fValue) {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] = fValue;
    }
    return this;
}

Packet* Packet::scaleBy(float32 fValue) {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] *= fValue;
    }
    return this;
}

Packet* Packet::biasBy(float32 fValue) {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] += fValue;
    }
    return this;
}

Packet* Packet::scaleAndBiasBy(float32 fScale, float32 fBias) {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] = afSamples[u] * fScale + fBias;
    }
    return this;
}

Packet* Packet::scaleAndBiasBy(Packet const* poPacket, float32 fScale, float32 fBias) {
    if (poPacket) {
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] = poPacket->afSamples[u] * fScale + fBias;
        }
    } else {
        fillWith(fBias);
    }
    return this;
}

Packet* Packet::sumWith(Packet const* poPacket) {
    if (poPacket) {
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] += poPacket->afSamples[u];
        }
    }
    return this;
}

Packet* Packet::modulateWith(Packet const* poPacket) {
    if (poPacket) {
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            afSamples[u] *= poPacket->afSamples[u];
        }
    }
    return this;
}

Packet* Packet::accumulate(Packet const* poPacket, float32 fValue) {
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
        void operator()(Packet* poPacket) const {
            Packet::destroy(poPacket);
        }
};

/**
 * Create a new Packet instance and obtain a shared pointer.
 */
class Packet::Pool {
    public:
        static Packet* apPool[64];
        static uint64  uFreedMask;

        Pool() {

        }
        ~Pool() {
            for (int i=0; i<64; ++i) {
                if (apPool[i]) {
                    std::fprintf(stderr, "Deleting pool Packet at %p [pool index %d]\n", apPool[i], i);
                    delete apPool[i];
                }
            }
        }
};

Packet* Packet::Pool::apPool[64] = { nullptr };
uint64  Packet::Pool::uFreedMask = 0;

// Allow global startup and shutdown to alloc/free the pool
static Packet::Pool oPool;

Packet::Ptr Packet::create() {

    Packet* poPacket = nullptr;

    if (!Pool::uFreedMask) {
        // If the freed mask is zero, we just have to return a new instance.
        poPacket = new Packet();
        //std::fprintf(stderr, "Created new Packet at %p\n", poPacket);
    } else {
        // Otherwise get one of the recently freed.
        int iFreed = __builtin_ffsll(Pool::uFreedMask) - 1;

        // should never happen...
        //if (!Pool::apPool[iFreed]) {
        //    throw std::bad_alloc();
        //}
        poPacket             = Pool::apPool[iFreed];
        Pool::apPool[iFreed] = nullptr;
        Pool::uFreedMask     &= ~(1 << iFreed);
        //std::fprintf(stderr, "Using recycled Packet at %p [pool index %d]\n", poPacket, iFreed);
    }

    // Tracking
    uint64 uPacketsInUse = ++uPacketsCreated - uPacketsDestroyed;
    if (uPacketsInUse > uPeakPacketsInUse) {
        uPeakPacketsInUse = uPacketsInUse;
    }
    return Ptr(poPacket, Deleter());
}

/**
 * @inheritDoc
 */
Packet::ConstPtr Packet::getSilence() {
    static Packet::Ptr pSilence;
    if (!pSilence.get()) {
        pSilence = Packet::create();
        pSilence->fillWith(0.0f);
    }
    return pSilence;
}


/**
 * Free a Packet instance
 */
void Packet::destroy(Packet* poPacket) {
    if (poPacket) {
        ++uPacketsDestroyed;

        // Invert the mask when looking for where to put the Packet for recycling
        uint64 uInvMask = ~Pool::uFreedMask;

        if (uInvMask) {
            int iRecycle = __builtin_ffsll(uInvMask) - 1;
            Pool::apPool[iRecycle] = poPacket;
            Pool::uFreedMask |= 1 << iRecycle;

            // should never happen
//            if (Pool::apPool[iRecycle]) {
//                std::fprintf(stderr, "Recycle slot not empty, but should be\n");
//                delete poPacket;
//                delete Pool::apPool[iRecycle];
//                Pool::apPool[iRecycle] = nullptr;
//            } else {
//                std::fprintf(stderr, "Recyling Packet at %p in pool index %d\n", poPacket, iRecycle);
//                Pool::apPool[iRecycle] = poPacket;
//                Pool::uFreedMask |= 1 << iRecycle;
//            }

        } else {
            //std::fprintf(stderr, "Can't recycle Packet at %p, pool is full\n", poPacket);

            // The freed pool was already full, so we can't recycle the deleted packet
            delete poPacket;
        }

    }
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

bool TPacketIndexAware::useLast(size_t uIndex) {
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
