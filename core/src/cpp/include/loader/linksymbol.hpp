#ifndef __MC64K_LOADER_LINKSYMBOL_HPP__
#   define __MC64K_LOADER_LINKSYMBOL_HPP__

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
#include "misc/exception.hpp"

#include <initializer_list>

namespace MC64K {
namespace Loader {

#define END_SYMBOL { 0, { 0 }, 0 }
#define IMPORT_SYMBOL(name, access)         { (name), {0}, (access) }
#define EXPORT_SYMBOL(name, access, entity) { (name), { (entity) }, (access) }

/**
 * LinkSymbol
 *
 * Used to resolve exported and imported symbols
 */
struct LinkSymbol {

    /**
     * Flag definitions
     */
    enum {
        READ        = 1,
        WRITE       = 2,
        EXECUTE     = 4,
        ACCESS_MASK = READ|WRITE|EXECUTE
    };

    /**
     * Name
     */
    const char* sIdentifier;

    /**
     * Location
     */
    union {
        void*        pRawData;
        const uint8* pByteCode;
    };

    /**
     * Other properties
     */
    uint64 uFlags;
};


/**
 * LinkSymbolSet (abstract).
 *
 * Provides the basic interface for a set of LinkSymbol instances but doesn't do any resource management.
 */
class LinkSymbolSet {
    protected:
        LinkSymbol*  pSymbols;
        size_t       uNumSymbols;

        /**
         * Check that the index is in range.
         *
         * @param  size_t uIndex
         * @throws MC64K::OutOfRangeException
         */
        void assertIndex(size_t uIndex) const {
            if (uIndex >= uNumSymbols) {
                throw MC64K::OutOfRangeException("Invalid index");
            }
        }

        /**
         * Protected constructor, to be invoked by derived classes only.
         */
        LinkSymbolSet(LinkSymbol* pSymbols, size_t uNumSymbols) : pSymbols(pSymbols), uNumSymbols(uNumSymbols) {
        }

    public:
        /**
         * Virtual Destructor.
         */
        virtual ~LinkSymbolSet() = 0;

        /**
         * Return the number of symbols in the set.
         *
         * @return size_t
         */
        size_t getCount() const {
            return uNumSymbols;
        }

        /**
         * Return a reference to the Symbol data
         *
         * @return LinkSymbol*
         */
        LinkSymbol* getSymbols() const {
            return pSymbols;
        }

        /**
         * Array access (range checked)
         *
         * @param  size_t uIndex
         * @return LinkSymbol&
         * @throws MC64K::OutOfRangeException
         */
        LinkSymbol& operator[](size_t uIndex) {
            assertIndex(uIndex);
            return pSymbols[uIndex];
        }

        /**
         * Array access (range checked)
         *
         * @param  size_t uIndex
         * @return const LinkSymbol&
         * @throws MC64K::OutOfRangeException
         */
        const LinkSymbol& operator[](size_t uIndex) const {
            assertIndex(uIndex);
            return pSymbols[uIndex];
        }

        /**
         * Symbol table dump to stream.
         *
         * @param std::FILE* pStream
         */
        void dump(std::FILE* pStream) const;
};

/**
 * StaticLinkSymbolSet
 *
 * Statically initialised symbol set.
 */
class StaticLinkSymbolSet : public LinkSymbolSet {
    public:
        /**
         * Constructor. Expects an array of symbols, terminated by a symbol referencing null
         */
        StaticLinkSymbolSet(std::initializer_list<LinkSymbol> oSymbols);
        ~StaticLinkSymbolSet();

        StaticLinkSymbolSet(const StaticLinkSymbolSet& oSet);
};

/**
 * DynamicLinkSymbolSet
 */
class DynamicLinkSymbolSet : public LinkSymbolSet {
    private:
        const uint8* pRawData;

    public:
        /**
         * Constructor. If raw data is provided, e.g. the already loaded string data, ownership of that data
         * is transferred to this instance and will be released on destruction.
         *
         * If a non-zero symbol count is provided, allocate() behaviour is invoked.
         *
         * @param  size_t       uNumSymbols
         * @param  const uint8* pRawData
         * @throws MC64K::OutOfMemoryException
         */
        DynamicLinkSymbolSet(size_t uNumSymbols, const uint8* pRawData = 0);

        /**
         * Destructor. Releases any resource owned by the set.
         */
        ~DynamicLinkSymbolSet();

        /**
         * Allocate space for a set of LinkSymbols. These are allocated uninitialised and may contain junk.
         *
         * @param  size_t uNumSymbols
         * @return LinkSymbol
         * @throws MC64K::OutOfRangeException
         * @throws MC64K::OutOfMemoryException
         */
        LinkSymbol* allocate(size_t uNumSymbols);

};

}} // namespace
#endif
