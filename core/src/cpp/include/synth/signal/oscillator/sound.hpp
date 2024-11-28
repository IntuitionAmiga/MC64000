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
        float32 clampFrequency(float32 fFrequency) const noexcept override;

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emitNew() noexcept override;

        void configureAntialias() noexcept;

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
        Sound* reset() noexcept override;

        /**
         * @inheritDoc
         */
        Sound* enable() noexcept override;

        /**
         * Set the antialias mode explicitly
         */
        Sound* setAntialiasMode(Antialias eMode) noexcept {
            eAAMode = eMode;
            configureAntialias();
            return this;
        }

        /**
         * Set a Pitch Modulator, e.g. LFO (though can be any stream)
         *
         * Hardwired version.
         */
        Sound* setPitchModulator(IStream& roModulator) noexcept {
            poPitchModulator   = &roModulator;
            configureInputStage();
            return this;
        }

        /**
         * Set a Pitch Modulator, e.g. LFO (though can be any stream)
         */
        Sound* setPitchModulator(IStream::Ptr const& roModulatorPtr) noexcept {
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
        Sound* setPhaseModulator(IStream& roModulator) noexcept {
            poPhaseModulator   = &roModulator;
            configureInputStage();
            return this;
        }

        /**
         * Set a Phase Modulator, e.g. another oscillator (though can be any stream)
         */
        Sound* setPhaseModulator(IStream::Ptr const& roModulatorPtr) noexcept {
            oPhaseModulatorPtr = roModulatorPtr;
            poPhaseModulator   = roModulatorPtr.get();
            configureInputStage();
            return this;
        }

        /**
         * Set strenghth of the phase modulator
         */
        Sound* setPhaseModulationIndex(float32 fIndex) noexcept {
            fPhaseModulationIndex = fIndex;
            configureInputStage();
            return this;
        }

        /**
         * Set a Level Modulator, e.g. another oscillator (though can be any stream)
         *
         * Hardwired version
         */
        Sound* setLevelModulator(IStream& roModulator) noexcept {
            poLevelModulator   = &roModulator;
            configureOutputStage();
            return this;
        }

        /**
         * Set a Level Modulator, e.g. another oscillator (though can be any stream)
         */
        Sound* setLevelModulator(IStream::Ptr const& roModulatorPtr) noexcept {
            oLevelModulatorPtr = roModulatorPtr;
            poLevelModulator   = roModulatorPtr.get();
            configureOutputStage();
            return this;
        }

        Sound* setLevelModulationIndex(float32 fIndex) noexcept {
            fLevelModulationIndex = fIndex;
            configureOutputStage();
            return this;
        }

        /**
         * Set a Pitch Envelope
         *
         * Hardwired version
         */
        Sound* setPitchEnvelope(IEnvelope& roEnvelope) noexcept {
            poPitchEnvelope   = &roEnvelope;
            configureInputStage();
            return this;
        }


        /**
         * Set a Pitch Envelope
         */
        Sound* setPitchEnvelope(IEnvelope::Ptr const& roEnvelopePtr) noexcept {
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
        Sound* setLevelEnvelope(IEnvelope& roEnvelope) noexcept {
            poLevelEnvelope   = &roEnvelope;
            configureOutputStage();
            return this;
        }

        /**
         * Set a Level Envelope
         */
        Sound* setLevelEnvelope(IEnvelope::Ptr const& roEnvelopePtr) noexcept {
            oLevelEnvelopePtr = roEnvelopePtr;
            poLevelEnvelope   = roEnvelopePtr.get();
            configureOutputStage();
            return this;
        }

        Sound* setPhaseFeedbackIndex(float32 fIndex) noexcept {
            fPhaseFeedbackIndex = fIndex;
            configureOutputStage();
            return this;
        }

        /**
         * @inheritDoc
         */
        Sound* setWaveform(IWaveform::Ptr const& roWaveformPtr) noexcept {
            IOscillator::setWaveform(roWaveformPtr);
            return this;
        }

    private:
        typedef void (*Stage)(Sound*);

        Stage cInput;
        Stage cOutput;
        static Stage aInputStages[8];
        static Stage aOutputStages[8];
        void configureInputStage() noexcept;
        void configureOutputStage() noexcept;

        void populatePitchShiftedPacket(Packet const* poPitchShifts) noexcept;
        void populatePitchAndPhaseShiftedPacket(Packet const* poPitchShifts, Packet const* poPhaseShifts) noexcept;
        void populateOutputPacketWithFeedback(Packet const* poLevelPacket) noexcept;

        // Configuration optimised Input stages
        static void inputAperiodic(Sound* poOscillator) noexcept;
        static void inputDirect(Sound* poOscillator) noexcept;
        static void inputPitchMod(Sound* poOscillator) noexcept;
        static void inputPitchEnv(Sound* poOscillator) noexcept;
        static void inputPitchModEnv(Sound* poOscillator) noexcept;
        static void inputPhaseMod(Sound* poOscillator) noexcept;
        static void inputPhaseModPitchMod(Sound* poOscillator) noexcept;
        static void inputPhaseModPitchEnv(Sound* poOscillator) noexcept;
        static void inputPhaseModPitchModEnv(Sound* poOscillator) noexcept;

        // Configuration optimised Output stages
        static void outputDirect(Sound* poOscillator) noexcept;
        static void outputLevelMod(Sound* poOscillator) noexcept;
        static void outputLevelEnv(Sound* poOscillator) noexcept;
        static void outputLevelModEnv(Sound* poOscillator) noexcept;
        static void outputFeedback(Sound* poOscillator) noexcept;
        static void outputFeedbackLevelMod(Sound* poOscillator) noexcept;
        static void outputFeedbackLevelEnv(Sound* poOscillator) noexcept;
        static void outputFeedbackLevelModEnv(Sound* poOscillator) noexcept;
};

}

#endif
