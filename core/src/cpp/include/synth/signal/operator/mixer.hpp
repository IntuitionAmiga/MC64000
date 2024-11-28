#ifndef MC64K_SYNTH_SIGNAL_MIXER_HPP
    #define MC64K_SYNTH_SIGNAL_MIXER_HPP

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
#include <unordered_map>
#include <synth/signal.hpp>

namespace MC64K::Synth::Audio::Signal::Operator {

/**
 * Simple mixer class. Mixes a fixed number of input channels into an output.
 */
class FixedMixer : public TStreamCommon, public TOutputStream, protected TPacketIndexAware {

    private:
        struct Channel {
            IStream::Ptr oSourcePtr;
            IStream*     poSource;
            float32      fLevel;
            Channel(): poSource{nullptr}, fLevel{0.0f} { }
        };

        Channel*    poChannels;
        //Packet::Ptr oOutputPacketPtr;

        uint64      uBitMap;
        float32     fOutputLevel;
        uint32      uNumChannels;

    public:
        static constexpr uint32 const MIN_CHANNELS = 2;
        static constexpr uint32 const MAX_CHANNELS = 64;

        FixedMixer(uint32 uNumChannels, float32 fOutputLevel = 1.0f);
        ~FixedMixer();

        /**
         * @inheritDoc
         */
        FixedMixer* reset() noexcept override;

        FixedMixer* enable() noexcept override;

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex = 0) noexcept override;

        size_t getInputCount() const noexcept {
            return uNumChannels;
        }

        /**
         * Get the level for an input. Returns zero for any channel out of range.
         *
         * @param  uint32 uChannelNum
         * @return float32
         */
        float32 getInputLevel(uint32 uChannelNum) const noexcept {
            if (uChannelNum < uNumChannels) {
                return poChannels[uChannelNum].fLevel;
            }
            return 0.0f;
        }

        /**
         * Set the level for an input. If the input channel number is out of range, no
         * action is taken.
         *
         * @param  uint32 uChannelNum
         * @return FixedMixer*
         */
        FixedMixer* setInputLevel(uint32 uChannelNum, float32 fLevel) noexcept {
            if (uChannelNum < uNumChannels) {
                poChannels[uChannelNum].fLevel = fLevel;
            }
            return this;
        }


        /**
         * Returns the current output level.
         *
         * @return float32
         */
        float32 getOutputLevel() const noexcept {
            return fOutputLevel;
        }

        /**
         * Sets the output level.
         */
        FixedMixer* setOutputLevel(float32 fLevel) noexcept {
            fOutputLevel = fLevel;
            return this;
        }


        FixedMixer* setInputStream(uint32 uChannelNum, IStream &roSource, float32 fLevel) noexcept;
        FixedMixer* setInputStream(uint32 uChannelNum, IStream::Ptr const& roSourcePtr, float32 fLevel) noexcept;

    protected:
        /**
         * Generate a new packet. This is called by emit() when it is determined that
         * the packet we are being asked for is not the one we last calculated.
         *
         * @return Packet::ConstPtr
         */
        Packet::ConstPtr emitNew() noexcept;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Simple mixer class. Mixes one or more channels into a single output
 * using assignable but non-automated volume controls.
 */
class SimpleMixer : public TStreamCommon, public TOutputStream, protected TPacketIndexAware {

    public:
        // Each attached stream is given an ID so that the level for
        // that ID can be modified. There is no numeric requirement
        // for the ID other than that it uniquely identifies the channel.
        typedef uint64 ChannelID;

    private:
        /**
         * Structure that represents an input channel and level.
         */
        struct Channel {
            IStream::Ptr oSourcePtr;
            IStream*     poSource;
            float32      fLevel;
        };

        std::unordered_map<ChannelID, Channel> oChannels;

        //Packet::Ptr oOutputPacketPtr;

        float32 fOutputLevel;

    public:
        SimpleMixer(float32 fOutputLevel = 1.0f);
        ~SimpleMixer();

        /**
         * @inheritDoc
         */
        SimpleMixer* reset() noexcept override;

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex = 0) noexcept override;

        /**
         * Returns the number of input streams.
         *
         * @return size_t
         */
        size_t getInputCount() const noexcept {
            return oChannels.size();
        }

        /**
         * Returns the current output level.
         *
         * @return float32
         */
        float32 getOutputLevel() const noexcept {
            return fOutputLevel;
        }

        /**
         * Set the current output level.
         *
         * @param  float32 fLevel
         * @return this
         */
        SimpleMixer* setOutputLevel(float32 fLevel) noexcept {
            fOutputLevel = fLevel;
            return this;
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
        SimpleMixer* addInputStream(ChannelID uID, IStream::Ptr const& roSourcePtr, float32 fLevel);

        SimpleMixer* addInputStream(ChannelID uID, IStream& roSource, float32 fLevel);

        /**
         *  Removes an input stream, if it is attached.
         */
        SimpleMixer* removeInputStream(ChannelID uID);

        /**
         * Get the level for an input. Returns zero for any unrecognised input ID.
         *
         * @param  ChannelID uChannelID
         * @return float32
         */
        float32 getInputLevel(ChannelID uID) const noexcept;

        /**
         * Set the level for an input. If the input channel ID is unrecognised, no
         * action is taken.
         *
         * @param  ChannelID uChannelID
         * @return float32
         */
        SimpleMixer* setInputLevel(ChannelID uID, float32 fLevel) noexcept;

    protected:
        /**
         * Generate a new packet. This is called by emit() when it is determined that
         * the packet we are being asked for is not the one we last calculated.
         *
         * @return Packet::ConstPtr
         */
        Packet::ConstPtr emitNew() noexcept;

};

///////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace

#endif
