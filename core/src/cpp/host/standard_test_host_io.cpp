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
#include "host/standard_test_host.hpp"
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
 * Open a stream
 */
void openStream(Machine::GPRegister* aoGPR) {
    unsigned iMode = aoGPR[Machine::GPRegister::R0].uByte;

    // Pessimism...
    aoGPR[Machine::GPRegister::R0].uQuad = ERR_OPEN;
    if (iMode > MODE_APPEND_UPDATE) {
        aoGPR[Machine::GPRegister::R8].sString = 0;
    } else {
        std::fprintf(
            stderr,
            "Attempting to open %s in %s...\n",
            aoGPR[Machine::GPRegister::R8].sString,
            aOpenModes[iMode]
        );

        std::FILE* pStream = std::fopen(
            aoGPR[Machine::GPRegister::R8].sString,
            aOpenModes[iMode]
        );
        if (pStream) {
            aoGPR[Machine::GPRegister::R8].pAny  = pStream;
            aoGPR[Machine::GPRegister::R0].uQuad = ERR_NONE;
        }
    }
};

void closeStream(Machine::GPRegister* aoGPR) {
    std::FILE* pStream = (std::FILE*)aoGPR[Machine::GPRegister::R8].pAny;
    if (pStream) {
        std::fclose(pStream);
        aoGPR[Machine::GPRegister::R8].pAny  = 0;
        aoGPR[Machine::GPRegister::R0].uQuad = ERR_NONE;
    } else {
        aoGPR[Machine::GPRegister::R0].uQuad = ERR_CLOSE;
    }
}

void readStream(Machine::GPRegister* aoGPR) {
    std::FILE* pStream = (std::FILE*)aoGPR[Machine::GPRegister::R8].pAny;
    uint8*     pBuffer = aoGPR[Machine::GPRegister::R9].puByte;
    size_t     uSize   = aoGPR[Machine::GPRegister::R0].uQuad;
    if (pStream && pBuffer) {

        aoGPR[Machine::GPRegister::R0].uQuad = std::fread(
            pBuffer,
            1,
            uSize,
            pStream
        );
        aoGPR[Machine::GPRegister::R1].uQuad = aoGPR[Machine::GPRegister::R0].uQuad == uSize ? ERR_NONE : ERR_READ;
    } else {
        aoGPR[Machine::GPRegister::R0].uQuad = 0;
        aoGPR[Machine::GPRegister::R1].uQuad = ERR_READ;
    }
}

void writeStream(Machine::GPRegister* aoGPR) {
    std::FILE* pStream = (std::FILE*)aoGPR[Machine::GPRegister::R8].pAny;
    uint8*     pBuffer = aoGPR[Machine::GPRegister::R9].puByte;
    size_t     uSize   = aoGPR[Machine::GPRegister::R0].uQuad;
    if (pStream && pBuffer) {
        aoGPR[Machine::GPRegister::R0].uQuad = std::fwrite(
            pBuffer,
            1,
            uSize,
            pStream
        );
        aoGPR[Machine::GPRegister::R1].uQuad = aoGPR[Machine::GPRegister::R0].uQuad == uSize ? ERR_NONE : ERR_WRITE;
    } else {
        aoGPR[Machine::GPRegister::R0].uQuad = 0;
        aoGPR[Machine::GPRegister::R1].uQuad = ERR_WRITE;
    }
}

/**
 * Basic IO
 *
 * The VM has put the byte value of the enumeration on the stack and triggered our HCF vector.
 *
 */
Interpreter::Status hook() {

    Machine::GPRegister* aoGPR = Interpreter::gpr();

    Call iOperation = (Call) *aoGPR[Machine::GPRegister::SP].piByte;
    switch (iOperation) {
        case INIT:
        case DONE:
            break;

        case PRINT_STRING:
            std::fputs(aoGPR[Machine::GPRegister::R8].sString, stdout);
            break;

        case PRINT_BYTE:
            std::printf(sByteFormat, aoGPR[Machine::GPRegister::R8].iByte);
            break;

        case PRINT_WORD:
            std::printf(sWordFormat, aoGPR[Machine::GPRegister::R0].iWord);
            break;

        case PRINT_LONG:
            std::printf(sLongFormat, aoGPR[Machine::GPRegister::R0].iLong);
            break;

        case PRINT_QUAD:
            std::printf(sQuadFormat, aoGPR[Machine::GPRegister::R0].iQuad);
            break;

        case PRINT_SINGLE: {
                Machine::FPRegister* aoFPR = Interpreter::fpr();
                std::printf(sSingleFormat, (double)aoFPR[Machine::FPRegister::FP0].fSingle);
            }
            break;

        case PRINT_DOUBLE: {
                Machine::FPRegister* aoFPR = Interpreter::fpr();
                std::printf(sDoubleFormat, (double)aoFPR[Machine::FPRegister::FP0].fDouble);
            }
            break;

        case SET_FMT_BYTE:
            sByteFormat = aoGPR[Machine::GPRegister::R8].sString;
            break;

        case SET_FMT_WORD:
            sWordFormat = aoGPR[Machine::GPRegister::R8].sString;
            break;

        case SET_FMT_LONG:
            sLongFormat = aoGPR[Machine::GPRegister::R8].sString;
            break;

        case SET_FMT_QUAD:
            sQuadFormat = aoGPR[Machine::GPRegister::R8].sString;
            break;

        case SET_FMT_SINGLE:
            sSingleFormat = aoGPR[Machine::GPRegister::R8].sString;
            break;

        case SET_FMT_DOUBLE:
            sDoubleFormat = aoGPR[Machine::GPRegister::R8].sString;
            break;

        case CLR_FMT_BYTE:
            sByteFormat = sDefaultByteFormat;
            break;

        case CLR_FMT_WORD:
            sWordFormat = sDefaultWordFormat;
            break;

        case CLR_FMT_LONG:
            sLongFormat = sDefaultLongFormat;
            break;

        case CLR_FMT_QUAD:
            sQuadFormat = sDefaultQuadFormat;
            break;

        case CLR_FMT_SINGLE:
            sSingleFormat = sDefaultSingleFormat;
            break;

        case CLR_FMT_DOUBLE:
            sDoubleFormat = sDefaultDoubleFormat;
            break;


        case FILE_OPEN:
            openStream(aoGPR);
            break;

//         case FILE_SEEK:
//             break;
//
//         case FILE_TELL:
//             break;

        case FILE_READ:
            readStream(aoGPR);
            break;

        case FILE_WRITE:
            writeStream(aoGPR);
            break;

        case FILE_CLOSE:
            closeStream(aoGPR);
            break;

        default:
            std::fprintf(stderr, "Unknown IO operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

}}} // namespace

