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

Packet* Packet::sumWith(Packet const* poPacket) {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] += poPacket->afSamples[u];
    }
    return this;
}

Packet* Packet::modulateWith(Packet const* poPacket) {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] *= poPacket->afSamples[u];
    }
    return this;
}

Packet* Packet::accumulate(Packet const* poPacket, float32 fValue) {
    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        afSamples[u] += poPacket->afSamples[u] * fValue;
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
Packet::Ptr Packet::create() {
    Packet* poPacket = new Packet();
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
        delete poPacket;
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
