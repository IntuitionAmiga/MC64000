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
 *
 * End product of successfully loading an MC64K object file for execution.
 */
class Executable {
    friend const Executable* Binary::load(const char*);

    private:
        // Symbols are managed by LinkSymbolSet.
        DynamicLinkSymbolSet oImportedSymbols;
        DynamicLinkSymbolSet oExportedSymbols;

        const uint8* pTargetData;
        const uint8* pByteCode;

    public:

        /**
         * Obtain the set of imported symbols, i.e. those the executable expects to be provided to it.
         *
         * @return const LinkSymbolSet*
         */
        const LinkSymbolSet* getImportedSymbolSet() const {
            return &oImportedSymbols;
        }

        /**
         * Obtain the set of exported symbols, i.e. those the executable exposes to the application host.
         *
         * @return const LinkSymbolSet*
         */
        const LinkSymbolSet* getExportedSymbolSet() const {
            return &oExportedSymbols;
        }

        /**
         * Destructor
         */
        ~Executable();

    private:
        /**
         * Constructor.
         *
         * Instantiable only by the binary loader friend class. Note that ownership of the raw memory referenced
         * by the target data and bytecode are taken over by this instance and are freed by it on destruction.
         *
         * @param const Host::Definition& oDefinition
         * @param const uint8* pRawTargetData
         * @param const uint8* pRawByteCode
         * @param uint8*       pRawImportData
         * @param uint8*       pRawExportData
         */
        Executable(
            const Host::Definition& oDefinition,
            const uint8*            pRawTargetData,
            const uint8*            pRawByteCode,
            uint8*                  pRawImportData,
            uint8*                  pRawExportData
        );

        /**
         * Extracts the symbol access flags from the end of the symbol name and returns
         * the starting address of the next one. In the process, the access flags in the
         * string data are converted to a null so that the name can be used as a regular
         * C string. Thus, this function modifies the string passed.
         *
         * @param  char*   sSymbolName
         * @param  uint64& uSymbolFlags
         * @return char*
         */
        char* processSymbolName(char* sSymbolName, uint64& uSymbolFlags);
};

}} // namespace
#endif
