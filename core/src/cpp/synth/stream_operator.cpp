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

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <synth/signal/operator/leveladjust.hpp>
#include <synth/signal/operator/mixer.hpp>
#include <synth/signal/operator/automute.hpp>
#include <synth/signal/operator/packet_relay.hpp>

namespace MC64K::Synth::Audio::Signal::Operator {

LevelAdjust::LevelAdjust(
    IStream& roSourceInput,
    float32 fInitialOutputLevel,
    float32 fInitialOutputBias
):
    poSourceInput{&roSourceInput},
    fOutputLevel{0.0f},
    fOutputBias{fInitialOutputBias},
    bMuted{false}
{
    setOutputLevel(fInitialOutputLevel);
    std::fprintf(stderr, "Created LevelAdjust at %p with output level %.3f with input at %p\n", this, fOutputLevel, poSourceInput);
}


LevelAdjust::LevelAdjust(
    IStream::Ptr const& roSourceInputPtr,
    float32 fInitialOutputLevel,
    float32 fInitialOutputBias
):
    oSourceInputPtr{roSourceInputPtr},
    fOutputLevel{0.0f},
    fOutputBias{fInitialOutputBias},
    bMuted{false}
{
    poSourceInput = oSourceInputPtr.get();
    setOutputLevel(fInitialOutputLevel);
    std::fprintf(stderr, "Created LevelAdjust at %p with output level %.3f\n", this, fOutputLevel);
}

LevelAdjust::~LevelAdjust() {
    std::fprintf(stderr, "Destroyed LevelAdjust at %p\n", this);
}

bool LevelAdjust::canEnable() const {
    return poSourceInput != nullptr;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr LevelAdjust::emit(size_t uIndex) {
    if (!bEnabled || bMuted) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return oLastPacketPtr;
    }
    return emitNew();
}

Packet::ConstPtr LevelAdjust::emitNew() {
    if (!oLastPacketPtr.get()) {
        oLastPacketPtr = Packet::create();
    }
    oLastPacketPtr->scaleAndBiasBy(poSourceInput->emit(uLastIndex), fOutputLevel, fOutputBias);
    return oLastPacketPtr;
}

LevelAdjust* LevelAdjust::reset() {
    uLastIndex = 0;
    uSamplePosition = 0;
    if ( auto p = oLastPacketPtr.get() ) {
        p->clear();
    }
    if (poSourceInput) {
        poSourceInput->reset();
    }
    return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

FixedMixer::FixedMixer(uint32 uNumChannels, float32 fOutputLevel):
    pChannels{nullptr},
    uBitMap{0},
    fOutputLevel{fOutputLevel} {
    this->uNumChannels = uNumChannels < MIN_CHANNELS ?
        MIN_CHANNELS : uNumChannels > MAX_CHANNELS ?
            MAX_CHANNELS : uNumChannels;
    pChannels = new Channel[this->uNumChannels];
    std::fprintf(stderr, "Created FixedMixer at %p with output level %.3f\n", this, fOutputLevel);
}

FixedMixer::~FixedMixer() {
    delete[] pChannels;
    std::fprintf(stderr, "Destroyed FixedMixer at %p\n", this);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FixedMixer::emit(size_t uIndex) {

    if (!bEnabled || 0 == uBitMap) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return poLastPacket;
    }
    return emitNew();
}

Packet::ConstPtr FixedMixer::emitNew() {
    if (!poLastPacket.get()) {
        poLastPacket = Packet::create();
    }
    Packet* poOutput = poLastPacket.get();
    poOutput->clear();
    for (uint32 uChannelNum = 0; uChannelNum < uNumChannels; ++uChannelNum) {
        if (auto poInput = pChannels[uChannelNum].poSource.get()) {
            if (poInput->isEnabled()) {
                poOutput->accumulate(
                    poInput->emit(uLastIndex),
                    pChannels[uChannelNum].fLevel * fOutputLevel
                );
            }
        }
    }
    return poLastPacket;
}

FixedMixer* FixedMixer::reset() {
    uLastIndex = 0;
    uSamplePosition = 0;
    if ( auto p = poLastPacket.get() ) {
        p->clear();
    }
    std::fprintf(stderr, "FixedMixer %p reset()\n", this);

    for (uint32 uChannelNum = 0; uChannelNum < uNumChannels; ++uChannelNum) {
        if (auto poInput = pChannels[uChannelNum].poSource.get()) {
            poInput->reset();
        }
    }
    return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

SimpleMixer::SimpleMixer(float32 fOutputLevel): fOutputLevel(fOutputLevel) {
    std::fprintf(stderr, "Created SimpleMixer at %p with output level %.3f\n", this, fOutputLevel);
}

SimpleMixer::~SimpleMixer() {
    std::fprintf(stderr, "Destroyed SimpleMixer at %p\n", this);
}

/**
 * @inheritDoc
 *
 * Zero out
 */
SimpleMixer* SimpleMixer::reset() {
    uLastIndex = 0;
    uSamplePosition = 0;
    if ( auto p = poLastPacket.get() ) {
        p->clear();
    }
    std::fprintf(stderr, "SimpleMixer %p reset()\n", this);
    for (auto pPair = oChannels.begin(); pPair != oChannels.end(); ++pPair) {
        pPair->second.poSource->reset();
        std::fprintf(
            stderr,
            "\tResetting input ID:%lu [%p]\n",
            pPair->first,
            pPair->second.poSource.get()
        );
    }
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr SimpleMixer::emit(size_t uIndex) {
    if (!bEnabled || 0 == oChannels.size()) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return poLastPacket;
    }
    return emitNew();
}

Packet::ConstPtr SimpleMixer::emitNew() {
    if (!poLastPacket.get()) {
        poLastPacket = Packet::create();
    }
    Packet* pOutput = poLastPacket.get();
    pOutput->clear();
    for (auto pPair = oChannels.begin(); pPair != oChannels.end(); ++pPair) {
        if (pPair->second.poSource->isEnabled()) {
            pOutput->accumulate(
                pPair->second.poSource->emit(uLastIndex),
                pPair->second.fLevel
            );
        }
    }
    pOutput->scaleBy(fOutputLevel);
    return poLastPacket;
}

/**
 * Set the current output level.
 *
 * @param  float32 fLevel
 * @return this
 */
SimpleMixer* SimpleMixer::setOutputLevel(float32 fLevel) {
    fOutputLevel = fLevel;
    return this;
}

/**
 * Attach (or replace) an input stream. If the stream pointer is empty
 * no action is taken.
 *
 * @param  ChannelID uID
 * @param  IStream::Ptr poSource
 * @param  float32 fLevel
 * @return this
 */
SimpleMixer* SimpleMixer::addInputStream(SimpleMixer::ChannelID uID, IStream::Ptr const& poSource, float32 fLevel) {
    if (poSource.get()) {

        std::fprintf(stderr, "SimpleMixer %p addInputStream() setting %p [ID:%lu]\n", this, poSource.get(), uID);
        Channel& roChannel = oChannels[uID];
        roChannel.poSource  = poSource;
        roChannel.fLevel   = fLevel;
    } else {
        std::fprintf(stderr, "SimpleMixer %p addInputStream() not adding empty stream [ID:%lu]\n", this, uID);
    }
    return this;
}

/**
 *  Removes an input stream, if it is attached.
 */
SimpleMixer* SimpleMixer::removeInputStream(SimpleMixer::ChannelID uID) {
    oChannels.erase(uID);
    return this;
}

/**
 * Get the level for an input. Returns zero for any unrecognised input ID.
 *
 * @param  ChannelID uChannelID
 * @return float32
 */
float32 SimpleMixer::getInputLevel(SimpleMixer::ChannelID uID) const {
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
SimpleMixer* SimpleMixer::setInputLevel(SimpleMixer::ChannelID uID, float32 fLevel) {
    auto pChannel = oChannels.find(uID);
    if (pChannel != oChannels.end()) {
        pChannel->second.fLevel = fLevel;
    }
    return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @inheritDoc
 */
AutoMuteSilence::AutoMuteSilence(
    IStream::Ptr poInput,
    float32 fDuration,
    float32 fRMSLevel
) {
    setStream(poInput);
    setDuration(fDuration);
    setThreshold(fRMSLevel);
    std::fprintf(stderr, "Created AutoMuteSilence at %p\n", this);
}

/**
 * @inheritDoc
 */
AutoMuteSilence::~AutoMuteSilence() {
    std::fprintf(stderr, "Destroyed AutoMuteSilence at %p\n", this);
}

/**
 * @inheritDoc
 */
size_t AutoMuteSilence::getPosition() {
    return poSource.get() ? poSource->getPosition() : 0;
}

/**
 * @inheritDoc
 */
bool AutoMuteSilence::isEnabled() {
    return bEnabled;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::enable() {
    if (poSource.get()) {
        fLastTotalSquared  = 0.0;
        uSilentPacketCount = 0;
        bEnabled = true;
    } else {
        bEnabled = false;
    }
    return this;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::disable() {
    bEnabled = false;
    return this;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::reset() {
    if (poSource.get()) {
        poSource->reset();
        fLastTotalSquared  = 0.0;
        uSilentPacketCount = 0;
        bEnabled = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr AutoMuteSilence::emit(size_t uIndex) {
    if (bEnabled) {
        auto poPacket = poSource->emit(uIndex);
        float64 fTotalSquared = 0.0;

        float32 const* afSamples = poPacket->afSamples;
        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            float64 fSample = afSamples[u];
            fTotalSquared += fSample * fSample;
        }
        fTotalSquared *= RMS_SCALE;

        if (fTotalSquared > fLastTotalSquared) {
            // If the total is rising, keep the gate open
            fLastTotalSquared =  fTotalSquared;
            uSilentPacketCount = 0;
        } else if (fTotalSquared < fThresholdSquared) {
            // If the total is not rising and is below the threshold, start closing the gate
            if (++uSilentPacketCount > uSilentPacketLimit) {
                // Closed
                bEnabled          = false;
                fLastTotalSquared = 0.0;
            }
        } else {
            // Not rising but above the threshold, keep gate open
            uSilentPacketCount = 0;
        }
        return poPacket;
    } else {
        return Packet::getSilence();
    }
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::setDuration(float32 fSeconds) {
    uSilentPacketLimit = (fSeconds > 0.0f) ? (uint32)(fSeconds / PACKET_PERIOD) : 0;
    std::fprintf(
        stderr,
        "AutoMuteSilence::setDuration() %.03fs (%u consecutive packets)\n",
        fSeconds,
        uSilentPacketLimit
    );
    return this;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::setThreshold(float32 fRMSLevel) {
    fThresholdSquared = (float64)fRMSLevel;
    fThresholdSquared *= fThresholdSquared;
    std::fprintf(
        stderr,
        "AutoMuteSilence::setThreshold() RMS: %.6g\n",
        fRMSLevel
    );
    return this;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::setStream(IStream::Ptr const& poInput) {
    poSource = poInput;
    if (!poSource.get()) {
        disable();
    }
    return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PacketRelay::PacketRelay() {
    bEnabled = true;
    std::fprintf(
        stderr,
        "Created PacketRelay at %p\n",
        this
    );
}

PacketRelay::~PacketRelay() {
    std::fprintf(
        stderr,
        "Destroyed PacketRelay at %p\n",
        this
    );
}

/**
 * @inheritDoc
 */
Packet::ConstPtr PacketRelay::emit(size_t uIndex) {
    uSamplePosition += PACKET_SIZE;
    if (!oOutputPacketPtr.get()) {
        return Packet::getSilence();
    }
    return oOutputPacketPtr;
}

}


