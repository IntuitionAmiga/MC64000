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
#include <cstring>
#include <host/standard_test_host_display.hpp>
#include <machine/register.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Display {

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
        for (int i = 0; i < 100; ++i) {
            Interpreter::setProgramCounter(puFrameHookByteCode);
            Interpreter::run();
        }
        return Interpreter::INITIALISED;
    }
    return Interpreter::INVALID_ENTRYPOINT;
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

