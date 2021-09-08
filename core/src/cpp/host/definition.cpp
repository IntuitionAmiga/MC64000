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
#include <cstdlib>
#include "host/definition.hpp"

namespace MC64K {
namespace Host {

Definition::Definition(
    const char* sName,
    const Misc::Version oVersion,
    Machine::Interpreter::HostCall aVectors[],
    Loader::LinkSymbol aExports[],
    Loader::LinkSymbol aImports[]
) :
    sHostName(sName),
    aHostVectors(0),
    aHostExportSymbols(0),
    aHostImportSymbols(0),
    oVersion(oVersion)
{
    if (aVectors) {
        aHostVectors = aVectors;
        while (*aVectors) {
            ++uMaxVector;
            ++aVectors;
        }
    }

    if (aExports) {
        aHostExportSymbols = aExports;
    }
    if (aImports) {

    }
}

}} // namespace
