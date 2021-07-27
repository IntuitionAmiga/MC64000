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
#include <new>
#include "mc64k.hpp"
#include "loader/binary.hpp"

using namespace MC64K::Loader;

/**
 * Binary Constructor
 */
Binary::Binary(const char* sFileName) :
    sFileName(sFileName),
    pFileHandle(0), pChunkList(0),
    uChunkListLength(0)
{
    pFileHandle = std::fopen(sFileName, "rb");
    if (!pFileHandle) {
        throw Error(sFileName, "file could not be opened for input");
    }
    uint64 aHeader[2] = { 0, 0 };
    readChunkHeader(aHeader, FILE_MAGIC_ID);
}

/**
 * Binary Destructor
 */
Binary::~Binary() {
    if (pFileHandle) {
        std::fclose(pFileHandle);
        pFileHandle = 0;
    }
    if (pChunkList) {
        std::free(pChunkList);
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
void Binary::loadChunkList() {
    uint64 aHeader[2] = { 0, 0 };
    readChunkHeader(aHeader, CHUNK_LIST_ID);

    pChunkList = (ChunkListEntry*)std::malloc(aHeader[1]);

    if (!pChunkList) {
        throw Error(sFileName, "unable to allocate chunk", CHUNK_LIST_ID);
    }

    uChunkListLength = aHeader[1] / sizeof(ChunkListEntry);

    if (uChunkListLength != std::fread(pChunkList, sizeof(ChunkListEntry), uChunkListLength, pFileHandle)) {
        throw Error(sFileName, "failed to load chunk", CHUNK_LIST_ID);
    }
//     for (uint32 u = 0; u < uChunkListLength; ++u) {
//         std::printf("\tChunk ID 0x%0lX at offset %ld\n", pChunkList[u].uMagicID, pChunkList[u].iOffset);
//     }
}

const Binary::ChunkListEntry* Binary::findChunk(const uint64 uChunkID) {
    for (uint32 u = 0; u < uChunkListLength; ++u) {
        if (uChunkID == pChunkList[u].uMagicID) {
            return &pChunkList[u];
        }
    }
    throw Error(sFileName, "missing chunk", uChunkID);
}

uint8* Binary::readChunkData(const uint64 uChunkID) {
    uint64 aHeader[2] = { 0, 0 };
    uint64 uAllocSize = 0;
    uint8* pRawData   = 0;
    const ChunkListEntry* pChunkListEntry = findChunk(uChunkID);

    std::fseek(pFileHandle, pChunkListEntry->iOffset, SEEK_SET);
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
 * Attempts to load and return the Executable
 */
const Executable* Binary::load() {
    loadChunkList();
    uint8*      pByteCode   = 0;
    uint8*      pExportList = 0;
    Executable* pExecutable = 0;

    if (
        (pByteCode   = readChunkData(CHUNK_BYTE_CODE_ID)) &&
        (pExportList = readChunkData(CHUNK_EXPORT_LIST_ID)) &&
        (pExecutable = new (std::nothrow) Executable(pExportList, pByteCode))
    ) {
        return pExecutable;
    }

    std::free(pByteCode);
    std::free(pExportList);
    throw Error(sFileName, "unable to load binary");
}

/**
 * Executable Constructor
 *
 * Private, sanity checks performed before getting here.
 */
Executable::Executable(const uint8* pRawExportData, const uint8* pRawByteCode) :
    pExportData(pRawExportData),
    pByteCode(pRawByteCode),
    pEntryPoints(0),
    uNumEntryPoints(0)
{
    uNumEntryPoints = *(uint32*)pExportData;
    pEntryPoints    = (EntryPoint*)std::malloc(uNumEntryPoints * sizeof(EntryPoint));

    const uint32* pOffsets = (uint32*)(pExportData + 4);
    const uint8*  pLengths = pExportData + 4 + uNumEntryPoints * sizeof(uint32);
    const char*   pName    = (const char*)(pLengths + uNumEntryPoints);

    for (unsigned u = 0; u < uNumEntryPoints; ++u) {
        pEntryPoints[u].sFunction = pName;
        pEntryPoints[u].pByteCode = pRawByteCode + pOffsets[u];
        pName += pLengths[u] + 1; // 1 extra for null terminator
    }
}

/**
 * Executable destructor
 */
Executable::~Executable() {
    if (pExportData) {
        std::free((void*)pExportData);
    }
    if (pByteCode) {
        std::free((void*)pByteCode);
    }
    if (pEntryPoints) {
        std::free(pEntryPoints);
    }
}
