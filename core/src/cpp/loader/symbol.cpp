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

SymbolSet::SymbolSet(const size_t uNumSymbols) :
    pSymbols(0),
    uNumSymbols(uNumSymbols)
{
    if (uNumSymbols > 0) {
        allocateStorage(uNumSymbols);
    }
}

/**
 * Required destructor
 */
SymbolSet::~SymbolSet() {
    std::free((void*)pSymbols);
}

void SymbolSet::allocateStorage(const size_t uNumSymbols) {
    if (!(pSymbols = (Symbol*)std::malloc(uNumSymbols * sizeof(Symbol)))) {
        throw MC64K::OutOfMemoryException();
    }
    this->uNumSymbols = uNumSymbols;
}

/**
 * Dump the symbol set to a file stream.
 */
void SymbolSet::dump(std::FILE* pStream) const {
    for (size_t u = 0; u < uNumSymbols; ++u) {
        std::fprintf(
            pStream,
            "\t%4zu @ %p [%c%c%c] \'%s\'\n",
            u,
            pSymbols[u].pRawData,
            (pSymbols[u].uFlags & Symbol::READ    ? 'r' : '-'),
            (pSymbols[u].uFlags & Symbol::WRITE   ? 'w' : '-'),
            (pSymbols[u].uFlags & Symbol::EXECUTE ? 'x' : '-'),
            pSymbols[u].sIdentifier
        );
    }
}

/**
 * Attempt to locate symbol by name.
 */
Symbol* SymbolSet::find(const char* sIdentifier, const uint64 uAccess) const {
    for (size_t u = 0; u < uNumSymbols; ++u) {
        if (
            uAccess == (uAccess & pSymbols[u].uFlags) &&
            0 == std::strcmp(sIdentifier, pSymbols[u].sIdentifier)
        ) {
            return &pSymbols[u];
        }
    }
    return 0;
}

void SymbolSet::linkAgainst(const SymbolSet& oOther) const {
    for (size_t u = 0; u < uNumSymbols; ++u) {
        Symbol* pMatched = oOther.find(
            pSymbols[u].sIdentifier,
            pSymbols[u].uFlags & Symbol::ACCESS_MASK
        );
        if (pMatched) {
            pSymbols[u].pRawData = pMatched->pRawData;
            std::fprintf(
                stderr,
                "\tMatched %4zu %p [%c%c%c] %s\n",
                u,
                pSymbols[u].pRawData,
                (pSymbols[u].uFlags & Symbol::READ    ? 'r' : '-'),
                (pSymbols[u].uFlags & Symbol::WRITE   ? 'w' : '-'),
                (pSymbols[u].uFlags & Symbol::EXECUTE ? 'x' : '-'),
                pSymbols[u].sIdentifier
            );
        } else {
            std::fprintf(
                stderr,
                "\tUnable to match %4zu %p [%c%c%c] %s\n",
                u,
                pSymbols[u].pRawData,
                (pSymbols[u].uFlags & Symbol::READ    ? 'r' : '-'),
                (pSymbols[u].uFlags & Symbol::WRITE   ? 'w' : '-'),
                (pSymbols[u].uFlags & Symbol::EXECUTE ? 'x' : '-'),
                pSymbols[u].sIdentifier
            );
            throw LinkError();
        }
    }
}


/**
 * StaticSymbolSet Constructor
 *
 * Explicitly copy the std::initializer_list<Symbol> data here as the internal representation goes out of scope
 * once the calling context has gone.
 */
InitialisedSymbolSet::InitialisedSymbolSet(const std::initializer_list<Symbol>& oSymbols) :
    SymbolSet(oSymbols.size())
{
    if (uNumSymbols) {
        std::memcpy(pSymbols, oSymbols.begin(), uNumSymbols * sizeof(Symbol));
    }
}

/**
 * Required destructor
 */
InitialisedSymbolSet::~InitialisedSymbolSet() {

}

/**
 * DynamicSymbolSet Constructor
 */
LoadedSymbolSet::LoadedSymbolSet(const size_t uNumSymbols, const uint8* pRawData) :
    SymbolSet(uNumSymbols),
    pRawData(pRawData)
{

}

/**
 * SymbolSet allocator
 */
Symbol* LoadedSymbolSet::allocate(const size_t uNumSymbols) {
    if (uNumSymbols < 1) {
        throw MC64K::OutOfRangeException("Cannot allocate empty SymbolSet");
    }
    allocateStorage(uNumSymbols);
    return pSymbols;
}

/**
 * Required destructor
 */
LoadedSymbolSet::~LoadedSymbolSet() {
    std::free((void*)pRawData);
}
