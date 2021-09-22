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
SymbolSet::SymbolSet(size_t const uNumSymbols) :
    poSymbols(0),
    uNumSymbols(uNumSymbols)
{
    if (uNumSymbols > 0) {
        allocateStorage(uNumSymbols);
    }
}

/**
 * @inheritDoc
 */
SymbolSet::~SymbolSet() {
    std::free((void*)poSymbols);
}

/**
 * @inheritDoc
 */
void SymbolSet::allocateStorage(size_t const uNumSymbols) {
    if (!(poSymbols = (Symbol*)std::malloc(uNumSymbols * sizeof(Symbol)))) {
        throw MC64K::OutOfMemoryException();
    }
    this->uNumSymbols = uNumSymbols;
}

/**
 * @inheritDoc
 */
void SymbolSet::dump(std::FILE* poStream) const {
    for (size_t u = 0; u < uNumSymbols; ++u) {
        std::fprintf(
            poStream,
            "\t%4zu @ %p [%c%c%c] \'%s\'\n",
            u,
            poSymbols[u].pRawData,
            (poSymbols[u].uFlags & Symbol::READ    ? 'r' : '-'),
            (poSymbols[u].uFlags & Symbol::WRITE   ? 'w' : '-'),
            (poSymbols[u].uFlags & Symbol::EXECUTE ? 'x' : '-'),
            poSymbols[u].sIdentifier
        );
    }
}

/**
 * @inheritDoc
 */
Symbol* SymbolSet::find(char const* sIdentifier, uint64 const uAccess) const {
    for (size_t u = 0; u < uNumSymbols; ++u) {
        if (
            uAccess == (uAccess & poSymbols[u].uFlags) &&
            0 == std::strcmp(sIdentifier, poSymbols[u].sIdentifier)
        ) {
            return &poSymbols[u];
        }
    }
    return 0;
}

/**
 * @inheritDoc
 */
void SymbolSet::linkAgainst(SymbolSet const& roOther) const {
    for (size_t u = 0; u < uNumSymbols; ++u) {
        Symbol* poMatched = roOther.find(
            poSymbols[u].sIdentifier,
            poSymbols[u].uFlags & Symbol::ACCESS_MASK
        );
        if (poMatched) {
            poSymbols[u].pRawData = poMatched->pRawData;
            std::fprintf(
                stderr,
                "\tMatched %4zu %p [%c%c%c] %s\n",
                u,
                poSymbols[u].pRawData,
                (poSymbols[u].uFlags & Symbol::READ    ? 'r' : '-'),
                (poSymbols[u].uFlags & Symbol::WRITE   ? 'w' : '-'),
                (poSymbols[u].uFlags & Symbol::EXECUTE ? 'x' : '-'),
                poSymbols[u].sIdentifier
            );
        } else {
            std::fprintf(
                stderr,
                "\tUnable to match %4zu %p [%c%c%c] %s\n",
                u,
                poSymbols[u].pRawData,
                (poSymbols[u].uFlags & Symbol::READ    ? 'r' : '-'),
                (poSymbols[u].uFlags & Symbol::WRITE   ? 'w' : '-'),
                (poSymbols[u].uFlags & Symbol::EXECUTE ? 'x' : '-'),
                poSymbols[u].sIdentifier
            );
            throw LinkError();
        }
    }
}

/**
 * @inheritDoc
 */
InitialisedSymbolSet::InitialisedSymbolSet(const std::initializer_list<Symbol>& roSymbols) :
    SymbolSet(roSymbols.size())
{
    if (uNumSymbols) {
        std::memcpy(poSymbols, roSymbols.begin(), uNumSymbols * sizeof(Symbol));
    }
}

/**
 * @inheritDoc
 */
InitialisedSymbolSet::~InitialisedSymbolSet() {

}

/**
 * @inheritDoc
 */
LoadedSymbolSet::LoadedSymbolSet(const size_t uNumSymbols, const uint8* puRawData) :
    SymbolSet(uNumSymbols),
    puRawData(puRawData)
{

}

/**
 * @inheritDoc
 */
Symbol* LoadedSymbolSet::allocate(const size_t uNumSymbols) {
    if (uNumSymbols < 1) {
        throw MC64K::OutOfRangeException("Cannot allocate empty SymbolSet");
    }
    allocateStorage(uNumSymbols);
    return poSymbols;
}

/**
 * @inheritDoc
 */
LoadedSymbolSet::~LoadedSymbolSet() {
    std::free((void*)puRawData);
}

}} // namespace
