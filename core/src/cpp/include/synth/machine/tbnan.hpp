#ifndef MC64K_SYNTH_MACHINE_TBNAN_HPP
    #define MC64K_SYNTH_MACHINE_TBNAN_HPP

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

#include "monophonic.hpp"
#include <synth/signal.hpp>
#include <synth/signal/waveform/square.hpp>
#include <synth/signal/oscillator/sound.hpp>
#include <synth/signal/oscillator/LFO.hpp>
#include <synth/signal/envelope/decaypulse.hpp>
#include <synth/signal/filter/4polemulti.hpp>

namespace MC64K::Synth::Audio::Machine {

class TBNaN : public Monophonic, public virtual TSimpleVelocity {

    private:
        // Hardwired for efficiency
        Signal::Oscillator::Sound         oOscillator;
        Signal::Envelope::DecayPulse      oAEG;
        Signal::Filter::FourPoleMultiMode oFilter;
        Signal::Envelope::DecayPulse      oFEG;
        Signal::Oscillator::LFO           oPWMLFO;
        Signal::Waveform::ModulatedPWM    oPWMWave;

    public:
        static constexpr float32 const DEFAULT_AEG_DECAY_RATE = 0.07f;
        static constexpr float32 const DEFAULT_FEG_DECAY_RATE = 0.05f;
        static constexpr float32 const DEFAULT_CUTOFF         = 1.0f;
        static constexpr float32 const DEFAULT_RESONANCE      = 0.7f;

        enum Waveform {
            SAW     = Signal::IWaveform::SAW_DOWN,
            SQUARE  = Signal::IWaveform::SQUARE,
            PWM     = Signal::IWaveform::PULSE,
        };

        TBNaN();
        ~TBNaN();

        TBNaN* setPWMLFORate(float32 fHertz) noexcept {
            oPWMLFO.setFrequency(fHertz);
            return this;
        }

        TBNaN* setPWMLFODepth(float32 fDepth) noexcept {
            oPWMLFO.setDepth(fDepth);
            return this;
        }

        TBNaN* setLevelDecay(float32 fHalflife) noexcept {
            oAEG.setHalflife(fHalflife);
            return this;
        }

        TBNaN* setLevelTarget(float32 fTarget) noexcept {
            oAEG.setTarget(fTarget);
            return this;
        }

        TBNaN* setCutoff(float32 fCutoff) noexcept {
            oFilter.setCutoff(fCutoff);
            return this;
        }

        TBNaN* setResonance(float32 fResonance) noexcept {
            oFilter.setResonance(fResonance);
            return this;
        }

        TBNaN* setCutoffDecay(float32 fHalflife) noexcept {
            oFEG.setHalflife(fHalflife);
            return this;
        }

        TBNaN* setCutoffTarget(float32 fTarget) noexcept {
            oFEG.setTarget(fTarget);
            return this;
        }

        TBNaN* setVoiceNote(Voice eVoice, uint32 uNote) noexcept override;
        TBNaN* startVoice(Voice eVoice) noexcept override;
        TBNaN* stopVoice(Voice eVoice, bool bSoft) noexcept override;


};

} // namespace

#endif
