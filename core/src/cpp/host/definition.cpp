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
#include "host/definition.hpp"
#include "machine/limits.hpp"

namespace MC64K {
namespace Host {

/**
 * @inheritDoc
 */
Definition::Definition(
    char const* sName,
    Misc::Version const oVersion,
    std::initializer_list<Machine::Interpreter::HCFVector> const& roHCFVectors,
    std::initializer_list<Loader::Symbol> const& roExportedSymbols,
    std::initializer_list<Loader::Symbol> const& roImportedSymbols
) :
    sHostName(sName),
    pcHCFVectors(0),
    oExportSet(roExportedSymbols),
    oImportSet(roImportedSymbols),
    oVersion(oVersion),
    uNumHCFVectors(0)
{
    assert(roHCFVectors.size() <= Machine::Limits::MAX_HCF_VECTORS);
    if ((uNumHCFVectors = (uint32)roHCFVectors.size())) {
        size_t uSize = sizeof(Machine::Interpreter::HCFVector) * uNumHCFVectors;
        if (!(pcHCFVectors = (Machine::Interpreter::HCFVector*)std::malloc(uSize))) {
            throw MC64K::OutOfMemoryException();
        }
        std::memcpy(pcHCFVectors, roHCFVectors.begin(), uSize);
    }
}

/**
 * @inheritDoc
 */
Definition::~Definition() {
    std::free((void*)pcHCFVectors);
}

}} // namespace
