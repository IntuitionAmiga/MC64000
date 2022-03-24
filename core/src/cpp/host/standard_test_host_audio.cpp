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
#include <new>
#include <cstdio>
#include <host/standard_test_host_mem.hpp>
#include <host/standard_test_host_audio.hpp>
#include <host/audio/output.hpp>
#include <machine/register.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Audio {

void openAudio() {
    PackedParams oParams;
    oParams.u64 = Interpreter::gpr<ABI::INT_REG_0>().value<uint64>();

    uint16 uRateHz   = oParams.u16[0];
    uint16 uBufferMs = oParams.u16[1];
    uint8  uMode     = oParams.u8[4];
    uint8  uFormat   = oParams.u8[5];

    std::printf(
        "openAudio Requested rate:%d Hz, buffer:%d ms, mode:%d format:%d\n",
        (int)uRateHz,
        (int)uBufferMs,
        (int)uMode,
        (int)uFormat
    );

    if (uRateHz < Output::MIN_HZ || uRateHz > Output::MAX_HZ) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_HZ;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    if (uBufferMs < Output::MIN_MS || uBufferMs > Output::MAX_MS) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_BUF;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    if (uMode < Output::CH_MONO || uMode > Output::CH_STEREO) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_CHN;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    if (uFormat < Output::INT_8 || uFormat > Output::INT_16) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_FMT;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    try {
        // Obtain the appropriate manager for the host. This should use RAII semantics
        OutputPCMDevice* poDevice = createOutputPCMDevice(
            uRateHz,
            uBufferMs,
            (Output::ChannelMode)uMode,
            (Output::Format)uFormat
        );
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = poDevice->getContext();
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NONE;
    } catch (Error& roError) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = Mem::ERR_NO_MEM;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
    } catch (std::bad_alloc&) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = Mem::ERR_NO_MEM;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
    }
}

void closeAudio() {
    Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>();
    if (!poContext || !poContext->poOutputDevice) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
        return;
    } else {
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        delete poContext->poOutputDevice;
    }
}

void writeAudio() {
    Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>();
    if (!poContext || !poContext->poOutputDevice) {

        std::fprintf(
            stderr,
            "Null audio context\n"
        );

        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
        return;
    } else {

        size_t uLength = poContext->uSubmitLength ?
            poContext->uSubmitLength :
            poContext->uBufferLength;

//         std::fprintf(
//             stderr,
//             "Writing %zu samples...\n",
//             uLength
//         );

        poContext->poOutputDevice->write(
            poContext->oBuffer.pAny,
            uLength
        );

        poContext->uSubmitLength = 0;
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

