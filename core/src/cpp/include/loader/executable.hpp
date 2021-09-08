#ifndef __MC64K_LOADER_EXECUTABLE_HPP__
#   define __MC64K_LOADER_EXECUTABLE_HPP__

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
#include "misc/scalar.hpp"
#include "dependency.hpp"
#include "binary.hpp"

namespace MC64K {
namespace Loader {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Executable
 */
class Executable {
    friend const Executable* Binary::load(const char*);

    private:
        const uint8* pTargetData;
        const uint8* pImportData;
        const uint8* pExportData;
        const uint8* pByteCode;
        Dependency*  pDependencies;
        LinkSymbol*  pImportedSymbols;
        LinkSymbol*  pExportedSymbols;
        uint32       uNumDependencies;
        uint32       uNumImportedSymbols;
        uint32       uNumExportedSymbols;

    public:

        /**
         * Get the count of the dependencies.
         */
        uint32 getNumDependencies() const {
            return uNumDependencies;
        }

        /**
         * Get the set of named dependencies.
         */
        const Dependency* getDependencies() const {
            return pDependencies;
        }

        /**
         * Get the number of symbols exported by this binary.
         */
        uint32 getNumExportedSymbols() const {
            return uNumExportedSymbols;
        }

        /**
         * Get the symbols exported by this binary. They are assumed immutable from the host side.
         */
        const LinkSymbol* getExportedSymbols() const {
            return pExportedSymbols;
        }

        /**
         * Get the number of symbols imported by this binary.
         */
        uint32 getNumImportedSymbols() const {
            return uNumImportedSymbols;
        }

        /**
         * Get the symbols imported by this binary. The host is expected to match the identifiers with
         * appropriate runtime addresses for the expected data.
         */
        LinkSymbol* getImportedSymbols() const {
            return pImportedSymbols;
        }

        ~Executable();

    private:
        /**
         * Constructable only by the binary loader
         */
        Executable(
            const uint8* pRawTargetData,
            const uint8* pRawImportData,
            const uint8* pRawExportData,
            const uint8* pRawByteCode
        );

};

}} // namespace
#endif
