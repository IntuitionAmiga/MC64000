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
#include "host/standard_test_host_io.hpp"
#include "machine/register.hpp"

using MC64K::Machine::Interpreter;

namespace MC64K {
namespace StandardTestHost {
namespace IO {

/**
 * Default formatters
 */
char const* sDefaultByteFormat   = "%c";
char const* sDefaultWordFormat   = "%" PFS16;
char const* sDefaultLongFormat   = "%" PFS32;
char const* sDefaultQuadFormat   = "%" PFS64;
char const* sDefaultSingleFormat = "%g";
char const* sDefaultDoubleFormat = "%g";

/**
 * Active formatters
 */
char const* sByteFormat   = sDefaultByteFormat;
char const* sWordFormat   = sDefaultWordFormat;
char const* sLongFormat   = sDefaultLongFormat;
char const* sQuadFormat   = sDefaultQuadFormat;
char const* sSingleFormat = sDefaultSingleFormat;
char const* sDoubleFormat = sDefaultDoubleFormat;

/**
 * File open modes
 */
char const* aOpenModes[6] = {"r", "w", "a", "r+", "w+", "a+"};

/**
 * File seek modes
 */
int aSeekModes[3] = { SEEK_SET, SEEK_CUR, SEEK_END };

#define setIOWriteResult(iResult, iReg) aoGPR[(iReg)].uQuad = (iResult) >= 0 ? (unsigned)ABI::ERR_NONE : (unsigned)ERR_WRITE;

/**
 * Open a stream. Split out of hook().
 *
 * @param Machine::GPRegister* aoGPR
 */
void openStream(Machine::GPRegister* aoGPR) {
    unsigned iMode = aoGPR[ABI::INT_REG_0].uByte;

    // Pessimism...
    aoGPR[ABI::INT_REG_0].uQuad = ERR_OPEN;
    if (iMode > OPEN_APPEND_UPDATE) {
        aoGPR[ABI::PTR_REG_0].sString = 0;
    } else {
        std::FILE* pStream = std::fopen(
            aoGPR[ABI::PTR_REG_0].sString,
            aOpenModes[iMode]
        );
        if (pStream) {
            aoGPR[ABI::PTR_REG_0].pAny  = pStream;
            aoGPR[ABI::INT_REG_0].uQuad = ABI::ERR_NONE;
        } else {
            aoGPR[ABI::PTR_REG_0].pAny  = 0;
        }
    }
};

/**
 * Close a stream. Split out of hook().
 *
 * @param Machine::GPRegister* aoGPR
 */
void closeStream(Machine::GPRegister* aoGPR) {
    std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny;
    if (pStream) {
        std::fclose(pStream);
        aoGPR[ABI::PTR_REG_0].pAny  = 0;
        aoGPR[ABI::INT_REG_0].uQuad = ABI::ERR_NONE;
    } else {
        aoGPR[ABI::INT_REG_0].uQuad = ABI::ERR_NULL_PTR;
    }
}

/**
 * Read from a stream. Split out of hook().
 *
 * @param Machine::GPRegister* aoGPR
 */
void readStream(Machine::GPRegister* aoGPR) {
    std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny;
    uint8*     pBuffer = aoGPR[ABI::PTR_REG_1].puByte;

    if (pStream && pBuffer) {
        size_t uSize = aoGPR[ABI::INT_REG_0].uQuad;

        if (uSize) {
            aoGPR[ABI::INT_REG_0].uQuad = std::fread(
                pBuffer,
                1,
                uSize,
                pStream
            );
            aoGPR[ABI::INT_REG_1].uQuad = aoGPR[ABI::INT_REG_0].uQuad == uSize ?
                (uint64)ABI::ERR_NONE :
                (uint64)ERR_READ;
        } else {
            aoGPR[ABI::INT_REG_0].uQuad = 0;
            aoGPR[ABI::INT_REG_1].uQuad = ABI::ERR_BAD_SIZE;
        }
    } else {
        aoGPR[ABI::INT_REG_0].uQuad = 0;
        aoGPR[ABI::INT_REG_1].uQuad = ABI::ERR_NULL_PTR;
    }
}

/**
 * Write to a stream. Split out of hook().
 *
 * @param Machine::GPRegister* aoGPR
 */
void writeStream(Machine::GPRegister* aoGPR) {
    std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny;
    uint8*     pBuffer = aoGPR[ABI::PTR_REG_1].puByte;

    if (pStream && pBuffer) {
        size_t uSize   = aoGPR[ABI::INT_REG_0].uQuad;

        if (uSize) {
            aoGPR[ABI::INT_REG_0].uQuad = std::fwrite(
                pBuffer,
                1,
                uSize,
                pStream
            );
            aoGPR[ABI::INT_REG_1].uQuad = aoGPR[ABI::INT_REG_0].uQuad == uSize ?
                (uint64)ABI::ERR_NONE :
                (uint64)ERR_WRITE;
        } else {
            aoGPR[ABI::INT_REG_0].uQuad = 0;
            aoGPR[ABI::INT_REG_1].uQuad = ABI::ERR_BAD_SIZE;
        }
    } else {
        aoGPR[ABI::INT_REG_0].uQuad = 0;
        aoGPR[ABI::INT_REG_1].uQuad = ABI::ERR_NULL_PTR;
    }
}

/**
 * Seek in a stream. Split out of hook().
 *
 * @param Machine::GPRegister* aoGPR
 */
void seekStream(Machine::GPRegister* aoGPR) {
    std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny;
    if (!pStream) {
        aoGPR[ABI::INT_REG_0].uQuad = ABI::ERR_NULL_PTR;
    } else {
        int64      iOffset = aoGPR[ABI::INT_REG_0].iQuad;
        unsigned   iMode   = aoGPR[ABI::INT_REG_1].uByte;

        if (
            iMode <= FROM_END &&
            0 == std::fseek(pStream, iOffset, aSeekModes[iMode])
        ) {
            aoGPR[ABI::INT_REG_0].uQuad = ABI::ERR_NONE;
        } else {
            aoGPR[ABI::INT_REG_0].uQuad = ERR_FILE;
        }
    }
}

/**
 * Get location within a stream. Split out of hook().
 *
 * @param Machine::GPRegister* aoGPR
 */
void tellStream(Machine::GPRegister* aoGPR) {
    std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny;
    if (pStream) {
        aoGPR[ABI::INT_REG_0].iQuad = std::ftell(pStream);
        aoGPR[ABI::INT_REG_1].uQuad = aoGPR[ABI::INT_REG_0].iQuad >= 0 ?
            (uint64)ABI::ERR_NONE :
            (uint64)ERR_FILE;
    } else {
        aoGPR[ABI::INT_REG_0].iQuad = -1;
        aoGPR[ABI::INT_REG_1].uQuad =  ABI::ERR_NULL_PTR;
    }
}

/**
 * IO::hostVector()
 *
 * The VM has put the byte value of the enumeration on the stack and triggered our HCF vector.
 *
 */
Interpreter::Status hostVector() {

    Machine::GPRegister* aoGPR = Interpreter::gpr();

    Call iOperation = (Call) *aoGPR[Machine::GPRegister::SP].piByte;
    switch (iOperation) {
        case INIT:
        case DONE:
            break;

        case PRINT_STRING: {
            const char *pText = aoGPR[ABI::PTR_REG_0].sString;
            if (pText) {
                setIOWriteResult(std::fputs(pText, stdout), ABI::INT_REG_0);
            } else {
                aoGPR[ABI::INT_REG_0].uQuad =  ABI::ERR_NULL_PTR;
            }
            break;
        }

        case PRINT_BYTE: {
            setIOWriteResult(std::printf(sByteFormat, aoGPR[ABI::INT_REG_0].iByte), ABI::INT_REG_0);
            break;
        }

        case PRINT_WORD: {
            setIOWriteResult(std::printf(sWordFormat, aoGPR[ABI::INT_REG_0].iWord), ABI::INT_REG_0);
            break;
        }

        case PRINT_LONG: {
            setIOWriteResult(std::printf(sLongFormat, aoGPR[ABI::INT_REG_0].iLong), ABI::INT_REG_0);
            break;
        }

        case PRINT_QUAD: {
            setIOWriteResult(std::printf(sQuadFormat, aoGPR[ABI::INT_REG_0].iQuad), ABI::INT_REG_0);
            break;
        }

        case PRINT_SINGLE: {
            Machine::FPRegister* aoFPR = Interpreter::fpr();
            setIOWriteResult(std::printf(sSingleFormat, (double)aoFPR[ABI::FLT_REG_0].fSingle), ABI::INT_REG_0);
            break;
        }

        case PRINT_DOUBLE: {
            Machine::FPRegister* aoFPR = Interpreter::fpr();
            setIOWriteResult(std::printf(sDoubleFormat, (double)aoFPR[ABI::FLT_REG_0].fDouble), ABI::INT_REG_0);
            break;
        }

        case SET_FMT_BYTE:
            sByteFormat = aoGPR[ABI::PTR_REG_0].sString ? aoGPR[ABI::PTR_REG_0].sString : sDefaultByteFormat;
            break;

        case SET_FMT_WORD:
            sWordFormat = aoGPR[ABI::PTR_REG_0].sString ? aoGPR[ABI::PTR_REG_0].sString : sDefaultWordFormat;
            break;

        case SET_FMT_LONG:
            sLongFormat = aoGPR[ABI::PTR_REG_0].sString ? aoGPR[ABI::PTR_REG_0].sString : sDefaultLongFormat;
            break;

        case SET_FMT_QUAD:
            sQuadFormat = aoGPR[ABI::PTR_REG_0].sString ? aoGPR[ABI::PTR_REG_0].sString : sDefaultQuadFormat;
            break;

        case SET_FMT_SINGLE:
            sSingleFormat = aoGPR[ABI::PTR_REG_0].sString ? aoGPR[ABI::PTR_REG_0].sString : sDefaultSingleFormat;
            break;

        case SET_FMT_DOUBLE:
            sDoubleFormat = aoGPR[ABI::PTR_REG_0].sString ? aoGPR[ABI::PTR_REG_0].sString : sDefaultDoubleFormat;
            break;

        case FILE_OPEN:
            openStream(aoGPR);
            break;

        case FILE_SEEK:
            seekStream(aoGPR);
            break;

        case FILE_TELL:
            tellStream(aoGPR);
            break;

        case FILE_READ:
            readStream(aoGPR);
            break;

        case FILE_WRITE:
            writeStream(aoGPR);
            break;

        case FILE_CLOSE:
            closeStream(aoGPR);
            break;

        case FILE_PRINT_STRING:
            if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) {
                std::fputs(aoGPR[ABI::PTR_REG_1].sString, pStream);
            }
            break;

        case FILE_PRINT_BYTE:
            if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) {
                std::fprintf(pStream, sByteFormat, aoGPR[ABI::INT_REG_0].iByte);
            }
            break;

        case FILE_PRINT_WORD:
            if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) {
                std::fprintf(pStream, sWordFormat, aoGPR[ABI::INT_REG_0].iWord);
            }
            break;

        case FILE_PRINT_LONG:
            if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) {
                std::fprintf(pStream, sLongFormat, aoGPR[ABI::INT_REG_0].iLong);
            }
            break;

        case FILE_PRINT_QUAD:
            if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) {
                std::fprintf(pStream, sQuadFormat, aoGPR[ABI::INT_REG_0].iQuad);
            }
            break;

        case FILE_PRINT_SINGLE:
            if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) {
                Machine::FPRegister* aoFPR = Interpreter::fpr();
                std::fprintf(pStream, sSingleFormat, (double)aoFPR[ABI::FLT_REG_0].fSingle);
            }
            break;

        case FILE_PRINT_DOUBLE:
            if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) {
                Machine::FPRegister* aoFPR = Interpreter::fpr();
                std::fprintf(pStream, sDoubleFormat, (double)aoFPR[ABI::FLT_REG_0].fDouble);
            }
            break;

        default:
            std::fprintf(stderr, "Unknown IO operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

}}} // namespace

