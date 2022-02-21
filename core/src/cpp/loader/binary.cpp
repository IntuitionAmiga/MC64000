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

namespace MC64K::Loader {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @inheritDoc
 */
Binary::Binary(Host::Definition const& roDefinition) :
    roHostDefinition(roDefinition),
    sFileName(0),
    poFileHandle(0),
    poManifest(0),
    uManifestLength(0)
{

}

/**
 * @inheritDoc
 */
Binary::~Binary() {
    close();
}

/**
 * @inheritDoc
 */
const Executable* Binary::load(char const* sFileName) {

    open(sFileName);

    uint8*      puTargetData = 0;
    uint8*      puImportList = 0;
    uint8*      puExportList = 0;
    uint8*      puByteCode   = 0;
    Executable* poExecutable = 0;

    if (
        (puTargetData = readChunkData(CHUNK_TARGET_ID)) &&
        (validateTarget(puTargetData)) &&
        (puImportList = readChunkData(CHUNK_IMPORT_LIST_ID)) &&
        (puExportList = readChunkData(CHUNK_EXPORT_LIST_ID)) &&
        (puByteCode   = readChunkData(CHUNK_BYTE_CODE_ID)) &&
        (poExecutable = new (std::nothrow) Executable(
            roHostDefinition,
            puTargetData,
            puByteCode,
            puImportList,
            puExportList)
        )
    ) {
        close();
        return poExecutable;
    }
    close();
    std::free(puByteCode);
    std::free(puExportList);
    std::free(puImportList);
    std::free(puTargetData);
    throw Error(sFileName, "unable to load binary");
}

/**
 * @inheritDoc
 */
void Binary::open(char const* sFileName) {
    poFileHandle = std::fopen(sFileName, "rb");
    if (!poFileHandle) {
        throw Error(sFileName, "file could not be opened for input");
    }
    uint64 auHeader[2] = { 0, 0 };
    readChunkHeader(auHeader, FILE_MAGIC_ID);
    loadManifest();
}

/**
 * @inheritDoc
 */
void Binary::close() {
    if (poFileHandle) {
        std::fclose(poFileHandle);
        poFileHandle = 0;
    }
    if (poManifest) {
        std::free(poManifest);
        poManifest = 0;
    }
}

/**
 * @inheritDoc
 */
void Binary::readChunkHeader(uint64* puHeader, uint64 const uExpectedID) {
    if (2 != std::fread(puHeader, sizeof(uint64), 2, poFileHandle)) {
        throw Error(sFileName, "failed to load header");
    }
    if (uExpectedID != puHeader[0]) {
        throw Error(sFileName, "invalid header ID", puHeader[0]);
    }
}

/**
 * @inheritDoc
 */
void Binary::loadManifest() {
    uint64 auHeader[2] = { 0, 0 };
    readChunkHeader(auHeader, CHUNK_MANIFEST_ID);

    poManifest = (ManifestEntry*)std::malloc(auHeader[1]);

    if (!poManifest) {
        throw Error(sFileName, "unable to allocate chunk", CHUNK_MANIFEST_ID);
    }

    uManifestLength = (uint32) (auHeader[1] / sizeof(ManifestEntry));

    if (uManifestLength != std::fread(poManifest, sizeof(ManifestEntry), uManifestLength, poFileHandle)) {
        throw Error(sFileName, "failed to load chunk", CHUNK_MANIFEST_ID);
    }
}

/**
 * @inheritDoc
 */
const Binary::ManifestEntry* Binary::findChunk(uint64 const uChunkID) {
    for (uint32 u = 0; u < uManifestLength; ++u) {
        if (uChunkID == poManifest[u].uMagicID) {
            return &poManifest[u];
        }
    }
    throw Error(sFileName, "missing chunk", uChunkID);
}

/**
 * @inheritDoc
 */
uint8* Binary::readChunkData(uint64 const uChunkID) {
    uint64 auHeader[2] = { 0, 0 };
    uint64 uAllocSize  = 0;
    uint8* puRawData   = 0;
    ManifestEntry const* poManifestEntry = findChunk(uChunkID);

    std::fseek(poFileHandle, poManifestEntry->iOffset, SEEK_SET);
    readChunkHeader(auHeader, uChunkID);
    uAllocSize = alignSize(auHeader[1]);
    puRawData   = (uint8*)std::malloc(uAllocSize);
    if (!puRawData) {
        return 0;
    }
    if (uAllocSize != std::fread(puRawData, 1, uAllocSize, poFileHandle)) {
        std::free(puRawData);
        return 0;
    }
    return puRawData;
}

/**
 * @inheritDoc
 *
 * Simple target compatibility check.
 *
 * The raw chunk body data format is:
 *     Flags             uint32
 *     Stack Size        uint32
 *     Number of entries uint32
 *     Version table     uint32[Number of entries]
 *     Dependency names  char[] (null terminated)
 *
 * The first entry in the version table is the target.
 * For executable targets, the second entry in the version table is the host.
 */



bool Binary::validateTarget(uint8 const* puRawTarget) {
    uint32 uTargetFlags = *(uint32 const*)puRawTarget;

    // For an executable target we need to confirm that the current host dependency is viable.
    if (uTargetFlags & TARGET_EXECUTABLE) {

        // Initial version check is cheap. If the version number is compatible we can then check that we are
        // talking about the same thing.
        const Misc::Version* poVersionTable = (const Misc::Version*)(puRawTarget + sizeof(uint32) * 3);

        if (roHostDefinition.getVersion().isCompatible(poVersionTable[HOST_VERSION_ENTRY])) {

            uint32 uStackSize = *(uint32 const*)(puRawTarget + sizeof(uint32));
            if (
                uStackSize < Machine::Limits::MIN_STACK_SIZE ||
                uStackSize > Machine::Limits::MAX_STACK_SIZE
            ) {
                std::fprintf(
                    stderr,
                    "Invalid stack size %" PFU32 ", legal range is %" PFU32 " to %" PFU32 "\n",
                    uStackSize,
                    Machine::Limits::MIN_STACK_SIZE,
                    Machine::Limits::MAX_STACK_SIZE
                );
                return false;
            }


            // We need to compare the dependency name now. This involves locating the entry in the names
            // section that follows the version table. The host is always the second entry.
            uint32 uNumEntries = *(uint32 const*)(puRawTarget + 2 * sizeof(uint32));
            char const* sName  = (char const*)(puRawTarget + sizeof(uint32) * (3 + uNumEntries));

            // Skip over the target name to the next entry, which is expected to be the host.
            // Since all names are null terminated, we skip onwards into buffer overrun oblivion...
            while (*sName++);

            // Compare the names.
            if (0 == std::strcmp(roHostDefinition.getName(), sName)) {
                return true;
            } else {
                std::fprintf(
                    stderr,
                    "Incompatible host: Need \'%s\', have \'%s\'\n",
                    sName,
                    roHostDefinition.getName()
                );
            }
        } else {
            std::fprintf(
                stderr,
                "Host semantic version number check failed: Need %u.%u.%u, have %u.%u.%u\n",
                poVersionTable[HOST_VERSION_ENTRY].getMajor(),
                poVersionTable[HOST_VERSION_ENTRY].getMinor(),
                poVersionTable[HOST_VERSION_ENTRY].getPatch(),
                roHostDefinition.getVersion().getMajor(),
                roHostDefinition.getVersion().getMinor(),
                roHostDefinition.getVersion().getPatch()
            );
        }
    }
    return false;
}

} // namespace

