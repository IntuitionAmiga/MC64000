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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Binary Constructor
 */
Binary::Binary(const Host::Definition& oDefinition) :
    oHostDefinition(oDefinition),
    sFileName(0),
    pFileHandle(0),
    pManifest(0),
    uManifestLength(0)
{

}

/**
 * Binary Destructor
 */
Binary::~Binary() {
    close();
}

/**
 * Attempts to load and return the Executable
 */
const Executable* Binary::load(const char* sFileName) {

    open(sFileName);

    uint8*      pTargetData = 0;
    uint8*      pImportList = 0;
    uint8*      pExportList = 0;
    uint8*      pByteCode   = 0;
    Executable* pExecutable = 0;

    if (
        (pTargetData = readChunkData(CHUNK_TARGET_ID)) &&
        (validateTarget(pTargetData)) &&
        (pImportList = readChunkData(CHUNK_IMPORT_LIST_ID)) &&
        (pExportList = readChunkData(CHUNK_EXPORT_LIST_ID)) &&
        (pByteCode   = readChunkData(CHUNK_BYTE_CODE_ID)) &&
        (pExecutable = new (std::nothrow) Executable(pTargetData, pImportList, pExportList, pByteCode))
    ) {
        close();
        return pExecutable;
    }
    close();
    std::free(pByteCode);
    std::free(pExportList);
    std::free(pImportList);
    std::free(pTargetData);
    throw Error(sFileName, "unable to load binary");
}

/**
 * Opens a binary file ready for processing.
 */
void Binary::open(const char* sFileName) {
    pFileHandle = std::fopen(sFileName, "rb");
    if (!pFileHandle) {
        throw Error(sFileName, "file could not be opened for input");
    }
    uint64 aHeader[2] = { 0, 0 };
    readChunkHeader(aHeader, FILE_MAGIC_ID);
    loadManifest();
}

/**
 * Closes the binary file and frees any associated resources.
 */
void Binary::close() {
    if (pFileHandle) {
        std::fclose(pFileHandle);
        pFileHandle = 0;
    }
    if (pManifest) {
        std::free(pManifest);
        pManifest = 0;
    }
}

/**
 * Attempts to read the expected chunk header
 */
void Binary::readChunkHeader(uint64 *pHeader, uint64 const uExpectedID) {
    if (2 != std::fread(pHeader, sizeof(uint64), 2, pFileHandle)) {
        throw Error(sFileName, "failed to load header");
    }
    if (uExpectedID != pHeader[0]) {
        throw Error(sFileName, "invalid header ID", pHeader[0]);
    }
}

/**
 * Attempts to load and allocate the Chunk List
 */
void Binary::loadManifest() {
    uint64 aHeader[2] = { 0, 0 };
    readChunkHeader(aHeader, CHUNK_MANIFEST_ID);

    pManifest = (ManifestEntry*)std::malloc(aHeader[1]);

    if (!pManifest) {
        throw Error(sFileName, "unable to allocate chunk", CHUNK_MANIFEST_ID);
    }

    uManifestLength = aHeader[1] / sizeof(ManifestEntry);

    if (uManifestLength != std::fread(pManifest, sizeof(ManifestEntry), uManifestLength, pFileHandle)) {
        throw Error(sFileName, "failed to load chunk", CHUNK_MANIFEST_ID);
    }
    for (uint32 u = 0; u < uManifestLength; ++u) {
        std::printf(
            "\tChunk %.*s found at offset %ld\n",
            8,
            (const char*)(&pManifest[u].uMagicID),
            pManifest[u].iOffset
        );
    }
}

/**
 * Locate a chunk by ID
 */
const Binary::ManifestEntry* Binary::findChunk(const uint64 uChunkID) {
    for (uint32 u = 0; u < uManifestLength; ++u) {
        if (uChunkID == pManifest[u].uMagicID) {
            return &pManifest[u];
        }
    }
    throw Error(sFileName, "missing chunk", uChunkID);
}

/**
 * Allocate and load a chunk by ID
 */
uint8* Binary::readChunkData(const uint64 uChunkID) {
    uint64 aHeader[2] = { 0, 0 };
    uint64 uAllocSize = 0;
    uint8* pRawData   = 0;
    const ManifestEntry* pManifestEntry = findChunk(uChunkID);

    std::fseek(pFileHandle, pManifestEntry->iOffset, SEEK_SET);
    readChunkHeader(aHeader, uChunkID);
    uAllocSize = alignSize(aHeader[1]);
    pRawData   = (uint8*)std::malloc(uAllocSize);
    if (!pRawData) {
        return 0;
    }
    if (uAllocSize != std::fread(pRawData, 1, uAllocSize, pFileHandle)) {
        std::free(pRawData);
        return 0;
    }
    return pRawData;
}

/**
 * Simple target compatibility check.
 *
 * The raw chunk body data format is:
 *     Flags             uint32
 *     Number of entries uint32
 *     Version table     uint32[Number of entries]
 *     Dependency names  char[] (null terminated)
 *
 * The first entry in the version table is the target.
 * For executable targets, the second entry in the version table is the host.
 */
bool Binary::validateTarget(const uint8* pRawTarget) {
    uint32 targetFlags = *(const uint32*)pRawTarget;

    // For an executable target we need to confirm that the current host dependency is viable.
    if (targetFlags & TARGET_EXECUTABLE) {

        // Initial version check is cheap. If the version number is compatible we can then check that we are
        // talking about the same thing.
        const Misc::Version* pVersionTable = (const Misc::Version*)(pRawTarget + sizeof(uint32) * 2);

        if (oHostDefinition.getVersion().isCompatible(pVersionTable[HOST_VERSION_ENTRY])) {

            // We need to compare the dependency name now. This involves locating the entry in the names
            // section that follows the version table. The host is always the second entry.
            uint32 uNumEntries = *(const uint32*)(pRawTarget + sizeof(uint32));
            const char* sName  = (const char*)(pRawTarget + sizeof(uint32) * (2 + uNumEntries));

            // Skip over the target name to the next entry, which is expected to be the host.
            // Since all names are null terminated, we skip onwards into buffer overrun oblivion...
            while (*sName++);

            // Compare the names.
            if (0 == std::strcmp(oHostDefinition.getName(), sName)) {
                return true;
            } else {
                std::printf(
                    "Incompatible host: Need \'%s\', have \'%s\'\n",
                    sName,
                    oHostDefinition.getName()
                );
            }
        } else {
            std::printf(
                "Host semantic version number check failed: Need %u.%u.%u, have %u.%u.%u\n",
                pVersionTable[HOST_VERSION_ENTRY].getMajor(),
                pVersionTable[HOST_VERSION_ENTRY].getMinor(),
                pVersionTable[HOST_VERSION_ENTRY].getPatch(),
                oHostDefinition.getVersion().getMajor(),
                oHostDefinition.getVersion().getMinor(),
                oHostDefinition.getVersion().getPatch()
            );
        }
    }
    return false;
}
