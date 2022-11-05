#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVE_XFORM_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_WAVE_XFORM_HPP

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

namespace MC64K::Synth::Audio::Signal::Waveform {

/**
 * Transformer
 *
 * Divides an existing wave into 2<<N sequential slices that can have their phase and level
 * properties shifted and scaled. This can be used to produce many variants from simpler waveforms
 * with interesting harmonic modifications.
 */
template<size_t N>
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

        float32 aTransform[4 * SIZE] __attribute__ ((aligned (16))) = { };
        Ptr     pWaveform;
        float32 fPeriodAdjust;

    public:
        XForm(Ptr pSourceWaveform, float32 const* pCustomTransform):
            pWaveform(pSourceWaveform)
        {
            static_assert(N < 4, "Invalid size exponent for XForm");
            fPeriodAdjust = pWaveform->getPeriod() / PERIOD;
            if (pCustomTransform) {
                for (unsigned i = 0; i < T_SIZE; ++i) {
                    aTransform[i] = pCustomTransform[i];
                }
            }
            std::fprintf(stderr, "Created XForm<%u> at %p with matrix:\n", (uint32)N, this);
            for (unsigned i = 0; i < T_SIZE; i += 4) {
                std::fprintf(
                    stderr, "\t| %8.3f %8.3f %8.3f %8.3f |\n",
                    aTransform[i],
                    aTransform[i + 1],
                    aTransform[i + 2],
                    aTransform[i + 3]
                );
            }
        }

        ~XForm() {
            std::fprintf(stderr, "Destroyed XForm<%u> at %p\n", (uint32)N, this);
        }

        /**
         * @inheritDoc
         */
        float32 getPeriod() const {
            return PERIOD;
        }

        /**
         * @inheritDoc
         */
        Packet::Ptr map(Packet const* pInput) {
            Packet::Ptr    pReshaped = Packet::create();
            float32*       pDest     = pReshaped->aSamples;
            float32 const* pSrc      = pInput->aSamples;

            // Apply input phase modification
            for (unsigned i = 0; i < PACKET_SIZE; ++i) {
                float32 fVal          = pSrc[i];
                float32 fFloor        = std::floor(fVal);
                int32   iPhase        = (int32)fFloor;
                float32 const* aSlice = aTransform + ((iPhase & (SIZE - 1)) << 2);
                pDest[i] = fPeriodAdjust * (
                    aSlice[PHASE_MUL] * (fVal - fFloor) + aSlice[PHASE_ADD]
                );
            }

            pReshaped = pWaveform->map(pReshaped);

            // Apply the output amplitude modification
            pDest = pReshaped->aSamples;
            for (unsigned i = 0; i < PACKET_SIZE; ++i) {
                float32 const* aSlice = aTransform + ((((int32)std::floor(pSrc[i])) & (SIZE - 1)) << 2);
                pDest[i] = (pDest[i] * aSlice[LEVEL_MUL]) + aSlice[LEVEL_ADD];
            }
            return pReshaped;
        }

        /**
         * @inheritDoc
         */
        float32 value(float32 fTime) const {
            float32 fFloor        = std::floor(fTime);
            int32   iPhase        = (int32)fFloor;
            float32 const* aSlice = aTransform + ((iPhase & (SIZE - 1)) << 2);
            float32 fInput        = fPeriodAdjust * (
                aSlice[PHASE_MUL] * (fTime - fFloor) + aSlice[PHASE_ADD]
            );
            return (pWaveform->value(fInput) * aSlice[LEVEL_MUL]) + aSlice[LEVEL_ADD];
        }

        /**
         * Returns the enumerated shape identifier for the waveform.
         */
        FixedShape getShape() const {
            return IWaveform::XFORM;
        };

        /**
         * Returns whether or not the wave contains sharp discontinuities.
         */
        bool isDiscontinuous() const {
            return true;
        }

        /**
         * @inheritDoc
         */
        Ptr copy() {
            return Ptr(
                new XForm<N>(
                    pWaveform->copy(),
                    aTransform
                )
            );
        }
};

typedef XForm<0> XForm2;
typedef XForm<1> XForm4;
typedef XForm<2> XForm8;

}

#endif
