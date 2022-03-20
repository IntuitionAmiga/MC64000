#ifndef MC64K_LOADER_EXECUTABLE_HPP
    #define MC64K_LOADER_EXECUTABLE_HPP

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
#include <misc/scalar.hpp>
#include <machine/limits.hpp>
#include "dependency.hpp"
#include "binary.hpp"

namespace MC64K::Loader {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Executable
 *
 * End product of successfully loading an MC64K object file for execution.
 */
class Executable {
    friend Executable const* Binary::load(char const*);

    private:
        // Symbols are managed by SymbolSet.
        LoadedSymbolSet oImportedSymbols;
        LoadedSymbolSet oExportedSymbols;

        uint8 const* puTargetData;
        uint8 const* puByteCode;

        enum {
            TD_OFFSET_FLAGS    = 0,
            TD_OFFSET_STACK    = 4,
            TD_OFFSET_NUM_DEPS = 8
        };

    public:

        /**
         * Obtain the set of imported symbols, i.e. those the executable expects to be provided to it.
         *
         * @return SymbolSet const*
         */
        SymbolSet const* getImportedSymbolSet() const {
            return &oImportedSymbols;
        }

        /**
         * Obtain the set of exported symbols, i.e. those the executable exposes to the application host.
         *
         * @return SymbolSet const*
         */
        SymbolSet const* getExportedSymbolSet() const {
            return &oExportedSymbols;
        }

        /**
         * Get the stack size indicated by the executable
         */
        uint32 getStackSize() const;

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
         * @param Host::Definition const& roDefinition
         * @param uint8 const*            puRawTargetData
         * @param uint8 const*            puRawByteCode
         * @param uint8*                  puRawImportData
         * @param uint8*                  puRawExportData
         */
        Executable(
            Host::Definition const& roDefinition,
            uint8 const*            puRawTargetData,
            uint8 const*            puRawByteCode,
            uint8*                  puRawImportData,
            uint8*                  puRawExportData
        );

        /**
         * Extracts the symbol access flags from the end of the symbol name and returns
         * the starting address of the next one. In the process, the access flags in the
         * string data are converted to a null so that the name can be used as a regular
         * C string. Thus, this function modifies the string passed.
         *
         * @param  char*   sSymbolName
         * @param  uint64& ruSymbolFlags
         * @return char*
         */
        char* processSymbolName(char * sSymbolName, uint64 & ruSymbolFlags);
};

} // namespace
#endif
