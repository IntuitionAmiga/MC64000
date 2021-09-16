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
 * Constructor
 *
 * @param const char* sName,
 * @param const Misc::Version oVersion,
 * @param const std::initializer_list<Machine::Interpreter::HostCall>& oVectors,
 * @param const std::initializer_list<Loader::Symbol>& oExportedSymbols,
 * @param const std::initializer_list<Loader::Symbol>& oImportedSymbols
 */
Definition::Definition(
    const char* sName,
    const Misc::Version oVersion,
    const std::initializer_list<Machine::Interpreter::HostCall>& oVectors,
    const std::initializer_list<Loader::Symbol>& oExportedSymbols,
    const std::initializer_list<Loader::Symbol>& oImportedSymbols
) :
    sHostName(sName),
    aHostVectors(0),
    oExportSet(oExportedSymbols),
    oImportSet(oImportedSymbols),
    oVersion(oVersion)
{
    if (oVectors.size() > 256) {
        throw MC64K::OutOfRangeException("Vector List Too Large");
    }
    if ((uMaxVector = oVectors.size())) {

        size_t uSize = sizeof(Machine::Interpreter::HostCall) * oVectors.size();
        if (!(aHostVectors = (Machine::Interpreter::HostCall*)std::malloc(uSize))) {
            throw MC64K::OutOfMemoryException();
        }
        std::memcpy(aHostVectors, oVectors.begin(), uSize);
    }
}

/**
 * Destructor
 */
Definition::~Definition() {
    std::free((void*)aHostVectors);
}

}} // namespace
