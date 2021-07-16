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
#include "include/mc64k.hpp"
#include "include/loader.hpp"

using namespace MC64K::Loader;

const uint64 FILE_MAGIC = 0x583030303436434D; // MC64000X

Binary::Binary(const char* sFileName) : sFileName(sFileName), pFileHandle(0) {
    pFileHandle = std::fopen(sFileName, "rb");
    if (!pFileHandle) {
        throw Error::FileIO(sFileName, "file could not be opened for input");
    }

    uint64 aHeader[2] = { 0, 0 };
    if (2 != std::fread(aHeader, sizeof(uint64), 2, pFileHandle)) {
        throw Error::FileIO(sFileName, "failed to load header");
    }
    if (FILE_MAGIC != aHeader[0]) {
        throw Error::FileIO(sFileName, "invalid file header");
    }
}

Binary::~Binary() {
    if (pFileHandle) {
        std::fclose(pFileHandle);
        pFileHandle = 0;
    }
}

