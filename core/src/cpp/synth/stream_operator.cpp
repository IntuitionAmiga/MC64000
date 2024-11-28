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

#include <synth/signal/operator/delta_encode.hpp>
#include <synth/signal/operator/leveladjust.hpp>
#include <synth/signal/operator/mixer.hpp>
#include <synth/signal/operator/modulator.hpp>
#include <synth/signal/operator/automute.hpp>
#include <synth/signal/operator/packet_relay.hpp>

namespace MC64K::Synth::Audio::Signal::Operator {

///////////////////////////////////////////////////////////////////////////////////////////////////

SingleInSingleOut* SingleInSingleOut::reset() noexcept {
    uLastIndex = 0;
    uSamplePosition = 0;
    if (poInputStream) {
        poInputStream->reset();
    }
    return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////


Packet::ConstPtr DeltaEncode::emitNew() noexcept {
    float32 const* pfInput = poInputStream->emit(uLastIndex)->afSamples;
    float32* pfOutput = oOutputPacketPtr->afSamples;

    for (unsigned u = 0; u < PACKET_SIZE; ++u) {
        pfOutput[u] = pfInput[u] - fPrevious;
        fPrevious = pfInput[u];
    }

    return oOutputPacketPtr;
}


///////////////////////////////////////////////////////////////////////////////////////////////////

LevelAdjust::LevelAdjust(
    IStream& roSourceInput,
    float32 fInitialOutputLevel,
    float32 fInitialOutputBias
):
    SingleInSingleOut{roSourceInput},
    TOutputStream{},
    fOutputLevel{0.0f},
    fOutputBias{fInitialOutputBias},
    bMuted{false}
{
    setOutputLevel(fInitialOutputLevel);
    std::fprintf(stderr, "Created LevelAdjust at %p with output level %.3f with input at %p\n", this, fOutputLevel, poInputStream);
}


LevelAdjust::LevelAdjust(
    IStream::Ptr const& roSourceInputPtr,
    float32 fInitialOutputLevel,
    float32 fInitialOutputBias
):
    SingleInSingleOut{roSourceInputPtr},
    TOutputStream{},
    fOutputLevel{0.0f},
    fOutputBias{fInitialOutputBias},
    bMuted{false}
{
    poInputStream = oInputStreamPtr.get();
    setOutputLevel(fInitialOutputLevel);
    std::fprintf(stderr, "Created LevelAdjust at %p with output level %.3f\n", this, fOutputLevel);
}

LevelAdjust::~LevelAdjust() {
    std::fprintf(stderr, "Destroyed LevelAdjust at %p\n", this);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr LevelAdjust::emit(size_t uIndex) noexcept {
    if (!bEnabled || bMuted) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return oOutputPacketPtr;
    }
    return emitNew();
}

Packet::ConstPtr LevelAdjust::emitNew() noexcept {
    oOutputPacketPtr->scaleAndBiasBy(poInputStream->emit(uLastIndex), fOutputLevel, fOutputBias);
    return oOutputPacketPtr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

FixedMixer::FixedMixer(uint32 uNumChannels, float32 fOutputLevel):
    TOutputStream{},
    poChannels{nullptr},
    uBitMap{0},
    fOutputLevel{fOutputLevel}
{
    this->uNumChannels = uNumChannels < MIN_CHANNELS ?
        MIN_CHANNELS : uNumChannels > MAX_CHANNELS ?
            MAX_CHANNELS : uNumChannels;
    poChannels = new Channel[this->uNumChannels];
    std::fprintf(stderr, "Created FixedMixer at %p with output level %.3f\n", this, fOutputLevel);
}

FixedMixer::~FixedMixer() {
    delete[] poChannels;
    std::fprintf(stderr, "Destroyed FixedMixer at %p\n", this);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FixedMixer::emit(size_t uIndex) noexcept {
    if (!bEnabled || 0 == uBitMap) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return oOutputPacketPtr;
    }
    return emitNew();
}

Packet::ConstPtr FixedMixer::emitNew() noexcept {
    Packet* poOutput = oOutputPacketPtr.get();
    poOutput->clear();
    for (uint32 uChannelNum = 0; uChannelNum < uNumChannels; ++uChannelNum) {
        if (auto poInput = poChannels[uChannelNum].poSource) {
            if (poInput->isEnabled()) {
                poOutput->accumulate(
                    poInput->emit(uLastIndex),
                    poChannels[uChannelNum].fLevel * fOutputLevel
                );

            }
        }
    }
    return oOutputPacketPtr;
}

FixedMixer* FixedMixer::reset() noexcept {
    uLastIndex = 0;
    uSamplePosition = 0;
    oOutputPacketPtr->clear();
    std::fprintf(stderr, "FixedMixer %p reset()\n", this);
    for (uint32 uChannelNum = 0; uChannelNum < uNumChannels; ++uChannelNum) {
        if (auto poInput = poChannels[uChannelNum].poSource) {
            poInput->reset();
        }
    }
    return this;
}

FixedMixer* FixedMixer::enable() noexcept {
    TStreamCommon::enable();
    if (bEnabled) {
        std::fprintf(stderr, "FixedMixer %p enable()\n", this);
        for (uint32 uChannelNum = 0; uChannelNum < uNumChannels; ++uChannelNum) {
            if (auto poInput = poChannels[uChannelNum].poSource) {
                poInput->enable();
            }
        }
    }
    return this;
}

FixedMixer* FixedMixer::setInputStream(uint32 uChannelNum, IStream &roSource, float32 fLevel) noexcept {
    if (uChannelNum < uNumChannels) {
        poChannels[uChannelNum].poSource   = &roSource;
        poChannels[uChannelNum].fLevel     = fLevel;
        uBitMap |= 1 << uChannelNum;
    }
    return this;
}


FixedMixer* FixedMixer::setInputStream(uint32 uChannelNum, IStream::Ptr const& roSourcePtr, float32 fLevel) noexcept {
    if (uChannelNum < uNumChannels) {
        poChannels[uChannelNum].oSourcePtr = roSourcePtr;
        poChannels[uChannelNum].poSource   = roSourcePtr.get();
        poChannels[uChannelNum].fLevel     = fLevel;
        if (roSourcePtr.get()) {
            uBitMap |= 1 << uChannelNum;
        } else {
            uBitMap &= ~(1 << uChannelNum);
        }
    }
    return this;
}


///////////////////////////////////////////////////////////////////////////////////////////////////

SimpleMixer::SimpleMixer(float32 fOutputLevel):
    TOutputStream{},
    fOutputLevel(fOutputLevel)
{
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
SimpleMixer* SimpleMixer::reset() noexcept {
    uLastIndex = 0;
    uSamplePosition = 0;
    oOutputPacketPtr->clear();
    std::fprintf(stderr, "SimpleMixer %p reset()\n", this);
    for (auto pPair = oChannels.begin(); pPair != oChannels.end(); ++pPair) {
        pPair->second.poSource->reset();
        std::fprintf(
            stderr,
            "\tResetting input ID:%lu [%p]\n",
            pPair->first,
            pPair->second.poSource
        );
    }
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr SimpleMixer::emit(size_t uIndex) noexcept {
    if (!bEnabled || 0 == oChannels.size()) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return oOutputPacketPtr;
    }
    return emitNew();
}

Packet::ConstPtr SimpleMixer::emitNew() noexcept {
    Packet* pOutput = oOutputPacketPtr.get();
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
    return oOutputPacketPtr;
}

/**
 * Attach (or replace) an input stream. If the stream pointer is empty
 * no action is taken.
 *
 * @param  ChannelID uID
 * @param  IStream::Ptr oSourcePtr
 * @param  float32 fLevel
 * @return this
 */
SimpleMixer* SimpleMixer::addInputStream(SimpleMixer::ChannelID uID, IStream::Ptr const& oSourcePtr, float32 fLevel) {
    if (oSourcePtr.get()) {
        std::fprintf(stderr, "SimpleMixer %p addInputStream() setting %p [ID:%lu]\n", this, oSourcePtr.get(), uID);
        Channel& roChannel   = oChannels[uID];
        roChannel.oSourcePtr = oSourcePtr;
        roChannel.poSource   = oSourcePtr.get();
        roChannel.fLevel     = fLevel;
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
float32 SimpleMixer::getInputLevel(SimpleMixer::ChannelID uID) const noexcept {
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
SimpleMixer* SimpleMixer::setInputLevel(SimpleMixer::ChannelID uID, float32 fLevel) noexcept {
    auto pChannel = oChannels.find(uID);
    if (pChannel != oChannels.end()) {
        pChannel->second.fLevel = fLevel;
    }
    return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

FixedModulator::FixedModulator(uint32 uNumChannels):
    TOutputStream{},
    poChannels{nullptr},
    uBitMap{0} {
    this->uNumChannels = uNumChannels < MIN_CHANNELS ?
        MIN_CHANNELS : uNumChannels > MAX_CHANNELS ?
            MAX_CHANNELS : uNumChannels;
    poChannels = new Channel[this->uNumChannels];
    std::fprintf(stderr, "Created FixedModulator at %p\n", this);
}

FixedModulator::~FixedModulator() {
    delete[] poChannels;
    std::fprintf(stderr, "Destroyed FixedModulator at %p\n", this);
}

/**
 * @inheritDoc
 */
Packet::ConstPtr FixedModulator::emit(size_t uIndex) noexcept {

    if (!bEnabled || 0 == uBitMap) {
        return Packet::getSilence();
    }
    if (useLast(uIndex)) {
        return oOutputPacketPtr;
    }
    return emitNew();
}

Packet::ConstPtr FixedModulator::emitNew() noexcept {
    Packet* poOutput = oOutputPacketPtr.get();
    poOutput->fillWith(1.0f);
    for (uint32 uChannelNum = 0; uChannelNum < uNumChannels; ++uChannelNum) {
        if (auto poInput = poChannels[uChannelNum].poSource) {
            if (poInput->isEnabled()) {
                poOutput->modulateWith(
                    poInput->emit(uLastIndex)
                );
            }
        }
    }
    return oOutputPacketPtr;
}

FixedModulator* FixedModulator::reset() noexcept {
    uLastIndex = 0;
    uSamplePosition = 0;
    oOutputPacketPtr->clear();
    std::fprintf(stderr, "FixedModulator %p reset()\n", this);
    for (uint32 uChannelNum = 0; uChannelNum < uNumChannels; ++uChannelNum) {
        if (auto poInput = poChannels[uChannelNum].poSource) {
            poInput->reset();
        }
    }
    return this;
}

FixedModulator* FixedModulator::enable() noexcept {
    TStreamCommon::enable();
    if (bEnabled) {
        std::fprintf(stderr, "FixedModulator %p enable()\n", this);
        for (uint32 uChannelNum = 0; uChannelNum < uNumChannels; ++uChannelNum) {
            if (auto poInput = poChannels[uChannelNum].poSource) {
                poInput->enable();
            }
        }
    }
    return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @inheritDoc
 */
AutoMuteSilence::AutoMuteSilence(
    IStream::Ptr const& roInputPtr,
    float32 fDuration,
    float32 fRMSLevel
) {
    setStream(roInputPtr);
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
size_t AutoMuteSilence::getPosition() noexcept {
    return oSourcePtr.get() ? oSourcePtr->getPosition() : 0;
}

/**
 * @inheritDoc
 */
bool AutoMuteSilence::isEnabled() const noexcept {
    return bEnabled;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::enable() noexcept {
    if (oSourcePtr.get()) {
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
AutoMuteSilence* AutoMuteSilence::disable() noexcept {
    bEnabled = false;
    return this;
}

/**
 * @inheritDoc
 */
AutoMuteSilence* AutoMuteSilence::reset() noexcept {
    if (oSourcePtr.get()) {
        oSourcePtr->reset();
        fLastTotalSquared  = 0.0;
        uSilentPacketCount = 0;
        bEnabled = true;
    }
    return this;
}

/**
 * @inheritDoc
 */
Packet::ConstPtr AutoMuteSilence::emit(size_t uIndex) noexcept {
    if (bEnabled) {
        auto poPacket = oSourcePtr->emit(uIndex);
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
AutoMuteSilence* AutoMuteSilence::setDuration(float32 fSeconds) noexcept {
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
AutoMuteSilence* AutoMuteSilence::setThreshold(float32 fRMSLevel) noexcept {
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
AutoMuteSilence* AutoMuteSilence::setStream(IStream::Ptr const& roInputPtr) noexcept {
    oSourcePtr = roInputPtr;
    if (!oSourcePtr.get()) {
        disable();
    }
    return this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

PacketRelay::PacketRelay():
    TOutputStream{}
{
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
Packet::ConstPtr PacketRelay::emit(size_t uIndex) noexcept {
    uSamplePosition += PACKET_SIZE;
    if (!oOutputPacketPtr.get()) {
        return Packet::getSilence();
    }
    return oOutputPacketPtr;
}

}


