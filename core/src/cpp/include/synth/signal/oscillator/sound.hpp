#ifndef MC64K_SYNTH_SIGNAL_OSC_SOUND_HPP
    #define MC64K_SYNTH_SIGNAL_OSC_SOUND_HPP

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

class Sound : public IOscillator {

    public:
        static constexpr float32 const INV_TWELVETH  = (float32)(1.0/12.0);
        static constexpr float32 const MIN_FREQUENCY = 6.875f;
        static constexpr float32 const DEF_FREQUENCY = 440.0f;   // A4
        static constexpr float32 const MAX_FREQUENCY = 14080.0f; // A9

        static constexpr float32 const MIN_PHASE_MOD_INDEX    = 0.01f;
        static constexpr float32 const MIN_FEEDBACK_MOD_INDEX = 0.01f;
        static constexpr float32 const FEEDBACK_SCALE         = 0.75f;
        static constexpr float32 const MIN_LEVEL_MOD_INDEX    = 0.01f;

        enum Antialias {
            AA_OFF  = 0,
            AA_ON   = 1,
            AA_AUTO = 2,
        };

    protected:
        IStream::Ptr oPitchModulatorPtr;
        IStream::Ptr oPhaseModulatorPtr;
        IStream::Ptr oLevelModulatorPtr;

        IStream*     poPitchModulator;
        IStream*     poPhaseModulator;
        IStream*     poLevelModulator;

        IEnvelope::Ptr oPitchEnvelopePtr;
        IEnvelope::Ptr oLevelEnvelopePtr;

        IStream*       poPitchEnvelope;
        IStream*       poLevelEnvelope;

        float32 fPhaseModulationIndex = 1.0;
        float32 fLevelModulationIndex = 1.0;

        // Self modulation
        float32 fPhaseFeedbackIndex   = 0.0;
        float32 fFeedback1            = 0.0;
        float32 fFeedback2            = 0.0;

        // Antialias smoothing
        float32 fAAPrev1              = 0.0;
        float32 fAAPrev2              = 0.0;
        float32 fAAPrev3              = 0.0;
        float32 fAAPrev4              = 0.0;

        Antialias eAAMode    = AA_AUTO;
        bool      bAntialias = false;

        /**
         * @inheritDoc
         */
        float32 clampFrequency(float32 fFrequency) const override;

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emitNew() override;

        void configureAntialias();

    public:
        Sound(
            IWaveform::Ptr const& roWaveformPtr,
            float32 fFrequency = DEF_FREQUENCY,
            float32 fPhase     = 0.0f
        );

        ~Sound();

        /**
         * @inheritDoc
         */
        Sound* reset() override;

        /**
         * @inheritDoc
         */
        Sound* enable() override;

        /**
         * Set the antialias mode explicitly
         */
        Sound* setAntialiasMode(Antialias eMode) {
            eAAMode = eMode;
            configureAntialias();
            return this;
        }

        /**
         * Set a Pitch Modulator, e.g. LFO (though can be any stream)
         *
         * Hardwired version.
         */
        Sound* setPitchModulator(IStream& roModulator) {
            poPitchModulator   = &roModulator;
            configureInputStage();
            return this;
        }

        /**
         * Set a Pitch Modulator, e.g. LFO (though can be any stream)
         */
        Sound* setPitchModulator(IStream::Ptr const& roModulatorPtr) {
            oPitchModulatorPtr = roModulatorPtr;
            poPitchModulator   = roModulatorPtr.get();
            configureInputStage();
            return this;
        }

        /**
         * Set a Phase Modulator, e.g. another oscillator (though can be any stream)
         *
         * Hardwired version
         */
        Sound* setPhaseModulator(IStream& roModulator) {
            poPhaseModulator   = &roModulator;
            configureInputStage();
            return this;
        }

        /**
         * Set a Phase Modulator, e.g. another oscillator (though can be any stream)
         */
        Sound* setPhaseModulator(IStream::Ptr const& roModulatorPtr) {
            oPhaseModulatorPtr = roModulatorPtr;
            poPhaseModulator   = roModulatorPtr.get();
            configureInputStage();
            return this;
        }

