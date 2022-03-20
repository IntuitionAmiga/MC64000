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
#include <cstdlib>

#include "host/standard_test_host.hpp"
#include "host/runtime.hpp"
#include "loader/error.hpp"

#include "host/audio/config.hpp"
#include "host/audio/output.hpp"

#include <cmath>
using namespace MC64K::StandardTestHost::Audio;

/**
 * Entry point
 */
int main(int iArgN, char const** aArgV) {

    int16 *b = new int16[IConfig::PACKET_SIZE];

    float32 f = (float32)(4.0*M_PI / (float32)IConfig::PACKET_SIZE);

    for (unsigned u=0; u < IConfig::PACKET_SIZE; u++) {
        b[u] = (int16)(4096.0 * std::sin(f * (float32)u));
    }

    std::fprintf(stderr, "Allocated 16-bit buffer at %p, filled with sine\n", b);

    auto *p = createOutputPCMDevice(
        OutputPCMDevice::CH_MONO,
        OutputPCMDevice::RES_16_BIT,
        48000
    );

    std::fprintf(stderr, "Attempting playback. Mind your ears...\n");

    int const MAX_PACKETS = 1<<11;

    for (int i = 0; i < MAX_PACKETS; ) {
        p->write(b, IConfig::PACKET_SIZE);
        std::fprintf(stderr, "\r         \r%d packets written", ++i);
    }

    std::fprintf(stderr, "\nWrote %d packets...\n", MAX_PACKETS);

    delete p;

    delete[] b;

    return 0;

    if (iArgN < 2) {
        std::puts("Missing required parameter");
        std::exit(EXIT_FAILURE);
    }

    try {
        char const* sExecutableName = aArgV[1];

        MC64K::StandardTestHost::setCLIParameters(iArgN, aArgV);
        MC64K::Host::Runtime oRuntime(MC64K::StandardTestHost::instance, sExecutableName);
        oRuntime.invoke(MC64K::StandardTestHost::ABI::MAIN);

    } catch (MC64K::Loader::Error& oError) {
        std::printf(
            "Unable to load binary file \"%s\", %s.\n",
            oError.sFileName,
            oError.sIssue
        );
        std::exit(EXIT_FAILURE);
    } catch (MC64K::Machine::Error& oError) {
        std::printf(
            "Machine error: %s.\n",
            oError.sIssue
        );
        std::exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
