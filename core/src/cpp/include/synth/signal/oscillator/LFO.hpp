#ifndef MC64K_SYNTH_SIGNAL_OSC_LFO_HPP
    #define MC64K_SYNTH_SIGNAL_OSC_LFO_HPP

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

namespace MC64K::Synth::Audio::Signal::Oscillator {

/**
 * Basic LFO producing a signal in the range -depth to +depth (depending on the waveform)
 */
class LFO : public IOscillator {
    public:
        static constexpr float32 const MIN_FREQUENCY = 1.0f/60.0f;
        static constexpr float32 const DEF_FREQUENCY = 2.0f;
        static constexpr float32 const MAX_FREQUENCY = 64.0f;

    protected:
        float32 fDepth;
        float32 fBias;
        bool    bRetrigger;

        /**
         * @inheritDoc
         */
        float32 clampFrequency(float32 fFrequency);

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emitNew();

        /**
         * Common waveform input packet generation
         *
         * @return Packet const*
         */
        Packet const* generateCommon();

    public:
        /**
         * @param IWaveform::Ptr pWaveform
         * @param float32        fFrequency
         * @param float32        fDepth
         * @param bool           bRetrigger
         */
        LFO(
            IWaveform::Ptr const& pWaveform,
            float32 fFrequency = 0.0f,
            float32 fDepth     = 0.5f,
            bool    bRetrigger = false
        );

        ~LFO();

        /**
         * Oscillator resets on retrigger
         *
         * @return this
         */
        LFO* enableRetrigger() {
            bRetrigger = true;
            return this;
        }

        /**
         * Oscillator does not reset on retrigger
         *
         * @return this
         */
        LFO* disableRetrigger() {
            bRetrigger = true;
            return this;
        }

        /**
         * @inheritDoc
         */
        LFO* reset() {
            if (bRetrigger) {
                IOscillator::reset();
            }
            return this;
        }

};

/**
 * Basic LFO producing a signal in the range 0.0 to depth (depending on the waveform)
 */
class LFOOneToZero : public LFO {

    protected:
        /**
         * @inheritDoc
         */
        Packet::ConstPtr emitNew();

    public:

        /**
         * @inheritDoc
         */
        LFOOneToZero(
            IWaveform::Ptr const& pWaveform,
            float32 fFrequency = 0.0f,
            float32 fDepth     = 0.5f,
            bool    bRetrigger = false
        ) : LFO(pWaveform, fFrequency, fDepth, bRetrigger) {

        }
};

/**
 * Basic LFO producing a signal in the range 1.0 to 1.0 - depth (depending on the waveform)
 */
class LFOZeroToOne : public LFO {

    protected:
        /**
         * @inheritDoc
         */
        Packet::ConstPtr emitNew();

    public:
        /**
         * @inheritDoc
         */
        LFOZeroToOne(
            IWaveform::Ptr const& pWaveform,
            float32 fFrequency = 0.0f,
            float32 fDepth     = 0.5f,
            bool    bRetrigger = false
        ) : LFO(pWaveform, fFrequency, fDepth, bRetrigger) {

        }
};


}

#endif
