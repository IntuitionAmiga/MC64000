#include <cstdio>
#include <cstdlib>
#include <mc64k.hpp>
#include <machine/timing.hpp>
#include <synth/signal.hpp>

using namespace MC64K::Machine;
using namespace MC64K::Synth::Audio;


int main(int const iArgCount, char const** aiArgVal) {

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
    {
        auto pInput = Signal::Packet::create();
        auto pWaveform = Signal::IWaveform::get(eShape);

        float32 fScale = pWaveform->getPeriod();
        float32 fStart = -fScale;
        fScale /= 128.0f;

        for (unsigned u=0; u < 256; ++u) {
            pInput->aSamples[u] = fStart + (float32)u * fScale;
        }

        auto pOutput = pWaveform->map(pInput);

        int16 aSamples[256];

        for (unsigned u=0; u < 256; ++u) {
            std::printf(
                "%3u : %.10f => %.10f\n",
                u,
                pInput->aSamples[u],
                pOutput->aSamples[u]
            );
            aSamples[u] = (int16)(32000 * pOutput->aSamples[u]);
        }

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
    }

//         Signal::IWaveform::Shape aWaveShapes[] = {
//             Signal::IWaveform::SINE,
//             Signal::IWaveform::TRIANGLE,
//             Signal::IWaveform::SAW,
//             Signal::IWaveform::SQUARE,
//             Signal::IWaveform::SINEX
//         };
//
//         for (auto eShape : aWaveShapes) {
//             auto pWaveform = Signal::IWaveform::get(eShape);
//
//             float32 fScale = pWaveform->getPeriod();
//             fScale /= 256.0f;
//
//             for (unsigned u=0; u < 256; ++u) {
//                 pInput->aSamples[u] = (float32)u * fScale;
//             }
//
//             Nanoseconds::Value uMark = Nanoseconds::mark();
//
//             for (unsigned u=0; u < 1000000; ++u) {
//                 auto pOutput = pWaveform->map(pInput);
//             }
//
//             uMark = Nanoseconds::mark() - uMark;
//
//             float64 fMegaPacketsPerSecond = 1.e9 / (float64)uMark;
//
//             std::printf(
//                 "\nShape %d : Total time %lu ns [%.2f Mpackets / s]\n",
//                 (int)eShape,
//                 uMark,
//                 fMegaPacketsPerSecond
//             );
//         }

    }

    Signal::Packet::dumpStats();

    return EXIT_SUCCESS;
}
