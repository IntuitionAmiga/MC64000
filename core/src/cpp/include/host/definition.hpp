#ifndef __MC64K_HOST_DEFINITION_HPP__
#define __MC64K_HOST_DEFINITION_HPP__

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

#include "machine/interpreter.hpp"
#include "misc/version.hpp"
#include "loader/linksymbol.hpp"

namespace MC64K {
namespace Host {

/**
 * Definition
 *
 * Basic Host Definition structure.
 */
class Definition {

    private:
        const char*                     sHostName;
        Machine::Interpreter::HostCall* aHostVectors;
        Loader::LinkSymbol*             aHostExportSymbols;
        Loader::LinkSymbol*             aHostImportSymbols;
        Misc::Version                   oVersion;
        uint8                           uMaxVector;

    public:
        /**
        * aHostVectors is expected to be null terminated.
        * aExports and aImports must be terminated with a Symbol referencing null
        */
        Definition(
            const char* sName,
            const Misc::Version oVersion,
            Machine::Interpreter::HostCall aVectors[],
            Loader::LinkSymbol aExports[],
            Loader::LinkSymbol aImports[]
        );

        const char* getName() const {
            return sHostName;
        }

        Misc::Version getVersion() const {
            return oVersion;
        }

        void setVector(Machine::Interpreter::HostCall cFunction, uint8 uIndex);

};

}} // namespace

#endif
