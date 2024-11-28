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

#include <cstdio>

#include <synth/note.hpp>
#include <synth/machine/tbnan.hpp>

namespace MC64K::Synth::Audio::Machine {

using Audio::Note;
using Signal::IWaveform;
using Signal::Filter::FourPoleMultiMode;

TBNaN::TBNaN():
    Monophonic(),
    oOscillator{IWaveform::get(Signal::IWaveform::TRIANGLE)},
    oAEG{1.0f, TBNaN::DEFAULT_AEG_DECAY_RATE},
    oFilter{
        IStream::NONE,
        FourPoleMultiMode::LOW_PASS,
        TBNaN::DEFAULT_CUTOFF,
        TBNaN::DEFAULT_RESONANCE
    },
    oFEG{1.0f, TBNaN::DEFAULT_FEG_DECAY_RATE},
    oPWMLFO{IWaveform::get(IWaveform::SINE)},
    oPWMWave{IStream::NONE, 0.5f}
{
    oOscillator.setLevelEnvelope(oAEG);
    oFilter.setInputStream(oOscillator);
    oFilter.setCutoffEnvelope(oFEG);
    oPWMLFO.enable();
    oPWMWave.setModulator(oPWMLFO);

    oOscillator.setWaveformUnmanaged(oPWMWave);

    setVoiceSource(oFilter, 1.0f);
    std::fprintf(stderr, "Constructed TBNaN at %p\n", this);
}

TBNaN::~TBNaN() {
    std::fprintf(stderr, "Destroyed TBNaN at %p\n", this);
}

TBNaN* TBNaN::setVoiceNote(Voice eVoice, uint32 uNote) noexcept {
    oOscillator.setFrequency(Note::getFrequency(uNote));
    return this;
}

TBNaN* TBNaN::startVoice(Voice eVoice) noexcept {
    std::fprintf(stderr, "TBNaN::startVoice()\n");
    oVoice.reset();
    oVoice.enable();
    return this;
}

TBNaN* TBNaN::stopVoice(Voice eVoice, bool bSoft) noexcept {
    std::fprintf(stderr, "TBNaN::stopVoice()\n");
    oVoice.disable();
    return this;
}


}
