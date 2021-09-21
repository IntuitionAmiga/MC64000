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
    char const* sIdentifier;

    /**
     * Location
     */
    union {
        void*        pRawData;
        void const*  pConstRawData;
        uint8 const* puByteCode;
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
 *
 * Note that while the SymbolSet itself adheres to const correctness, it manages a set of symbols that are not
 * constant, i.e. are open to modification. This is a requirement of the basic linking operation.
 */
class SymbolSet {
    protected:
        Symbol*  poSymbols;
        size_t   uNumSymbols;

        /**
         * Check that the index is in range.
         *
         * @param  size_t const uIndex
         * @throws MC64K::OutOfRangeException
         */
        void assertIndex(size_t const uIndex) const {
            if (uIndex >= uNumSymbols) {
                throw MC64K::OutOfRangeException("Invalid index");
            }
        }

        /**
         * Protected constructor, to be invoked by derived classes only.
         *
         * @param size_t const uNumSymbols
         */
        SymbolSet(size_t const uNumSymbols);

        /**
         * Allocator routine
         *
         * @param  size_t const uNumSymbols
         * @throws MC64K::OutOfMemoryException
         */
        void allocateStorage(size_t const uNumSymbols);

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
         * @param  size_t const uIndex
         * @return Symbol&
         * @throws MC64K::OutOfRangeException
         */
        Symbol& operator[](size_t const uIndex) {
            assertIndex(uIndex);
            return poSymbols[uIndex];
        }

        /**
         * Array access (range checked)
         *
         * @param  size_t const uIndex
         * @return Symbol const&
         * @throws MC64K::OutOfRangeException
         */
        Symbol const& operator[](size_t const uIndex) const {
            assertIndex(uIndex);
            return poSymbols[uIndex];
        }

        /**
         * Try to find the Symbol for a given identifier that can satisfy the given access flags.
         * Where a symbol name is located, each set bit in the supplied access flags must be enabled
         * for the symbol.
         *
         * @param  char const* sIdentifier
         * @param  uint64 const uFlags
         * @return Symbol*
         */
        Symbol* find(char const* sIdentifier, uint64 const uAccess = 0) const;

        /**
         * Symbol table dump to stream.
         *
         * @param std::FILE* poStream
         */
        void dump(std::FILE* poStream) const;

        /**
         * Link symbols against another symbol set.
         */
        void linkAgainst(SymbolSet const& roOther) const;
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
         * @param std::initializer_list<Symbol> const& roSymbols
         */
        InitialisedSymbolSet(std::initializer_list<Symbol> const& roSymbols);

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
        uint8 const* puRawData;

    public:
        /**
         * Constructor
         *
         * @param  size_t const uNumSymbols
         * @param  uint8 const* puRawData
         * @throws MC64K::OutOfMemoryException
         */
        LoadedSymbolSet(size_t const uNumSymbols, uint8 const* puRawData);

        /**
         * Destructor.
         */
        ~LoadedSymbolSet();

        /**
         * Allocate space for a set of Symbols. These are allocated uninitialised and may contain junk.
         *
         * @param  size_t const uNumSymbols
         * @return Symbol
         * @throws MC64K::OutOfRangeException
         * @throws MC64K::OutOfMemoryException
         */
        Symbol* allocate(size_t const uNumSymbols);
};

}} // namespace
#endif

