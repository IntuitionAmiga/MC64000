#ifndef MC64K_SYNTH_SIGNAL_AUTOMUTE_HPP
    #define MC64K_SYNTH_SIGNAL_AUTOMUTE_HPP

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

// TODO - reimplement extending SingleInSingleOut

/**
 * Simple auto mute class that detects when it's input is below a critical threshold
 * and mutes it. When muted, silence packets are returned and the input stream is not
 * invoked.
 */
class AutoMuteSilence : public virtual IStream, protected TPacketIndexAware {

    private:
        IStream::Ptr oSourcePtr;
        float64 fThresholdSquared;
        float64 fLastTotalSquared;
        uint32  uSilentPacketLimit;
        uint32  uSilentPacketCount;
        bool    bEnabled;

        static constexpr float64 const RMS_SCALE = 1.0 / PACKET_SIZE;

    public:
        static constexpr float32 const DEF_THRESHOLD = 1.0f/256.0f;
        static constexpr float32 const DEF_DURATION  = 0.05f;

        AutoMuteSilence(
            IStream::Ptr const& roInput,
            float32 fDuration = DEF_DURATION,
            float32 fRMSLevel = DEF_THRESHOLD
        );

        ~AutoMuteSilence();

        /**
         * @inheritDoc
         */
        size_t getPosition() noexcept;

        /**
         * @inheritDoc
         */
        bool isEnabled() const noexcept override;

        /**
         * @inheritDoc
         */
        AutoMuteSilence* enable() noexcept override;

        /**
         * @inheritDoc
         */
        AutoMuteSilence* disable() noexcept override;

        /**
         * @inheritDoc
         */
        AutoMuteSilence* reset() noexcept override;

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex = 0) noexcept override;

        /**
         * Set the duration the input stream must be silent for before disabling.
         */
        AutoMuteSilence* setDuration(float32 fSeconds) noexcept;

        /**
         * Set the RMS signal level the input stream must be silent for before
         * disabling.
         */
        AutoMuteSilence* setThreshold(float32 fRMSLevel) noexcept;

        /**
         * Set a new input stream. If the pointer is empty, the unit is disabled.
         */
        AutoMuteSilence* setStream(IStream::Ptr const& roInputPtr) noexcept;

        /**
         * Get the enclosed stream
         */
        IStream::Ptr getStream() const noexcept {
            return oSourcePtr;
        }

};

} // namespace

#endif
