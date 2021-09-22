#ifndef __MC64K_HOST_RUNTIME_HPP__
#define __MC64K_HOST_RUNTIME_HPP__

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
#include "definition.hpp"

namespace MC64K {
namespace Host {

/**
 * Runtime
 */
class Runtime {
    private:
        Definition& roDefinition;
        Loader::Executable const* poExecutable;

    public:
        /**
         * Constructor
         *
         * Requires the host definition and the name of a binary executable to load.
         */
        Runtime(Definition& roDefinition, char const* sBinaryPath);
        ~Runtime();

        /**
         * Invokes a virtual machien call. The interpreter will run the code until
         * it returns at the same stack depth of encounters an error. The ID
         * must match an expected enumerated function ID exported by the binary.
         *
         * @param  size_t const uFunctionID
         * @return Machine::Interpreter::Status
         */
        Machine::Interpreter::Status invoke(size_t const uFunctionID);
};

}} // namespace

#endif
