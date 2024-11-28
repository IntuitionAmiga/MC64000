#ifndef MC64K_SYNTH_SIGNAL_FILTER_4PM_HPP
    #define MC64K_SYNTH_SIGNAL_FILTER_4PM_HPP

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

#include <synth/signal.hpp>

namespace MC64K::Synth::Audio::Signal::Filter {
using namespace MC64K::StandardTestHost::Audio::IConfig;

/**
 * Four pole multimode filter
 *
 */
class FourPoleMultiMode : public IFilter  {
    public:
        enum Mode {
            LOW_PASS    = 0,
            HI_PASS     = 1,
            BAND_PASS   = 2,
            BAND_REJECT = 3,
        };

    private:
        static constexpr float64 const FEEDBACK       = 0.418;
        static constexpr float64 const FEEDBACK_PHASE = 0.36;
        static constexpr float64 const SCALE_MAX_Q    = 4.0;

        typedef Packet::ConstPtr (*Control)(FourPoleMultiMode*);
        typedef void (*Process)(FourPoleMultiMode*);

        Control cCutoff;
        Control cResonance;
        Process cProcess;

        float64 fPole1    = 0.0;
        float64 fPole2    = 0.0;
        float64 fPole3    = 0.0;
        float64 fPole4    = 0.0;
        float64 fFeedback = 0.0;
        Mode    eMode     = LOW_PASS;

        void filterSample(float64 fInput, float64 fCutoff, float64 fResonance) noexcept;

    protected:
        void configure() noexcept override;

    public:
        Packet::ConstPtr emit(uint64 uIndex) noexcept override;

        FourPoleMultiMode* setMode(Mode eMode) noexcept {
            this->eMode = eMode;
            configure();
            return this;
        }

        FourPoleMultiMode(IStream& roInput, Mode eMode, float32 fCutoff, float32 fResonance);
        FourPoleMultiMode(IStream::Ptr const& roInputPtr, Mode eMode, float32 fCutoff, float32 fResonance);
        ~FourPoleMultiMode();

    private:
        // Functions that generate a control input packet, based on which controllers are set.
        static Packet::ConstPtr cutoffMod(FourPoleMultiMode* poFilter) noexcept;
        static Packet::ConstPtr cutoffEnv(FourPoleMultiMode* poFilter) noexcept;
        static Packet::ConstPtr cutoffModEnv(FourPoleMultiMode* poFilter) noexcept;
        static Packet::ConstPtr resonanceMod(FourPoleMultiMode* poFilter) noexcept;
        static Packet::ConstPtr resonanceEnv(FourPoleMultiMode* poFilter) noexcept;
        static Packet::ConstPtr resonanceModEnv(FourPoleMultiMode* poFilter) noexcept;

        // Functions that actually carry out the filtering, based on the mode and which controllers are set.
        static void lowPassFixedCFixedQ(FourPoleMultiMode* poFilter) noexcept;
        static void lowPassVaryingCFixedQ(FourPoleMultiMode* poFilter) noexcept;
        static void lowPassFixedCFVaryingQ(FourPoleMultiMode* poFilter) noexcept;
        static void lowPassVaryingCVaryingQ(FourPoleMultiMode* poFilter) noexcept;
        static void hiPassFixedCFixedQ(FourPoleMultiMode* poFilter) noexcept;
        static void hiPassVaryingCFixedQ(FourPoleMultiMode* poFilter) noexcept;
        static void hiPassFixedCFVaryingQ(FourPoleMultiMode* poFilter) noexcept;
        static void hiPassVaryingCVaryingQ(FourPoleMultiMode* poFilter) noexcept;
        static void bandPassFixedCFixedQ(FourPoleMultiMode* poFilter) noexcept;
        static void bandPassVaryingCFixedQ(FourPoleMultiMode* poFilter) noexcept;
        static void bandPassFixedCFVaryingQ(FourPoleMultiMode* poFilter) noexcept;
        static void bandPassVaryingCVaryingQ(FourPoleMultiMode* poFilter) noexcept;
        static void bandRejectFixedCFixedQ(FourPoleMultiMode* poFilter) noexcept;
        static void bandRejectVaryingCFixedQ(FourPoleMultiMode* poFilter) noexcept;
        static void bandRejectFixedCFVaryingQ(FourPoleMultiMode* poFilter) noexcept;
        static void bandRejectVaryingCVaryingQ(FourPoleMultiMode* poFilter) noexcept;
};

}
#endif
