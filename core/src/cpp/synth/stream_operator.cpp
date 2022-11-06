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
#include <synth/signal/operator/mixer.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/operator/mixer.hpp>
namespace MC64K::Synth::Audio::Signal::Operator {

FixedMixer::FixedMixer(float32 fOutputLevel): fOutputLevel(fOutputLevel) {
    std::fprintf(stderr, "Created FixedMixer at %p\n", this);
}

FixedMixer::~FixedMixer() {
    std::fprintf(stderr, "Destroyed FixedMixer at %p\n", this);
}

/**
 * @inheritDoc
 *
 * Zero out
 */
FixedMixer* FixedMixer::reset() {
    uLastIndex = 0;
    uSamplePosition = 0;
    if ( auto p = pLastPacket.get() ) {
        p->clear();
    }
    std::fprintf(stderr, "FixedMixer %p reset()\n", this);
    for (auto pPair = oChannels.begin(); pPair != oChannels.end(); ++pPair) {
        pPair->second.pSource->reset();
        std::fprintf(
            stderr,
            "\tResetting input ID:%lu [%p]\n",
            pPair->first,
            pPair->second.pSource.get()
        );
    }
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FixedMixer::emit(size_t uIndex) {
    if (!bEnabled || 0 == oChannels.size()) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return pLastPacket;
    }
    return emitNew();
}

Packet::ConstPtr FixedMixer::emitNew() {
    if (!pLastPacket.get()) {
        pLastPacket = Packet::create();
    }
    Packet* pOutput = pLastPacket.get();
    pOutput->clear();
    for (auto pPair = oChannels.begin(); pPair != oChannels.end(); ++pPair) {
        if (pPair->second.pSource->isEnabled()) {
            pOutput->accumulate(
                pPair->second.pSource->emit(uLastIndex),
                pPair->second.fLevel
            );
        }
    }
    pOutput->scaleBy(fOutputLevel);
    return pLastPacket;
}

/**
 * Set the current output level.
 *
 * @param  float32 fLevel
 * @return this
 */
FixedMixer* FixedMixer::setOutputLevel(float32 fLevel) {
    fOutputLevel = fLevel;
    return this;
}

/**
 * Attach (or replace) an input stream. If the stream pointer is empty
 * no action is taken.
 *
 * @param  ChannelID uID
 * @param  IStream::Ptr pSource
 * @param  float32 fLevel
 * @return this
 */
FixedMixer* FixedMixer::addInputStream(FixedMixer::ChannelID uID, IStream::Ptr pSource, float32 fLevel) {
    if (pSource.get()) {
        std::fprintf(stderr, "FixedMixer %p addInputStream() setting %p [ID:%lu]\n", this, pSource.get(), uID);
        Channel& roChannel = oChannels[uID];
        roChannel.pSource  = pSource;
        roChannel.fLevel   = fLevel;
    } else {
        std::fprintf(stderr, "FixedMixer %p addInputStream() not adding empty stream [ID:%lu]\n", this, uID);
    }
    return this;
}

/**
 *  Removes an input stream, if it is attached.
 */
FixedMixer* FixedMixer::removeIputStream(FixedMixer::ChannelID uID) {
    oChannels.erase(uID);
    return this;
}

/**
 * Get the level for an input. Returns zero for any unrecognised input ID.
 *
 * @param  ChannelID uChannelID
 * @return float32
 */
float32 FixedMixer::getInputLevel(FixedMixer::ChannelID uID) const {
    auto pChannel = oChannels.find(uID);
    if (pChannel != oChannels.end()) {
        return pChannel->second.fLevel;
    }
    return 0.0f;
}

/**
 * Set the level for an input. If the input channel ID is unrecognised, no
 * action is taken.
 *
 * @param  ChannelID uChannelID
 * @return float32
 */
FixedMixer* FixedMixer::setInputLevel(FixedMixer::ChannelID uID, float32 fLevel) {
    auto pChannel = oChannels.find(uID);
    if (pChannel != oChannels.end()) {
        pChannel->second.fLevel = fLevel;
    }
    return this;
}

} // namespace

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/operator/automute.hpp>
namespace MC64K::Synth::Audio::Signal::Operator {


}


