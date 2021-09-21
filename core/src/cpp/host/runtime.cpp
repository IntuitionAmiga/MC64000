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
#include <cassert>
#include "host/runtime.hpp"
#include "loader/executable.hpp"

namespace MC64K {
namespace Host {

/**
 * @inheritDoc
 */
Runtime::Runtime(Definition& roDefinition, char const* sBinaryPath) :
    roDefinition(roDefinition),
    poExecutable(0)
{
    Loader::Binary oBinary(roDefinition);
    poExecutable = oBinary.load(sBinaryPath);

    std::fprintf(
        stderr,
        "Runtime: Executable instance loaded at %p for binary \'%s\'\n",
        poExecutable,
        sBinaryPath
    );

    // If the binary loaded without throwing stuff all over the shop, initialise the Interpreter
    Machine::Interpreter::allocateStack(poExecutable->getStackSize());
    Machine::Interpreter::initHCFVectors(
        roDefinition.getHCFVectors(),
        roDefinition.getNumHCFVectors()
    );
}

/**
 * @inheritDoc
 */
Runtime::~Runtime() {
    Machine::Interpreter::dumpState(stdout, 0xFFFFFFFF);
    delete poExecutable;
    Machine::Interpreter::freeStack();
}

/**
 * @inheritDoc
 */
Machine::Interpreter::Status Runtime::invoke(size_t uFunctionID) {
    const Loader::SymbolSet& roInvokable = roDefinition.getImportedSymbolSet();
    assert(roInvokable[uFunctionID].uFlags & Loader::Symbol::EXECUTE);
    Machine::Interpreter::setProgramCounter(roInvokable[uFunctionID].puByteCode);
    Machine::Interpreter::run();
    return Machine::Interpreter::getStatus();
}

}} // namespace
