#ifndef __MC64K_LOADER_SYMBOL_HPP__
#   define __MC64K_LOADER_SYMBOL_HPP__

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

#define IMPORT_SYMBOL(name, access)         { (name), {0}, (access) }
#define EXPORT_SYMBOL(name, access, entity) { (name), { (void*)entity }, (access) }

/**
 * Symbol
 *
 * Used to resolve exported and imported symbols
 */
struct Symbol {

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
        const void*  pConstRawData;
        const uint8* puByteCode;
    };

    /**
     * Other properties
     */
    uint64 uFlags;
};


/**
 * SymbolSet (abstract).
 *
 * Provides the basic interface for a set of Symbol instances but doesn't do any resource management.
 */
class SymbolSet {
    protected:
        Symbol*  poSymbols;
        size_t   uNumSymbols;

        /**
         * Check that the index is in range.
         *
         * @param  size_t uIndex
         * @throws MC64K::OutOfRangeException
         */
        void assertIndex(const size_t uIndex) const {
            if (uIndex >= uNumSymbols) {
                throw MC64K::OutOfRangeException("Invalid index");
            }
        }

        /**
         * Protected constructor, to be invoked by derived classes only.
         *
         * @param size_t uNumSymbols
         */
        SymbolSet(const size_t uNumSymbols);

        /**
         * Allocator routine
         *
         * @param  size_t uNumSymbols
         * @throws MC64K::OutOfMemoryException
         */
        void allocateStorage(const size_t uNumSymbols);

    public:

        /**
         * Virtual Destructor.
         */
        virtual ~SymbolSet() = 0;

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
         * @return Symbol*
         */
        Symbol* getSymbols() const {
            return poSymbols;
        }

        /**
         * Array access (range checked)
         *
         * @param  size_t uIndex
         * @return Symbol&
         * @throws MC64K::OutOfRangeException
         */
        Symbol& operator[](const size_t uIndex) {
            assertIndex(uIndex);
            return poSymbols[uIndex];
        }

        /**
         * Array access (range checked)
         *
         * @param  size_t uIndex
         * @return const Symbol&
         * @throws MC64K::OutOfRangeException
         */
        const Symbol& operator[](const size_t uIndex) const {
            assertIndex(uIndex);
            return poSymbols[uIndex];
        }

        /**
         * Try to find the Symbol for a given identifier that can satisfy the given access flags.
         * Where a symbol name is located, each set bit in the supplied access flags must be enabled
         * for the symbol.
         *
         * @param  const  char* sIdentifier
         * @param  uint64 uFlags
         * @return Symbol*
         */
        Symbol* find(const char* sIdentifier, const uint64 uAccess = 0) const;

        /**
         * Symbol table dump to stream.
         *
         * @param std::FILE* poStream
         */
        void dump(std::FILE* poStream) const;

        /**
         * Link symbols against another symbol set.
         */
        void linkAgainst(const SymbolSet& roOther) const;
};

/**
 * InitialisedSymbolSet
 *
 * Statically declared symbol set.
 */
class InitialisedSymbolSet : public SymbolSet {
    public:
        /**
         * Constructor for initialised set
         *
         * @param const std::initializer_list<Symbol>& roSymbols
         */
        InitialisedSymbolSet(const std::initializer_list<Symbol>& roSymbols);

        /**
         * Destructor
         */
        ~InitialisedSymbolSet();
};

/**
 * LoadedSymbolSet
 *
 * This represents a symbol table that was loaded from an object file. In addition to managing the storage, it also
 * takes ownership of a pointer to the raw data that was loaded and ensures it is released.
 */
class LoadedSymbolSet : public SymbolSet {
    private:
        const uint8* puRawData;

    public:
        /**
         * Constructor
         *
         * @param  const size_t uNumSymbols
         * @param  const uint8* puRawData
         * @throws MC64K::OutOfMemoryException
         */
        LoadedSymbolSet(const size_t uNumSymbols, const uint8* puRawData);

        /**
         * Destructor.
         */
        ~LoadedSymbolSet();

        /**
         * Allocate space for a set of Symbols. These are allocated uninitialised and may contain junk.
         *
         * @param  const size_t uNumSymbols
         * @return Symbol
         * @throws MC64K::OutOfRangeException
         * @throws MC64K::OutOfMemoryException
         */
        Symbol* allocate(const size_t uNumSymbols);
};

}} // namespace
#endif

