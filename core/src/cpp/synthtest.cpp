#include <cstdio>
#include <cstdlib>
#include <mc64k.hpp>
#include <machine/timing.hpp>
#include <synth/signal.hpp>
#include <synth/signal/oscillator/LFO.hpp>
#include <synth/signal/oscillator/sound.hpp>


using namespace MC64K::Machine;
using namespace MC64K::Synth::Audio;

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
        auto pInput = Signal::Packet::create();
        auto pWaveform = Signal::IWaveform::get(eShape);

        auto pCopy = pWaveform->copy();
        if (pCopy.get() == pWaveform.get()) {
            std::printf("Waveform %d copy() returns original\n", (int)eShape);
        } else {
            std::printf("Waveform %d copy() returns clone\n", (int)eShape);
        }

        float32 fScale = pWaveform->getPeriod();
        float32 fStart = -fScale;
        fScale /= 128.0f;

        for (unsigned u=0; u < 256; ++u) {
            pInput->aSamples[u] = fStart + (float32)u * fScale;
        }

        auto pOutput = pWaveform->map(pInput);

        int16 aSamples[256];

//         for (unsigned u=0; u < 256; ++u) {
//             std::printf(
//                 "%3u : %.10f => %.10f\n",
//                 u,
//                 pInput->aSamples[u],
//                 pOutput->aSamples[u]
//             );
//             aSamples[u] = (int16)(32000 * pOutput->aSamples[u]);
//         }

        std::snprintf(
            aFileName,
            sizeof(aFileName) - 1,
            "wave_%d.raw",
            (int32)eShape
        );
        std::FILE* pRawFile = std::fopen(aFileName, "wb");
        if (pRawFile) {
            for (int i=0; i<100; ++i) {
                std::fwrite(aSamples, sizeof(int16), 256, pRawFile);
            }
            std::fclose(pRawFile);
            std::printf("Wrote 16-bit binary %s\n", aFileName);
        }

        std::puts("Benchmarking map()...");

        Nanoseconds::Value uMark = Nanoseconds::mark();

        for (unsigned u=0; u < 10000000; ++u) {
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

int main(int const iArgCount, char const** aiArgVal) {

    Signal::Oscillator::Sound oOsc(
        Signal::IWaveform::get(Signal::IWaveform::SAW_DOWN),
        220.0f,
        0.0f
    );
    Signal::IStream::Ptr pModulator(
        new Signal::Oscillator::Sound(
            Signal::IWaveform::get(Signal::IWaveform::SINE),
            220.0f,
            0.0f
        )
    );
    pModulator->enable();
    oOsc.setPhaseModulator(pModulator);
    oOsc.setPhaseModulationIndex(0.25f);
    oOsc.enable();

    //oOsc.setPhaseFeedbackIndex(0.25f);

    int16 aSamples[256];
    std::FILE* pRawFile = std::fopen("osc_test.raw", "wb");
    if (pRawFile) {
        for (unsigned uIndex = 1; uIndex < 1000; ++uIndex) {
            auto pOutput = oOsc.emit(uIndex);

            //std::printf("Index %u: Position %lu\n", uIndex, oOsc.getPosition());

            for (unsigned i=0; i < 256; ++i) {
                aSamples[i] = (int16)(32000 * pOutput->aSamples[i]);
            }
            std::fwrite(aSamples, sizeof(int16), 256, pRawFile);
        }
        std::fclose(pRawFile);
    }
    Signal::Packet::dumpStats();

    return EXIT_SUCCESS;
}
