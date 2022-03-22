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
#include <host/standard_test_host_audio.hpp>
#include <host/audio/output.hpp>
#include <machine/register.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Audio {

void openAudio() {
    PackedParams oParams;
    oParams.u64 = Interpreter::gpr<ABI::INT_REG_0>().value<uint64>();

    std::printf(
        "openAudio rate:%d Hz, mode:%d resolution:%d\n",
        (int)oParams.u16[0],
        (int)oParams.u8[2],
        (int)oParams.u8[3]
    );
}

void closeAudio() {
    Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>();
    if (!poContext || !poContext->poOutputDevice) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
        return;
    }
}

void writeAudio() {
    Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>();
    if (!poContext || !poContext->poOutputDevice) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
        return;
    }
}

/**
 * Display::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {
    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {
        case INIT:
        case DONE:
            break;
        case OPEN:  openAudio();  break;
        case CLOSE: closeAudio(); break;
        case WRITE: writeAudio(); break;
        default:
            std::fprintf(stderr, "Unknown Audio operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

} // namespace

