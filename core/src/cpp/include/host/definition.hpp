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
 * This is the primary interface between the native host application and the MC64K virtual machine. The expectation
 * is that an instance of this class is statically configured and passed to the loader which uses it to validate
 * that the target is appropriate for the VM code and to ensure references to native data and vectors are set up.
 */
class Definition {

    private:
        const char*                     sHostName;
        Machine::Interpreter::HostCall* aHostVectors;
        Loader::StaticLinkSymbolSet     oExportSet;
        Loader::StaticLinkSymbolSet     oImportSet;
        Misc::Version                   oVersion;
        uint8                           uMaxVector;

    public:
        /**
         * aHostVectors is expected to be null terminated.
         * aExports and aImports must be terminated with a Symbol referencing null
         *
         * @param const char* sName
         * @param const Misc::Version oVersion
         * @param Machine::Interpreter::HostCall aVectors[]
         * @param Loader::LinkSymbol aExports[]
         * @param Loader::LinkSymbol aImports[]
         */
        Definition(
            const char* sName,
            const Misc::Version oVersion,
            Machine::Interpreter::HostCall aVectors[],
            const Loader::StaticLinkSymbolSet& oExport,
            const Loader::StaticLinkSymbolSet& oImport
        );

        /**
         * Get the name of the host application.
         *
         * @return const char*
         */
        const char* getName() const {
            return sHostName;
        }

        /**
         * Get the version of the host application.
         *
         * @return Misc::Version
         */
        Misc::Version getVersion() const {
            return oVersion;
        }

        Loader::LinkSymbolSet* getExportSet() {
            return &oExportSet;
        }

        Loader::LinkSymbolSet* getImportSet() {
            return &oImportSet;
        }
};

}} // namespace

#endif
