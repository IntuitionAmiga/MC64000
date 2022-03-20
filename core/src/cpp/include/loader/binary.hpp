#ifndef MC64K_LOADER_BINARY_HPP
    #define MC64K_LOADER_BINARY_HPP

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
#include <host/definition.hpp>
#include "error.hpp"
#include "symbol.hpp"

namespace MC64K::Loader {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * Forwards References
 */
class Executable;

/**
 * Binary
 *
 * Handles loading of assembled binary object code
 */
class Binary {
    public:
        /**
         * Constructor. Requires a valid Host Definition for loading validation purposes.
         *
         * @param Host::Definition const& roDefinition
         */
        Binary(Host::Definition const& roDefinition);

        /**
         * Destructor
         */
        ~Binary();

        /**
         * Loader. Attempts to load the named binary file and return an executable structure.
         *
         * @param  char const* sFileName
         * @return Executable const*
         */
        Executable const* load(char const* sFileName);

    private:

        /**
         * Magic ID values. 64-bit word representation of 8 character strings
         */
        enum Magic {
            FILE_MAGIC_ID        = 0x583030303436434D, // MC64000X
            CHUNK_MANIFEST_ID    = 0x74736566696E614D, // Manifest
            CHUNK_TARGET_ID      = 0x6F666E4974677254, // TrgtInfo
            CHUNK_BYTE_CODE_ID   = 0x65646F4365747942, // ByteCode
            CHUNK_EXPORT_LIST_ID = 0x646574726F707845, // Exported
            CHUNK_IMPORT_LIST_ID = 0x646574726F706D49, // Imported
        };

        /**
         * Target info flags.
         */
        enum TargetFlags {
            TARGET_EXECUTABLE    = 1,
        };

        /**
         * Reserved positions in the Target version table.
         */
        enum VersionTableOffset {
            TARGET_VERSION_ENTRY = 0,
            HOST_VERSION_ENTRY   = 1,
        };

        /**
         * Other stuff
         */
        enum {
            ALIGN_MASK = 7,
        };

        /**
         * Manifest record structure.
         */
        struct ManifestEntry {
            uint64 uMagicID;
            int64  iOffset;
        };

        Host::Definition const& roHostDefinition;
        char const*             sFileName;
        std::FILE*              poFileHandle;
        ManifestEntry*          poManifest;
        uint32                  uManifestLength;

        /**
         * Align an input size to the required boundary.
         *
         * @param  const size_t uSize
         * @return size_t
         */
        size_t alignSize(const size_t uSize) const {
            return (uSize + ALIGN_MASK) & ~ALIGN_MASK;
        }

        /**
         * Open the binary object file
         *
         * @param char const* sFileName
         */
        void open(char const* sFileName);

        /**
         * Close the binary object file
         */
        void close();

        /**
         * Load and allocate the Manifest List data
         */
        void loadManifest();

        /**
         * Try to find manifest record with the given chunk ID
         *
         * @param  uint64 const uExpectedID
         * @return ManifestEntry const*
         */
        ManifestEntry const* findChunk(uint64 const uChunkID);

        /**
         * Read a chunk header into a marshalling area
         *
         * @param uint64*      puHeader
         * @param uint64 const uExpectedID
         */
        void readChunkHeader(uint64* puHeader, uint64 const uExpectedID);

        /**
         * Load a chunk with the given ID. Uses the manifest data to locate the offset (if present),
         * allocates storage and loads the raw data.
         *
         * @param  uint64 const uChunkID
         * @return uint8*
         */
        uint8* readChunkData(uint64 const uChunkID);

        /**
         * Validate the raw taget data (minimum verification)
         *
         * @param  uint8 const* puRawTarget
         * @return bool
         */
        bool validateTarget(uint8 const* puRawTarget);
};

} // namespace
#endif
