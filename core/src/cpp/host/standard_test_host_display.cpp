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
#include <new>
#include <host/standard_test_host_mem.hpp>
#include <host/standard_test_host_display.hpp>
#include <host/display/context.hpp>
#include <machine/register.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Display {

char const* aFormatNames[] = {
    "8-bit CLUT",
    "32-bit ARGB",
};

Interpreter::Status runEventLoop() {
    Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>();
    if (!poContext || !poContext->poDevice) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
        return Interpreter::RUNNING;
    }
    poContext->poDevice->runEventLoop();
    return Interpreter::INITIALISED;
}

void updateDisplay() {
    Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>();
    if (!poContext || !poContext->poDevice) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ABI::ERR_NULL_PTR;
    }
    poContext->poDevice->updateDisplay();
}


void openDisplay() {
    OpenParams const* poParams = Interpreter::gpr<ABI::PTR_REG_0>().address<OpenParams>();

    std::printf(
        "openDisplay view:%d x %d, buffer:%d x %d, offsets:%d x %d, flags:%04X, fmt:%d, hz:%d\n",
        (int)poParams->uViewWidth,
        (int)poParams->uViewHeight,
        (int)poParams->uBufferWidth,
        (int)poParams->uBufferHeight,
        (int)poParams->uViewXOffset,
        (int)poParams->uViewYOffset,
        (unsigned)poParams->uFlags,
        (int)poParams->uPixelFormat,
        (int)poParams->uRateHz
    );

    if (poParams->uPixelFormat >= PXL_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>()    = ERR_INVALID_FMT;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    if (poParams->uViewWidth < WIDTH_MIN || poParams->uViewWidth > WIDTH_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_WIDTH;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    if (poParams->uBufferWidth > WIDTH_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_WIDTH;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    if (poParams->uViewHeight < HEIGHT_MIN || poParams->uViewHeight > HEIGHT_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_HEIGHT;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    if (poParams->uViewHeight < HEIGHT_MIN || poParams->uViewHeight > HEIGHT_MAX) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = ERR_INVALID_HEIGHT;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
        return;
    }

    // Create a sanitised copy of the paramters and apply any roundint or other
    // modification
    OpenParams oOpenParams = *poParams;
    oOpenParams.uViewWidth   = (uint16)((poParams->uViewWidth + WIDTH_ALIGN_MASK) & ~WIDTH_ALIGN_MASK);
    oOpenParams.uBufferWidth = poParams->uBufferWidth < oOpenParams.uViewWidth ?
        oOpenParams.uViewWidth :
        (uint16)((poParams->uBufferWidth + WIDTH_ALIGN_MASK) & ~WIDTH_ALIGN_MASK)
    ;

    oOpenParams.uBufferHeight = poParams->uBufferHeight < poParams->uViewHeight ?
        poParams->uViewHeight :
        poParams->uBufferHeight
    ;

    std::printf(
        "openDisplay aligned view:%d x %d, buffer:%d x %d\n",
        (int)oOpenParams.uViewWidth,
        (int)oOpenParams.uViewHeight,
        (int)oOpenParams.uBufferWidth,
        (int)oOpenParams.uBufferHeight
    );

    try {
        // Obtain the appropriate device for the host. This should use RAII semantics
        Device* poDevice = createDevice(oOpenParams);
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = poDevice->getContext();
    } catch (Error& roError) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = Mem::ERR_NO_MEM;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
    } catch (std::bad_alloc&) {
        Interpreter::gpr<ABI::INT_REG_0>().value<uint64>() = Mem::ERR_NO_MEM;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
    }
}

void closeDisplay() {
    if (Context* poContext = Interpreter::gpr<ABI::PTR_REG_0>().address<Context>()) {
        delete poContext->poDevice;
        poContext = nullptr;
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = nullptr;
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
        case UPDATE: updateDisplay(); break;
        default:
            std::fprintf(stderr, "Unknown Display operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

} // namespace

