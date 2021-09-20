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
#include <cstdlib>
#include "host/runtime.hpp"
#include "loader/executable.hpp"

namespace MC64K {
namespace Host {

/**
 * @inheritDoc
 */
Runtime::Runtime(Definition& roDefinition, const char* sBinaryPath) :
    roDefinition(roDefinition),
    poExecutable(0)
{
    std::puts(sBinaryPath);
}

/**
 * @inheritDoc
 */

Runtime::~Runtime() {
    delete poExecutable;
}

/**
 * @inheritDoc
 */
Machine::Interpreter::Status Runtime::invoke(size_t uFunctionID) {
    switch (uFunctionID) {
        default:
        return Machine::Interpreter::UNINITIALISED;
    }
}

}} // namespace
