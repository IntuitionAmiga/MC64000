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
#include <machine/register.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Audio {

/**
 * Display::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {
    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {
        case INIT:
        case DONE:
            break;
        case OPEN:  break;
        case CLOSE: break;
        case WRITE: break;
        default:
            std::fprintf(stderr, "Unknown Audio operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

} // namespace

