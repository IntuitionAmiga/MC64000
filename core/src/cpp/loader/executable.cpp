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

#include <cstdlib>
#include <cstring>
#include <new>
#include "mc64k.hpp"
#include "loader/executable.hpp"
#include "host/definition.hpp"

namespace MC64K {
namespace Loader {

/**
 * @inheritDoc
 */
char* Executable::processSymbolName(char* sSymbolName, uint64& ruSymbolFlags) {
    uint8 uByte;

    // Advance to the next name. The last byte is the symbol access flags value which is 0-7.
    // Any value above that is assumed to be part of the name.
    while ((uByte = *sSymbolName) > Symbol::ACCESS_MASK) {
        ++sSymbolName;
    }
    ruSymbolFlags = uByte;

    // Null terminate the string for use.
    *sSymbolName++ = 0;
    return sSymbolName;
}

/**
 * @inheritDoc
 */
Executable::Executable(
    Host::Definition const& roDefinition,
    uint8 const* puRawTargetData,
    uint8 const* puRawByteCode,
    uint8*       puRawImportData,
    uint8*       puRawExportData
) :
    oImportedSymbols(0, puRawImportData),
    oExportedSymbols(0, puRawExportData),
    puTargetData(puRawTargetData),
    puByteCode(puRawByteCode)
{
    std::fprintf(stderr, "Loading object file as host '%s'\n", roDefinition.getName());

    Symbol* poSymbol;
    uint32  uNumSymbols;
    if (
        (uNumSymbols = *(uint32*)puRawImportData) &&
        (poSymbol    = oImportedSymbols.allocate(uNumSymbols))
    ) {
        std::fprintf(stderr, "Linking %u imported symbols...\n", uNumSymbols);
        char* sSymbolName   = ((char*)puRawImportData) + sizeof(uint32);
        for (unsigned u = 0; u < uNumSymbols; ++u) {
            poSymbol[u].sIdentifier = sSymbolName;
            poSymbol[u].pRawData    = 0;
            sSymbolName = processSymbolName(sSymbolName, poSymbol[u].uFlags);
        }
        oImportedSymbols.linkAgainst(roDefinition.getExportedSymbolSet());
    }

    if (
        (uNumSymbols = *(uint32*)puRawExportData) &&
        (poSymbol    = oExportedSymbols.allocate(uNumSymbols))
    ) {
        std::fprintf(stderr, "Linking %u exported symbols...\n", uNumSymbols);
        uint32 const* puCodeOffsets = (uint32 const*)(puRawExportData + sizeof(uint32));
        char* sSymbolName = ((char*)puRawExportData) + sizeof(uint32) + uNumSymbols * sizeof(uint32);
        for (unsigned u = 0; u < uNumSymbols; ++u) {
            poSymbol[u].sIdentifier = sSymbolName;
            poSymbol[u].puByteCode  = puRawByteCode + puCodeOffsets[u];
            sSymbolName = processSymbolName(sSymbolName, poSymbol[u].uFlags);
        }
        roDefinition.getImportedSymbolSet().linkAgainst(oExportedSymbols);
    }
}

/**
 * @inheritDoc
 */
uint32 Executable::getStackSize() const {
    return *((const uint32*)(puTargetData + TD_OFFSET_STACK));
}

/**
 * @inheritDoc
 */
Executable::~Executable() {
    std::free((void*)puByteCode);
    std::free((void*)puTargetData);
}

}} // namespace

