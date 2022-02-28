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
#include <cstring>
#include <host/standard_test_host_mem.hpp>
#include <host/standard_test_host_display.hpp>
#include <machine/register.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Display {


union PackedParams {
    uint64 u64;
    uint16 u16[4];
    uint8  u8[8];
};

union PixelPointer {
    void*   puAny;
    uint8*  puByte;
    uint16* puWord;
    uint32* puLong;
};

struct Context {
    PixelPointer pDisplayBuffer;
    uint32 uFlags;
    uint16 uWidth;
    uint16 uHeight;
    uint16 uHardwareWidth;
    uint16 uHardwareHeight;
    uint16 uPixelFormat;
};

char const* aFormatNames[] = {
    "8-bit CLUT",
    "32-bit ARGB",
};

const uint8 aPixelSize[] = {
    1,
    4
};

/**
 * This is just a dummy proof of concept implementation for the moment. The idea will be that this will
 * form the basis of a "swap display buffer and gather input" event loop from the host that invokes the
 * user-supplied code address.
 *
 * TODO - The current mechanism is low latency as it just sets the PC and runs with it. We should probaby
 * validate that the address matches a designated known function ID before allowing it to be called back to.
 * This can be done before entering the loop.
 */
Interpreter::Status runEventLoop() {

    uint8 const* volatile puFrameHookByteCode = Interpreter::gpr<ABI::PTR_REG_0>().address<uint8 const>();
    if (puFrameHookByteCode) {
        for (int i = 0; i < 5; ++i) {
            Interpreter::setProgramCounter(puFrameHookByteCode);
            Interpreter::run();
        }
        return Interpreter::INITIALISED;
    }
    return Interpreter::INVALID_ENTRYPOINT;
}

void openDisplay() {

    PackedParams oParams;

    oParams.u64 = Interpreter::gpr<ABI::INT_REG_0>().value<uint64>();

    uint16 uFormat = oParams.u16[0];
    uint16 uWidth  = oParams.u16[1];
    uint16 uHeight = oParams.u16[2];

    if (uFormat >= PF_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>()    = ERR_INVALID_FMT;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = 0;
        return;
    }

    if (uWidth < WIDTH_MIN || uWidth > WIDTH_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_WIDTH;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = 0;
        return;
    }

    if (uHeight < HEIGHT_MIN || uHeight > HEIGHT_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_HEIGHT;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = 0;
        return;
    }

    size_t uBufferSize = uWidth * uHeight * aPixelSize[uFormat] + sizeof(Context);

    if (void* pAllocation = std::malloc(uBufferSize)) {
        std::printf(
            "\nHCF: Open Display Requested %" PFU16 " x %" PFU16 " fmt: %s, size needed %zu bytes, allocated at %p\n",
            uWidth,
            uHeight,
            aFormatNames[uFormat],
            uBufferSize,
            pAllocation
        );
        Context *poContext = (Context*)pAllocation;
        poContext->pDisplayBuffer.puByte = ((uint8*)pAllocation) + sizeof(Context);
        poContext->uFlags                = 0;
        poContext->uHardwareWidth        = poContext->uWidth  = uWidth;
        poContext->uHardwareHeight       = poContext->uHeight = uHeight;
        poContext->uPixelFormat          = uFormat;
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NONE;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = pAllocation;
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = Mem::ERR_NO_MEM;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = 0;
    }
}

void closeDisplay() {
    if (Context* pContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>()) {
        std::printf(
            "\nHCF: Close Display at %p\n",
            pContext
        );
        std::free(pContext);
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = 0;
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NONE;
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
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
        case OPEN:  openDisplay();  break;
        case CLOSE: closeDisplay(); break;
        case BEGIN:
            return runEventLoop();
            break;
        default:
            std::fprintf(stderr, "Unknown Display operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

} // namespace

