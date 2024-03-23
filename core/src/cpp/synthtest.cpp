#include <cstdio>
#include <cstdlib>
#include <mc64k.hpp>
#include <machine/timing.hpp>
#include <host/audio/output.hpp>
#include <synth/note.hpp>
#include <synth/signal.hpp>
#include <synth/signal/operator/leveladjust.hpp>
#include <synth/signal/operator/mixer.hpp>
#include <synth/signal/operator/automute.hpp>
#include <synth/signal/oscillator/LFO.hpp>
#include <synth/signal/oscillator/sound.hpp>
#include <synth/signal/envelope/decaypulse.hpp>
#include <synth/signal/envelope/shape.hpp>
#include <synth/signal/filter/4polemulti.hpp>

using namespace MC64K::Machine;
using namespace MC64K::Synth::Audio;
//using namespace MC64K::StandardTestHost::Audio::IConfig;
using namespace MC64K::StandardTestHost;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Outputs a stream to a 16-bit raw file for a number of packets.
 */
void writeRawFile(Signal::IStream* pStream, char const* sName, size_t uPackets) {
    int16 afSamples[Audio::IConfig::PACKET_SIZE];
    std::FILE* pRawFile = std::fopen(sName, "wb");
    if (pRawFile) {
        for (size_t uIndex = 0; uIndex < uPackets; ++uIndex) {
            auto pOutput = pStream->emit();
            for (unsigned i = 0; i < Audio::IConfig::PACKET_SIZE; ++i) {
                afSamples[i] = (int16)(32000 * pOutput->afSamples[i]);
            }
            std::fwrite(afSamples, sizeof(int16), Audio::IConfig::PACKET_SIZE, pRawFile);
        }
        std::fclose(pRawFile);
    }
}

