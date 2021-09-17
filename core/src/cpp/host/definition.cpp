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
#include "host/definition.hpp"

namespace MC64K {
namespace Host {

/**
 * @inheritDoc
 */
Definition::Definition(
    const char* sName,
    const Misc::Version oVersion,
    const std::initializer_list<Machine::Interpreter::HostCall>& roVectors,
    const std::initializer_list<Loader::Symbol>& roExportedSymbols,
    const std::initializer_list<Loader::Symbol>& roImportedSymbols
) :
    sHostName(sName),
    pcVectors(0),
    oExportSet(roExportedSymbols),
    oImportSet(roImportedSymbols),
    oVersion(oVersion)
{
    if (roVectors.size() > 256) {
        throw MC64K::OutOfRangeException("Vector List Too Large");
    }
    if ((uMaxVector = roVectors.size())) {
        size_t uSize = sizeof(Machine::Interpreter::HostCall) * roVectors.size();
        if (!(pcVectors = (Machine::Interpreter::HostCall*)std::malloc(uSize))) {
            throw MC64K::OutOfMemoryException();
        }
        std::memcpy(pcVectors, roVectors.begin(), uSize);
    }
}

/**
 * @inheritDoc
 */
Definition::~Definition() {
    std::free((void*)pcVectors);
}

}} // namespace
