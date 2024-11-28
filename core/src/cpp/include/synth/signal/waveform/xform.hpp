#ifndef MC64K_SYNTH_SIGNAL_WAVE_XFORM_HPP
    #define MC64K_SYNTH_SIGNAL_WAVE_XFORM_HPP

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

#include "constants.hpp"
#include <initializer_list>

namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * Transformer
 *
 * Divides an existing wave into 2<<N sequential slices that can have their phase and level
 * properties shifted and scaled. This can be used to produce many variants from simpler waveforms
 * with interesting harmonic modifications.
 */
template<size_t N, bool bMutable>
class XForm : public IWaveform {

    private:
        static constexpr uint32  const SIZE   = 2 << N;
        static constexpr uint32  const T_SIZE = 4 * SIZE;
        static constexpr float32 const PERIOD = (float32)(SIZE);
        enum {
            PHASE_MUL = 0,
            PHASE_ADD = 1,
            LEVEL_MUL = 2,
            LEVEL_ADD = 3
        };

        // 2 << N * { phase multiplier, phase displacement, scale multiplier, bias }

        Packet::Ptr oReshapePacketPtr;

        float32 afTransform[4 * SIZE] __attribute__ ((aligned (16))) = { };
        Ptr     poWaveform;
        float32 fPeriodAdjust;

    public:
        XForm(Ptr poSourceWaveform, float32 const* afCustomTransform):
            poWaveform{poSourceWaveform}
        {
            static_assert(N < 4, "Invalid size exponent for XForm");
            fPeriodAdjust = poWaveform->getPeriod() / PERIOD;
            if (afCustomTransform) {
                for (unsigned i = 0; i < T_SIZE; ++i) {
                    afTransform[i] = afCustomTransform[i];
                }
            }
//             std::fprintf(stderr, "Created XForm<%u> at %p with matrix:\n", (uint32)N, this);
//             for (unsigned i = 0; i < T_SIZE; i += 4) {
//                 std::fprintf(
//                     stderr, "\t| %8.3f %8.3f %8.3f %8.3f |\n",
//                     afTransform[i],
//                     afTransform[i + 1],
//                     afTransform[i + 2],
//                     afTransform[i + 3]
//                 );
//             }
        }

        /**
         * Constructor for initialiser list
         */
        XForm(Ptr poSourceWaveform, std::initializer_list<float32> const& roCustomTransform):
            XForm(poSourceWaveform, roCustomTransform.begin())
        {
        }

        ~XForm() {
            oReshapePacketPtr.reset();
            std::fprintf(stderr, "Destroyed XForm<%u> at %p\n", (uint32)N, this);
        }

        /**
         * @inheritDoc
         */
        float32 getPeriod() const noexcept override {
            return PERIOD;
        }

        /**
         * @inheritDoc
         */
        void map(Packet const* poInput, Packet* poOutput) noexcept override {
            if (!oReshapePacketPtr.get()) {
                oReshapePacketPtr = Packet::createUnpooled();
            }
            float32*       pDest     = oReshapePacketPtr->afSamples;
            float32 const* pSrc      = poInput->afSamples;

            // Apply input phase modification
            for (unsigned i = 0; i < PACKET_SIZE; ++i) {
                float32 fVal          = pSrc[i];
                float32 fFloor        = std::floor(fVal);
                int32   iPhase        = (int32)fFloor;
                float32 const* aSlice = afTransform + ((iPhase & (SIZE - 1)) << 2);
                pDest[i] = fPeriodAdjust * (
                    aSlice[PHASE_MUL] * (fVal - fFloor) + aSlice[PHASE_ADD]
                );
            }

            poWaveform->map(oReshapePacketPtr.get(), poOutput);

            // Apply the output amplitude modification
            pDest = poOutput->afSamples;
            for (unsigned i = 0; i < PACKET_SIZE; ++i) {
                float32 const* aSlice = afTransform + ((((int32)std::floor(pSrc[i])) & (SIZE - 1)) << 2);
                pDest[i] = (pDest[i] * aSlice[LEVEL_MUL]) + aSlice[LEVEL_ADD];
            }

        }

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const noexcept {
            float32 fFloor        = std::floor(fTime);
            int32   iPhase        = (int32)fFloor;
            float32 const* aSlice = afTransform + ((iPhase & (SIZE - 1)) << 2);
            float32 fInput        = fPeriodAdjust * (
                aSlice[PHASE_MUL] * (fTime - fFloor) + aSlice[PHASE_ADD]
            );
            return (poWaveform->value(fInput) * aSlice[LEVEL_MUL]) + aSlice[LEVEL_ADD];
        }

        /**
         * @inheritDoc
         */
        FixedShape getShape() const noexcept override {
            return IWaveform::XFORM;
        };

        /**
         * @inheritDoc
         */
        bool isDiscontinuous() const noexcept override {
            return true;
        }

        /**
         * @inheritDoc
         */
        bool isAperiodic() const noexcept override {
            return false;
        }

        /**
         * @inheritDoc
         */
        Ptr copy() {
            if constexpr (bMutable) {
                return Ptr(
                    new XForm<N, true>(
                        poWaveform->copy(),
                        afTransform
                    )
                );
            } else {
                return IWaveform::copy();
            }
        }
};

}

#endif