void writeAudio(Signal::IStream* pStream, Audio::Context* poContext, size_t uPackets) {
    int16* afSamples = poContext->oBuffer.piWord;
    for (size_t uIndex = 0; uIndex < uPackets; ++uIndex) {
        auto pOutput = pStream->emit();
        for (unsigned i = 0; i < Audio::IConfig::PACKET_SIZE; ++i) {
            afSamples[i] = (int16)(32000 * pOutput->afSamples[i]);
        }
        poContext->poOutputDevice->write(
            poContext->oBuffer.pAny,
            Audio::IConfig::PACKET_SIZE
        );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Tests the standard waveforms
 */
void testWaveforms() {
    Signal::IWaveform::FixedShape aWaveShapes[] = {
        Signal::IWaveform::SINE,
        Signal::IWaveform::TRIANGLE,
        Signal::IWaveform::SAW_DOWN,
        Signal::IWaveform::SAW_UP,
        Signal::IWaveform::SQUARE,
        Signal::IWaveform::PULSE,
        Signal::IWaveform::PULSE_10,
        Signal::IWaveform::PULSE_20,
        Signal::IWaveform::PULSE_30,
        Signal::IWaveform::PULSE_40,
        Signal::IWaveform::PULSE_50,
        Signal::IWaveform::POKEY,
        Signal::IWaveform::NOISE,
        Signal::IWaveform::SINE_HALF_RECT,
        Signal::IWaveform::SINE_FULL_RECT,
        Signal::IWaveform::SINE_SAW,
        Signal::IWaveform::SINE_SAW_HARD, // todo
        Signal::IWaveform::SINE_PINCH,
        Signal::IWaveform::SINE_CUT, // todo
        Signal::IWaveform::TX81Z_1,
        Signal::IWaveform::TX81Z_2,
        Signal::IWaveform::TX81Z_3,
        Signal::IWaveform::TX81Z_4,
        Signal::IWaveform::TX81Z_5,
        Signal::IWaveform::TX81Z_6, // todo - need to change xform tool
        Signal::IWaveform::TX81Z_7,
        Signal::IWaveform::TX81Z_8, // todo - need to change xform tool
        Signal::IWaveform::TRIANGLE_HALF_RECT,
    };

    char const* aWaveNames[] = {
        "sine",
        "triangle",
        "saw_down",
        "saw_up",
        "square",
        "pulse",
        "pulse_10",
        "pulse_20",
        "pulse_30",
        "pulse_40",
        "pulse_50",
        "pokey",
        "noise",
        "sine_half_rect",
        "sine_full_rect",
        "sine_saw",
        "sine_saw_hard",
        "sine_pinch",
        "sine_cut",
        "tx81z_1",
        "tx81z_2",
        "tx81z_3",
        "tx81z_4",
        "tx81z_5",
        "tx81z_6",
        "tx81z_7",
        "tx81z_8",
        "triangle_half_rect"
    };

    char sFilename[80] = "\0";

    for (auto eShape : aWaveShapes) {
        Signal::Packet::Ptr poInput       = Signal::Packet::create();
        Signal::IWaveform::Ptr poWaveform = Signal::IWaveform::get(eShape);
        Signal::IWaveform::Ptr poCopy     = poWaveform->copy();

        std::printf(
            "Testing Waveform Shape %d (%s)\n"
            "\tPeriod %f\n"
            "\tcopy() returns %s instance at %p\n",
            (int)eShape,
            aWaveNames[(int)eShape],
            poWaveform->getPeriod(),
            (poCopy.get() == poWaveform.get() ? "original" : "new"),
            poWaveform.get()
        );

        float32 fScale = poWaveform->getPeriod();
        float32 fStart = -fScale;
        fScale /= 128.0f;

        for (unsigned u = 0; u < Audio::IConfig::PACKET_SIZE; ++u) {
            poInput->afSamples[u] = fStart + (float32)u * fScale;
        }

        Signal::Packet::Ptr pOutput = poWaveform->map(poInput);

        int16 afSamples[Audio::IConfig::PACKET_SIZE];

        for (unsigned u = 0; u < Audio::IConfig::PACKET_SIZE; ++u) {
            afSamples[u] = (int16)(32000.0 * pOutput->afSamples[u]);
        }


        std::snprintf(
            sFilename,
            sizeof(sFilename) - 1,
            "junk/wave_%d_%s.raw",
            (int32)eShape,
            aWaveNames[(int32)eShape]
        );
        std::FILE* pRawFile = std::fopen(sFilename, "wb");
        if (pRawFile) {
            std::printf("\tWriting %s...\n", sFilename);
            for (unsigned i = 0; i < 1000; ++i) {
                std::fwrite(afSamples, sizeof(int16), Audio::IConfig::PACKET_SIZE, pRawFile);
            }
            std::fclose(pRawFile);
            std::printf("\tWrote 16-bit binary %s\n", sFilename);
        }

        std::printf("\tBenchmarking map() for 1M packets: ");
        Nanoseconds::Value uMark = Nanoseconds::mark();
        for (unsigned u = 0; u < 1000000; ++u) {
            auto pOutput = poWaveform->map(poInput);
        }
        uMark = Nanoseconds::mark() - uMark;
        float64 fSeconds = 1.e-9 * (float64)uMark;
        std::printf(
            "Took %lu ns [%.2f Mpackets / s]\n\n",
            uMark,
            1.0 / fSeconds
        );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Outputs the frequency table versus the ideal values (for when approximations are in use)
 */
void freqTable() {
    for (int i = 0; i < 128; ++i) {
        float64 fIdeal = 440.0 * std::exp2((float32)(i-69) * 1.0f/12.0);
        float64 fValue = Note::getFrequency(i);
        std::printf(
            "Note %3d : %.3fHz [Ideal: %.3fHz, Diff: %.3fHz]\n",
            i,
            fValue,
            fIdeal,
            fValue - fIdeal
        );
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Benchmarks a stream by timing the generation of 10M packets
 */
void benchmark(Signal::IStream* pStream) {
    unsigned const NUM_PACKETS = 10000000;
    std::printf("Benchmarking %u packets...\n", NUM_PACKETS);
    Nanoseconds::Value uMark = Nanoseconds::mark();
    for (unsigned u=0; u < NUM_PACKETS; ++u) {
        auto pOutput = pStream->emit();
    }
    uMark = Nanoseconds::mark() - uMark;
    float64 fSeconds = 1.0e-9 * (float64)uMark;
    float64 fPacketsPerSecond = (float64)NUM_PACKETS / fSeconds;
    float64 fTimeGenerated = (float64)NUM_PACKETS * Audio::IConfig::PACKET_PERIOD;
    std::printf(
        "\nTotal time %.3f [%.3f Packets/s] for %.2f seconds generated [%.2f x realtime]\n",
        fSeconds,
        fPacketsPerSecond,
        fTimeGenerated,
        fTimeGenerated / fSeconds
    );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mixtest(Audio::Context* poContext) {
    Signal::IStream::Ptr pStream1 (
        new Signal::Oscillator::Sound(
            Signal::IWaveform::get(Signal::IWaveform::SINE),
            110.0f,
            0.0f
        )
    );
    pStream1->enable();
    Signal::IStream::Ptr pStream2 (
        new Signal::Oscillator::Sound(
            Signal::IWaveform::get(Signal::IWaveform::SINE),
            55.1f,
            0.0f
        )
    );
    pStream2->enable();
    Signal::IStream::Ptr pStream3 (
        new Signal::Oscillator::Sound(
            Signal::IWaveform::get(Signal::IWaveform::SINE),
            110.0f,
            0.0f
        )
    );
    pStream3->enable();
    std::reinterpret_pointer_cast<Signal::Oscillator::Sound>(pStream1)->setPhaseModulator(pStream3);
    Signal::IEnvelope::Ptr pEnv (
        new Signal::Envelope::DecayPulse(
            1.0f,
            0.5f
        )
    );
    std::reinterpret_pointer_cast<Signal::Oscillator::Sound>(pStream3)->setLevelEnvelope(pEnv);
    std::reinterpret_pointer_cast<Signal::Oscillator::Sound>(pStream2)->setLevelEnvelope(pEnv);
    std::reinterpret_pointer_cast<Signal::Oscillator::Sound>(pStream1)->setLevelEnvelope(pEnv);

    Signal::IStream::Ptr pMixer (
        new Signal::Operator::FixedMixer(3, 1.0f)
    );

    std::reinterpret_pointer_cast<Signal::Operator::FixedMixer>(pMixer)
        ->setChannel(
            0,
            pStream1,
            0.8f
        )
        ->setChannel(
            1,
            pStream2,
            0.1f
        )
        ->setChannel(
            2,
            pStream3,
            0.1f
        );

    pMixer->enable();

    Signal::Operator::LevelAdjust oAdjust(pMixer, 0.5f, 0.0f);

    oAdjust.enable();
    writeAudio(&oAdjust, poContext, 500);
    //writeRawFile(&oMix, "mix_test.raw", 1000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void testNotes() {

    char const* asGood[] = {
        "C-1", "C#-1",
        "Db-1", "D-1", "D#-1",
        "Eb-1", "E-1", "Fb-1",
        "E#-1", "F-1", "F#-1",
        "Gb-1", "G-1", "G#-1",
        "Ab-1", "A-1", "A#-1",
        "Bb-1", "B-1", "B#-1",

        "C0", "C#0",
        "Db0", "D0", "D#0",
        "Eb0", "E0", "Fb0",
        "E#0", "F0", "F#0",
        "Gb0", "G0", "G#0",
        "Ab0", "A0", "A#0",
        "Bb0", "B0", "B#0",

        "C4", "C#4",
        "Db4", "D4", "D#4",
        "Eb4", "E4", "Fb4",
        "E#4", "F4", "F#4",
        "Gb4", "G4", "G#4",
        "Ab4", "A4", "A#4",
        "Bb4", "B4", "B#4",

        "C9", "C#9",
        "Db9", "D9", "D#9",
        "Eb9", "E9", "Fb9",
        "E#9", "F9", "F#9",
        "Gb9", "G9", // 127
    };

    for (unsigned u = 0; u < sizeof(asGood)/sizeof(char const*); ++u) {
        std::printf("Note (good): %s => %u\n", asGood[u], Note::getNumber(asGood[u]));
    }

    char const* asBad[] = {
        0,
        "\0",
        "blah",
        "1",
        "C",
        "Cb",
        "B-2", // range
        "G#9", // range
    };

    for (unsigned u = 0; u < sizeof(asBad)/sizeof(char const*); ++u) {
        std::printf("Note (bad): %s => %u\n", asBad[u], Note::getNumber(asBad[u]));
    }

}


/**
 * Main program
 */
int main(int const iArgCount, char const** aiArgVal) {

    //testNotes();
    //mixtest();

    Audio::OutputPCMDevice* poOutput = Audio::createOutputPCMDevice(
        Audio::IConfig::PROCESS_RATE,
        50, // ms
        Audio::Output::CH_MONO,
        Audio::Output::INT_16
    );

    if (poOutput) {
        Audio::Context* poContext = poOutput->getContext();

        std::printf(
            "Allocated Audio::Context at %p\n",
            poContext
        );

        mixtest(poContext);

        delete poOutput;
    }
    Signal::Packet::dumpStats();

    testWaveforms();
    Signal::Packet::dumpStats();

    return EXIT_SUCCESS;
}
