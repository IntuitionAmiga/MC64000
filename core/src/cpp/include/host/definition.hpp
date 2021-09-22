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
#include <initializer_list>

#include "machine/interpreter.hpp"
#include "misc/version.hpp"
#include "loader/symbol.hpp"

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
        char const*                      sHostName;
        Machine::Interpreter::HCFVector* pcHCFVectors;
        Loader::InitialisedSymbolSet     oExportSet;
        Loader::InitialisedSymbolSet     oImportSet;
        Misc::Version                    oVersion;
        uint32                           uNumHCFVectors;

    public:
        /**
         * Constructor. Uses a set of initializer_list for the vectors, export symbols and import symbols.
         * The data in these are copied to the instance internally and are not modified.
         *
         * Note that the free-form list initialisers may be subject to length limits. For example, if more than
         * 256 HCFVectors are defined, the code will fail an assertion check.
         *
         * @param char const* sName
         * @param Misc::Version const oVersion
         * @param std::initializer_list<Machine::Interpreter::HCFVector> const& roHCFVectors,
         * @param std::initializer_list<Loader::Symbol> const& roExportedSymbols,
         * @param std::initializer_list<Loader::Symbol> const& roImportedSymbols
         */
        Definition(
            char const* sName,
            Misc::Version const oVersion,
            std::initializer_list<Machine::Interpreter::HCFVector> const& roHCFVectors,
            std::initializer_list<Loader::Symbol> const& roExportedSymbols,
            std::initializer_list<Loader::Symbol> const& roImportedSymbols
        );

        /**
         * Destructor
         */
        ~Definition();

        /**
         * Get the name of the host application.
         *
         * @return char const*
         */
        char const* getName() const {
            return sHostName;
        }

        /**
         * Get the version of the host application.
         *
         * @return Misc::Version const
         */
        Misc::Version const getVersion() const {
            return oVersion;
        }

        /**
         * Get the symbols exported by the host definition
         *
         * @return Loader::SymbolSet const&
         */
        Loader::SymbolSet const& getExportedSymbolSet() const {
            return oExportSet;
        }

        /**
         * Get the symbols host expects to import.
         *
         * @return Loader::SymbolSet const&
         */
        Loader::SymbolSet const& getImportedSymbolSet() const {
            return oImportSet;
        }

        /**
         * @return Machine::Interpreter::HCFVector const*
         */
        Machine::Interpreter::HCFVector const* getHCFVectors() const {
            return pcHCFVectors;
        }

        /**
         * @return uint32
         */
        uint32 getNumHCFVectors() const {
            return uNumHCFVectors;
        }
};

}} // namespace

#endif
