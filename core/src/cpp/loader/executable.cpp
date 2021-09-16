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

using namespace MC64K::Loader;
using namespace MC64K::Host;

/**
 *
 */
char* Executable::processSymbolName(char* sSymbolName, uint64& uSymbolFlags) {
    uint8 uByte;

    // Advance to the next name. The last byte is the symbol access flags value which is 0-7.
    // Any value above that is assumed to be part of the name.
    while ((uByte = *sSymbolName) > Symbol::ACCESS_MASK) {
        ++sSymbolName;
    }
    uSymbolFlags = uByte;

    // Null terminate the string for use.
    *sSymbolName++ = 0;
    return sSymbolName;
}

/**
 * Executable Constructor
 *
 * Private, some sanity checks performed before getting here.
 */
Executable::Executable(
    const Host::Definition& oDefinition,
    const uint8* pRawTargetData,
    const uint8* pRawByteCode,
    uint8*       pRawImportData,
    uint8*       pRawExportData
) :
    oImportedSymbols(0, pRawImportData),
    oExportedSymbols(0, pRawExportData),
    pTargetData(pRawTargetData),
    pByteCode(pRawByteCode)
{
    std::fprintf(stderr, "Loading object file as host '%s'\n", oDefinition.getName());

    Symbol* pSymbol;
    uint32  uNumSymbols;
    if (
        (uNumSymbols = *(uint32*)pRawImportData) &&
        (pSymbol     = oImportedSymbols.allocate(uNumSymbols))
    ) {
        std::fprintf(stderr, "Linking %u imported symbols...\n", uNumSymbols);
        char* sSymbolName   = ((char*)pRawImportData) + sizeof(uint32);
        for (unsigned u = 0; u < uNumSymbols; ++u) {
            pSymbol[u].sIdentifier = sSymbolName;
            pSymbol[u].pRawData    = 0;
            sSymbolName = processSymbolName(sSymbolName, pSymbol[u].uFlags);
        }
        oImportedSymbols.linkAgainst(oDefinition.getExportedSymbolSet());
    }

    if (
        (uNumSymbols = *(uint32*)pRawExportData) &&
        (pSymbol     = oExportedSymbols.allocate(uNumSymbols))
    ) {
        std::fprintf(stderr, "Linking %u exported symbols...\n", uNumSymbols);
        const uint32* pCodeOffsets = (uint32*)(pRawExportData + sizeof(uint32));
        char* sSymbolName = ((char*)pRawExportData) + sizeof(uint32) + uNumSymbols * sizeof(uint32);
        for (unsigned u = 0; u < uNumSymbols; ++u) {
            pSymbol[u].sIdentifier = sSymbolName;
            pSymbol[u].pByteCode   = pRawByteCode + pCodeOffsets[u];
            sSymbolName = processSymbolName(sSymbolName, pSymbol[u].uFlags);
        }
        oDefinition.getImportedSymbolSet().linkAgainst(oExportedSymbols);
    }

}

/**
 * Executable destructor
 */
Executable::~Executable() {
    std::free((void*)pByteCode);
    std::free((void*)pTargetData);
}