        /**
         * Set strenghth of the phase modulator
         */
        Sound* setPhaseModulationIndex(float32 fIndex) {
            fPhaseModulationIndex = fIndex;
            configureInputStage();
            return this;
        }

        /**
         * Set a Level Modulator, e.g. another oscillator (though can be any stream)
         *
         * Hardwired version
         */
        Sound* setLevelModulator(IStream& roModulator) {
            poLevelModulator   = &roModulator;
            configureOutputStage();
            return this;
        }

        /**
         * Set a Level Modulator, e.g. another oscillator (though can be any stream)
         */
        Sound* setLevelModulator(IStream::Ptr const& roModulatorPtr) {
            oLevelModulatorPtr = roModulatorPtr;
            poLevelModulator   = roModulatorPtr.get();
            configureOutputStage();
            return this;
        }

        Sound* setLevelModulationIndex(float32 fIndex) {
            fLevelModulationIndex = fIndex;
            configureOutputStage();
            return this;
        }

        /**
         * Set a Pitch Envelope
         *
         * Hardwired version
         */
        Sound* setPitchEnvelope(IEnvelope& roEnvelope) {
            poPitchEnvelope   = &roEnvelope;
            configureInputStage();
            return this;
        }


        /**
         * Set a Pitch Envelope
         */
        Sound* setPitchEnvelope(IEnvelope::Ptr const& roEnvelopePtr) {
            oPitchEnvelopePtr = roEnvelopePtr;
            poPitchEnvelope   = roEnvelopePtr.get();
            configureInputStage();
            return this;
        }

        /**
         * Set a Level Envelope
         *
         * Hardwired version
         */
        Sound* setLevelEnvelope(IEnvelope& roEnvelope) {
            poLevelEnvelope   = &roEnvelope;
            configureOutputStage();
            return this;
        }

        /**
         * Set a Level Envelope
         */
        Sound* setLevelEnvelope(IEnvelope::Ptr const& roEnvelopePtr) {
            oLevelEnvelopePtr = roEnvelopePtr;
            poLevelEnvelope   = roEnvelopePtr.get();
            configureOutputStage();
            return this;
        }

        Sound* setPhaseFeedbackIndex(float32 fIndex) {
            fPhaseFeedbackIndex = fIndex;
            configureOutputStage();
            return this;
        }

        /**
         * @inheritDoc
         */
        Sound* setWaveform(IWaveform::Ptr const& roWaveformPtr);

    private:
        typedef void (*Stage)(Sound*);

        Stage cInput;
        Stage cOutput;
        static Stage aInputStages[8];
        static Stage aOutputStages[8];
        void configureInputStage();
        void configureOutputStage();

        void populatePitchShiftedPacket(Packet const* poPitchShifts);
        void populatePitchAndPhaseShiftedPacket(Packet const* poPitchShifts, Packet const* poPhaseShifts);
        void populateOutputPacketWithFeedback(Packet const* poLevelPacket);

        // Configuration optimised Input stages
        static void inputAperiodic(Sound* poOscillator);
        static void inputDirect(Sound* poOscillator);
        static void inputPitchMod(Sound* poOscillator);
        static void inputPitchEnv(Sound* poOscillator);
        static void inputPitchModEnv(Sound* poOscillator);
        static void inputPhaseMod(Sound* poOscillator);
        static void inputPhaseModPitchMod(Sound* poOscillator);
        static void inputPhaseModPitchEnv(Sound* poOscillator);
        static void inputPhaseModPitchModEnv(Sound* poOscillator);

        // Configuration optimised Output stages
        static void outputDirect(Sound* poOscillator);
        static void outputLevelMod(Sound* poOscillator);
        static void outputLevelEnv(Sound* poOscillator);
        static void outputLevelModEnv(Sound* poOscillator);
        static void outputFeedback(Sound* poOscillator);
        static void outputFeedbackLevelMod(Sound* poOscillator);
        static void outputFeedbackLevelEnv(Sound* poOscillator);
        static void outputFeedbackLevelModEnv(Sound* poOscillator);
};

}

#endif
