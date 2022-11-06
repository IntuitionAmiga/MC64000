#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_AUTOMUTE_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_AUTOMUTE_HPP

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
 * Simple mixer class. Mixes one or more channels into a single output
 * using assignable but non-automated volume controls.
 */
class AutoMuteSilence : public IStream, protected TPacketIndexAware {

    private:
        IStream::Ptr pSource;
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
            IStream::Ptr pInput,
            float32 fDuration = DEF_DURATION,
            float32 fRMSLevel = DEF_THRESHOLD
        );

        ~AutoMuteSilence();

        /**
         * @inheritDoc
         */
        size_t getPosition();

        /**
         * @inheritDoc
         */
        bool isEnabled();

        /**
         * @inheritDoc
         */
        AutoMuteSilence* enable();

        /**
         * @inheritDoc
         */
        AutoMuteSilence* disable();

        /**
         * @inheritDoc
         */
        AutoMuteSilence* reset();

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex = 0);

        /**
         * Set the duration the input stream must be silent for before disabling.
         */
        AutoMuteSilence* setDuration(float32 fSeconds);

        /**
         * Set the RMS signal level the input stream must be silent for before
         * disabling.
         */
        AutoMuteSilence* setThreshold(float32 fRMSLevel);

        /**
         * Set a new input stream. If the pointer is empty, the unit is disabled.
         */
        AutoMuteSilence* setStream(IStream::Ptr pInput);

        /**
         * Get the enclosed stream
         */
        IStream::Ptr getStream() const {
            return pSource;
        }

};

} // namespace

#endif
