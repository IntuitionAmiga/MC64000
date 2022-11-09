#include <cstdio>
#include <cstdlib>
#include <mc64k.hpp>
#include <machine/timing.hpp>
#include <synth/note.hpp>
#include <synth/signal.hpp>
#include <synth/signal/operator/mixer.hpp>
#include <synth/signal/operator/automute.hpp>
#include <synth/signal/oscillator/LFO.hpp>
#include <synth/signal/oscillator/sound.hpp>
#include <synth/signal/envelope/decaypulse.hpp>
#include <synth/signal/envelope/shape.hpp>

using namespace MC64K::Machine;
using namespace MC64K::Synth::Audio;
using namespace MC64K::StandardTestHost::Audio::IConfig;

/**
 * Outputs a stream to a 16-bit raw file for a number of packets.
 */
void writeRawFile(Signal::IStream* pStream, char const* sName, size_t uPackets) {
    int16 aSamples[PACKET_SIZE];
    std::FILE* pRawFile = std::fopen(sName, "wb");
    if (pRawFile) {
        for (size_t uIndex = 0; uIndex < uPackets; ++uIndex) {
            auto pOutput = pStream->emit();
            for (unsigned i = 0; i < PACKET_SIZE; ++i) {
                aSamples[i] = (int16)(32000 * pOutput->aSamples[i]);
            }
            std::fwrite(aSamples, sizeof(int16), PACKET_SIZE, pRawFile);
        }
        std::fclose(pRawFile);
    }
}

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
        auto pInput    = Signal::Packet::create();
        auto pWaveform = Signal::IWaveform::get(eShape);
        auto pCopy     = pWaveform->copy();

        std::printf(
            "Testing Waveform Shape %d (%s)\n"
            "\tPeriod %f\n"
            "\tcopy() returns %s instance at %p\n",
            (int)eShape,
            aWaveNames[(int)eShape],
            pWaveform->getPeriod(),
            (pCopy.get() == pWaveform.get() ? "original" : "new"),
            pWaveform.get()
        );

        float32 fScale = pWaveform->getPeriod();
        float32 fStart = -fScale;
        fScale /= 128.0f;

        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            pInput->aSamples[u] = fStart + (float32)u * fScale;
        }

        auto pOutput = pWaveform->map(pInput);

        int16 aSamples[PACKET_SIZE];

        for (unsigned u = 0; u < PACKET_SIZE; ++u) {
            aSamples[u] = (int16)(32000.0 * pOutput->aSamples[u]);
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
                std::fwrite(aSamples, sizeof(int16), PACKET_SIZE, pRawFile);
            }
            std::fclose(pRawFile);
            std::printf("\tWrote 16-bit binary %s\n", sFilename);
        }

        std::printf("\tBenchmarking map() for 1M packets: ");
        Nanoseconds::Value uMark = Nanoseconds::mark();
        for (unsigned u = 0; u < 1000000; ++u) {
            auto pOutput = pWaveform->map(pInput);
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
    float64 fTimeGenerated = (float64)NUM_PACKETS * MC64K::StandardTestHost::Audio::IConfig::PACKET_PERIOD;
    std::printf(
        "\nTotal time %.3f [%.3f Packets/s] for %.2f seconds generated [%.2f x realtime]\n",
        fSeconds,
        fPacketsPerSecond,
        fTimeGenerated,
        fTimeGenerated / fSeconds
    );
}

void mixtest() {
    Signal::IStream::Ptr pStream1 (
        new Signal::Oscillator::Sound(
            Signal::IWaveform::get(Signal::IWaveform::SINE),
            220.0f,
            0.0f
        )
    );
    pStream1->enable();

    Signal::IStream::Ptr pStream2 (
        new Signal::Oscillator::Sound(
            Signal::IWaveform::get(Signal::IWaveform::SINE),
            660.0f,
            0.0f
        )
    );
    pStream2->enable();

    Signal::IStream::Ptr pStream3 (
        new Signal::Oscillator::Sound(
            Signal::IWaveform::get(Signal::IWaveform::SINE),
            440.0f,
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

    Signal::Operator::SimpleMixer oMix(1.0f);

    oMix.addInputStream(
        0xdeadbeef,
        pStream1,
        0.8f
    );

    oMix.addInputStream(
        0xabadafe,
        pStream2,
        0.1f
    );

    oMix.addInputStream(
        0x69696969,
        pStream3,
        0.1f
    );

    oMix.enable();

    writeRawFile(&oMix, "mix_test.raw", 1000);
}

/**
 * Main program
 */
int main(int const iArgCount, char const** aiArgVal) {

    Signal::Envelope::Shape oShape(
        0.0f, // start at zero
        {
            {1.0f, 0.5f}, // increase to 1.0 after +0.5s
            {0.5f, 0.5f}, // decrease to 0.5 after +0.5s
            {0.0f, 2.0f}, // decrease to 0.0 after +2.0s
        } // total duration 3.0s
    );


    Signal::Packet::dumpStats();

    return EXIT_SUCCESS;
}
