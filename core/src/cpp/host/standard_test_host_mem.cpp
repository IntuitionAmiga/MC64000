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
#include <cstdlib>
#include <cstring>
#include <host/standard_test_host_mem.hpp>
#include <machine/register.hpp>
#include <host/mem/inline.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Mem {

/**
 * Fill a word aligned block with words. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint16 uValue
 * @param uint64 uSize
 */
void fillWord(void* pBuffer, uint16 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 1) {
        --uSize;
        ++uRawAddress;
    }
    uint16 *p = (uint16*)uRawAddress;
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Fill a long aligned block with longs. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint32 uValue
 * @param uint64 uSize
 */
void fillLong(void* pBuffer, uint32 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 3) {
        --uSize;
        uRawAddress = (uRawAddress + 3) & ~3ULL;
    }
    uint32 *p = (uint32*)uRawAddress;
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Fill a word aligned block with words. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint64 uValue
 * @param uint64 uSize
 */
void fillQuad(void* pBuffer, uint64 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 7) {
        --uSize;
        uRawAddress = (uRawAddress + 7) & ~7ULL;
    }
    uint64 *p = (uint64*)uRawAddress;
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Locate the first occurence of a 16 bit value in a memory block. If the base address is not aligned, search starts
 * from the next aligned address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param  void const*    pBuffer
 * @param  uint16         uValue
 * @param  uint64         uSize
 * @return uint16 const*  pFound
 */
uint16 const* findWord(void const* pBuffer, uint16 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 1) {
        --uSize;
        ++uRawAddress;
    }
    uint16 const *p = (uint16 const*)uRawAddress;
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

/**
 * Locate the first occurence of a 32 bit value in a memory block. If the base address is not aligned, search starts
 * from the next aligned address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param  void const*    pBuffer
 * @param  uint32         uValue
 * @param  uint64         uSize
 * @return uint32 const*  pFound
 */
uint32 const* findLong(void const* pBuffer, uint32 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 3) {
        --uSize;
        uRawAddress = (uRawAddress + 3) & ~3ULL;
    }
    uint32 const *p = (uint32 const*)uRawAddress;
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

/**
 * Locate the first occurence of a 64 bit value in a memory block. If the base address is not aligned, search starts
 * from the next aligned address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks
 *
 * @param  void const*   pBuffer
 * @param  uint64        uValue
 * @param  uint64        uSize
 * @return uint64 const* pFound
 */
uint64 const* findQuad(void const* pBuffer, uint64 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 7) {
        --uSize;
        uRawAddress = (uRawAddress + 7) & ~7ULL;
    }
    uint64 const *p = (uint64 const*)uRawAddress;
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

/**
 * Mem::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {
    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {
        case INIT:
        case DONE:
            break;

        case ALLOC:
            if (uint64 uSize  = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
                void* pBuffer = std::malloc(uSize);
                Interpreter::gpr<ABI::PTR_REG_0>().pAny  = pBuffer;
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = pBuffer ?
                    (uint64)ABI::ERR_NONE :
                    (uint64)ERR_NO_MEM;
            } else {
                Interpreter::gpr<ABI::PTR_REG_0>().pAny  = 0;
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ERR_MEM;
            }
            break;

        case FREE:
            std::free(Interpreter::gpr<ABI::PTR_REG_0>().pAny);
            Interpreter::gpr<ABI::PTR_REG_0>().pAny = 0;
            break;

        case COPY:
            if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
                void* pFrom = Interpreter::gpr<ABI::PTR_REG_0>().pAny;
                void* pTo   = Interpreter::gpr<ABI::PTR_REG_1>().pAny;
                if (pFrom && pTo) {
                    std::memcpy(pTo, pFrom, uSize);
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
                } else {
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
                }
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
            }
            break;

        case FILL_BYTE: fill<uint8>();  break;
        case FILL_WORD: fill<uint16>(); break;
        case FILL_LONG: fill<uint32>(); break;
        case FILL_QUAD: fill<uint64>(); break;
        case FIND_BYTE: find<uint8>();  break;
        case FIND_WORD: find<uint16>(); break;
        case FIND_LONG: find<uint32>(); break;
        case FIND_QUAD: find<uint64>(); break;

        case STR_LENGTH: {
            char const* sString = Interpreter::gpr<ABI::PTR_REG_0>().sString;
            if (sString) {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = std::strlen(sString);
                Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NONE;
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = 0;
                Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NULL_PTR;
            }
            break;
        }
        case STR_COMPARE: {
            char const* sString1 = Interpreter::gpr<ABI::PTR_REG_0>().sString;
            char const* sString2 = Interpreter::gpr<ABI::PTR_REG_1>().sString;
            if (sString1 && sString2) {
                Interpreter::gpr<ABI::INT_REG_0>().iQuad = std::strcmp(sString1, sString2);
                Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NONE;
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = 0;
                Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NULL_PTR;
            }
            break;
        }

        default:
            std::fprintf(stderr, "Unknown Mem operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

} // namespace

