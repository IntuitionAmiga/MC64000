#ifndef MC64K_SYNTH_SIGNAL_MODULATOR_HPP
    #define MC64K_SYNTH_SIGNAL_MODULATOR_HPP

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
class FixedModulator : public TStreamCommon, TOutputStream, protected TPacketIndexAware {

    private:
        struct Channel {
            IStream::Ptr oSourcePtr;
            IStream*     poSource;
            Channel(): poSource{nullptr} { }
        };

        Channel*    poChannels;

        uint64      uBitMap;
        uint32      uNumChannels;

    public:
        static constexpr uint32 const MIN_CHANNELS = 2;
        static constexpr uint32 const MAX_CHANNELS = 64;

        FixedModulator(uint32 uNumChannels);
        ~FixedModulator();

        /**
         * @inheritDoc
         */
        FixedModulator* reset() noexcept override;

        FixedModulator* enable() noexcept override;

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex = 0) noexcept override;

        size_t getInputCount() const noexcept {
            return uNumChannels;
        }

        FixedModulator* setInputSource(uint32 uChannelNum, IStream &roSource) noexcept {
            if (uChannelNum < uNumChannels) {
                poChannels[uChannelNum].poSource   = &roSource;
                uBitMap |= 1 << uChannelNum;
            }
            return this;
        }


        FixedModulator* setInputSource(uint32 uChannelNum, IStream::Ptr const& roSourcePtr) noexcept {
            if (uChannelNum < uNumChannels) {
                poChannels[uChannelNum].oSourcePtr = roSourcePtr;
                poChannels[uChannelNum].poSource   = roSourcePtr.get();
                if (roSourcePtr.get()) {
                    uBitMap |= 1 << uChannelNum;
                } else {
                    uBitMap &= ~(1 << uChannelNum);
                }
            }
            return this;
        }

    protected:
        /**
         * Generate a new packet. This is called by emit() when it is determined that
         * the packet we are being asked for is not the one we last calculated.
         *
         * @return Packet::ConstPtr
         */
        Packet::ConstPtr emitNew() noexcept;
};


} // namespace

#endif
