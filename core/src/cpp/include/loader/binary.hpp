#ifndef __MC64K_LOADER_BINARY_HPP__
#   define __MC64K_LOADER_BINARY_HPP__

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

#include "error.hpp"
#include <cstdio>

namespace MC64K {
namespace Loader {

class Executable;

/**
 * Binary
 */
class Binary {
    private:
        const char* sFileName;
        std::FILE*  pFileHandle;
    public:
        Binary(const char* sFileName);
        ~Binary();

        const Executable* load();

    private:
        const uint64 FILE_MAGIC_ID        = 0x583030303436434D; // MC64000X
        const uint64 CHUNK_LIST_ID        = 0x7473694C6B6E6843; // ChnkList
        const uint64 CHUNK_BYTE_CODE_ID   = 0x65646F4365747942; // ByteCode
        const uint64 CHUNK_EXPORT_LIST_ID = 0x646574726F707845; // Exported
        const uint64 CHUNK_IMPORT_LIST_ID = 0x646574726F706D49; // Imported
        const uint64 ALIGN_MASK           = 7;

        struct ChunkListEntry {
            uint64 uMagicID;
            int64  iOffset;
        };

        ChunkListEntry* pChunkList;
        uint32          uChunkListLength;

        uint64 alignSize(const uint64 uSize) const {
            return (uSize + ALIGN_MASK) & ~ALIGN_MASK;
        }

        void   readChunkHeader(uint64* pHeader, const uint64 uExpectedID);
        void   loadChunkList();
        uint8* readChunkData(const uint64 uChunkID);
        const  ChunkListEntry* findChunk(const uint64 uChunkID);
};

/**
 * Executable
 */
class Executable {
    friend const Executable* Binary::load();

    public:
        struct Symbol {
            const char* sIdentifier;
            union {
                const uint8* pByteCode;
                void*        pRawData;
            };
        };

    private:
        const uint8* pExportData;
        const uint8* pImportData;
        const uint8* pByteCode;
        Symbol*      pExportedSymbols;
        Symbol*      pImportedSymbols;
        uint32       uNumExportedSymbols;
        uint32       uNumImportedSymbols;

    public:

        /**
         * Get the number of symbols exported by this binary.
         */
        uint32 getNumExportedSymbols() const {
            return uNumExportedSymbols;
        }

        /**
         * Get the symbols exported by this binary. They are assumed immutable from the host side.
         */
        const Symbol* getExportedSymbols() const {
            return pExportedSymbols;
        }

        /**
         * Get the number of symbols imported by this binary.
         */
        uint32 getNumImportedSymbols() const {
            return uNumImportedSymbols;
        }

        /**
         * Get the symbols imported by this binary. The host is expected to match the identifiers with
         * appropriate runtime addresses for the expected data.
         */
        Symbol* getImportedSymbols() const {
            return pImportedSymbols;
        }

        ~Executable();

    private:
        /**
         * Constructable only by the binary loader
         */
        Executable(
            const uint8* pRawExportData,
            const uint8* pRawImportData,
            const uint8* pRawByteCode
        );

};

}} // namespace
#endif
