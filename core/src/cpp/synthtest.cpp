#include <cstdio>
#include <cstdlib>
#include <mc64k.hpp>
#include <machine/timing.hpp>
#include <synth/note.hpp>
#include <synth/signal.hpp>
#include <synth/signal/oscillator/LFO.hpp>
#include <synth/signal/oscillator/sound.hpp>


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
        Signal::IWaveform::SAW_UP,
        Signal::IWaveform::SAW_DOWN,
        Signal::IWaveform::SQUARE,
        Signal::IWaveform::PULSE_10,
        Signal::IWaveform::PULSE_20,
        Signal::IWaveform::PULSE_30,
        Signal::IWaveform::PULSE_40,
    };

    char aFileName[32] = "\0";

    for (auto eShape : aWaveShapes) {
        auto pInput    = Signal::Packet::create();
        auto pWaveform = Signal::IWaveform::get(eShape);
        auto pCopy     = pWaveform->copy();
        if (pCopy.get() == pWaveform.get()) {
            std::printf("Waveform %d copy() returns original\n", (int)eShape);
        } else {
            std::printf("Waveform %d copy() returns clone\n", (int)eShape);
        }

        float32 fScale = pWaveform->getPeriod();
        float32 fStart = -fScale;
        fScale /= 128.0f;

        for (unsigned u=0; u < PACKET_SIZE; ++u) {
            pInput->aSamples[u] = fStart + (float32)u * fScale;
        }

        auto pOutput = pWaveform->map(pInput);

        int16 aSamples[PACKET_SIZE];
        std::snprintf(
            aFileName,
            sizeof(aFileName) - 1,
            "wave_%d.raw",
            (int32)eShape
        );
        std::FILE* pRawFile = std::fopen(aFileName, "wb");
        if (pRawFile) {
            for (unsigned i = 0; i < 1000; ++i) {
                std::fwrite(aSamples, sizeof(int16), PACKET_SIZE, pRawFile);
            }
            std::fclose(pRawFile);
            std::printf("Wrote 16-bit binary %s\n", aFileName);
        }

        std::puts("Benchmarking map() performance...");
        Nanoseconds::Value uMark = Nanoseconds::mark();
        for (unsigned u = 0; u < 10000000; ++u) {
            auto pOutput = pWaveform->map(pInput);
        }
        uMark = Nanoseconds::mark() - uMark;
        float64 fMegaPacketsPerSecond = 1.e9 / (float64)uMark;
        std::printf(
            "\nShape %d : Total time %lu ns [%.2f Mpackets / s]\n",
            (int)eShape,
            uMark,
            fMegaPacketsPerSecond
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

/**
 * Main program
 */
int main(int const iArgCount, char const** aiArgVal) {

    Signal::Oscillator::Sound oOsc(
        Signal::IWaveform::get(Signal::IWaveform::TX81Z_5),
        220.0f,
        0.0f
    );
    oOsc.enable();

    //benchmark(&oOsc);

//     Signal::IStream::Ptr pModulator(
//         new Signal::Oscillator::LFOZeroToOne(
//             Signal::IWaveform::get(Signal::IWaveform::SAW_UP),
//             0.5f,
//             48.0f
//         )
//     );
//     pModulator->enable();
//     oOsc.setPitchModulator(pModulator);

    writeRawFile(&oOsc, "osc_test.raw", 5000);

    Signal::Packet::dumpStats();

    return EXIT_SUCCESS;
}
