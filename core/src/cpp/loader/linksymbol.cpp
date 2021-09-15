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
 * Required empty destructor
 */
LinkSymbolSet::~LinkSymbolSet() {

}

/**
 * Dump the symbol set to a file stream.
 */
void LinkSymbolSet::dump(std::FILE* pStream) const {
    for (unsigned u = 0; u < uNumSymbols; ++u) {
        std::fprintf(
            pStream,
            "\t%2u %p [%c%c%c] %s\n",
            u,
            pSymbols[u].pRawData,
            (pSymbols[u].uFlags & LinkSymbol::READ    ? 'r' : '-'),
            (pSymbols[u].uFlags & LinkSymbol::WRITE   ? 'w' : '-'),
            (pSymbols[u].uFlags & LinkSymbol::EXECUTE ? 'x' : '-'),
            pSymbols[u].sIdentifier
        );
    }
}

/**
 * StaticLinkSymbolSet Constructor
 */
StaticLinkSymbolSet::StaticLinkSymbolSet(std::initializer_list<LinkSymbol> oSymbols) :
    LinkSymbolSet(0, 0)
{
    pSymbols = (LinkSymbol*)oSymbols.begin();
    if (pSymbols) {
        LinkSymbol* p = pSymbols;
        size_t uCount = 0;
        while (p->sIdentifier) {
            ++uCount;
            ++p;
        }
        uNumSymbols = uCount;
    }
}

StaticLinkSymbolSet::StaticLinkSymbolSet(const StaticLinkSymbolSet& oSet) :
    LinkSymbolSet(oSet.pSymbols, oSet.uNumSymbols)
{
}


/**
 * Required destructor
 */
StaticLinkSymbolSet::~StaticLinkSymbolSet() {
    std::printf("Destroying StaticLinkSymbolSet at %p...\n", this);
}

/**
 * DynamicLinkSymbolSet Constructor
 */
DynamicLinkSymbolSet::DynamicLinkSymbolSet(size_t uNumSymbols, const uint8* pRawData) :
    LinkSymbolSet(0, uNumSymbols),
    pRawData(pRawData)
{
    if (uNumSymbols) {
        allocate(uNumSymbols);
    }
}

/**
 * LinkSymbolSet allocator
 */
LinkSymbol* DynamicLinkSymbolSet::allocate(size_t uNumSymbols) {
    if (uNumSymbols < 1) {
        throw MC64K::OutOfRangeException("Cannot allocate empty LinkSymbolSet");
    }
    if (!(pSymbols = (LinkSymbol*)std::malloc(uNumSymbols * sizeof(LinkSymbol)))) {
        throw MC64K::OutOfMemoryException();
    }
    this->uNumSymbols = uNumSymbols;
    return pSymbols;
}

/**
 * Required destructor
 */
DynamicLinkSymbolSet::~DynamicLinkSymbolSet() {
    std::printf("Destroying DynamicLinkSymbolSet at %p...\n", this);

    std::free((void*)pSymbols);
    std::free((void*)pRawData);
}
