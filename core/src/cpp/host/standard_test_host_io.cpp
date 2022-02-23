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
#include <host/standard_test_host_io.hpp>
#include <machine/register.hpp>
#include <host/io/inline.hpp>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::IO {

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

/**
 * Attempts to open the file with name pointed to by <ABI::PTR_REG_0> in the access mode specified by
 * <ABI::INT_REG_0>.b. On success, the stream handle is returned in <ABI::PTR_REG_0> and a success return in
 * <ABI::INT_REG_0>.q. Otherwise, null is returned in <ABI::PTR_REG_0> and an error in <ABI::INT_REG_0>.q.
 */
void openStream() {
    unsigned iMode = Interpreter::gpr<ABI::INT_REG_0>().uByte;

    // Pessimism...
    Interpreter::gpr<ABI::INT_REG_0>().uQuad = ERR_OPEN;
    if (iMode > OPEN_APPEND_UPDATE) {
        Interpreter::gpr<ABI::PTR_REG_0>().sString = 0;
    } else {
        std::FILE* pStream = std::fopen(
            Interpreter::gpr<ABI::PTR_REG_0>().sString,
            aOpenModes[iMode]
        );
        if (pStream) {
            Interpreter::gpr<ABI::PTR_REG_0>().pAny  = pStream;
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
        } else {
            Interpreter::gpr<ABI::PTR_REG_0>().pAny  = 0;
        }
    }
};

/**
 * Attempts to close the stream pointed to by <ABI::PTR_REG_0>. On success, sets <ABI::PTR_REG_0> to null and returns
 * a success indicator in <ABI::INT_REG_0>.q. Otherwise an error is returned in <ABI::INT_REG_0>.q
 */
void closeStream() {
    if (std::FILE* pStream = Interpreter::gpr<ABI::PTR_REG_0>().address<std::FILE>()) {
        std::fclose(pStream);
        Interpreter::gpr<ABI::PTR_REG_0>().pAny  = 0;
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
    }
}

/**
 * Reads up to <ABI::INT_REG_0>.q bytes from the stream pointed to by <ABI::PTR_REG_0> into the buffer pointed to by
 * <ABI::PTR_REG_1>. Returns the number of bytes read in <ABI::INT_REG_0>.q and an overall indicator of success/failure
 * in <ABI::INT_REG_1>.
 */
void readStream() {
    std::FILE* pStream = Interpreter::gpr<ABI::PTR_REG_0>().address<std::FILE>();
    uint8*     pBuffer = Interpreter::gpr<ABI::PTR_REG_1>().puByte;
    if (pStream && pBuffer) {
        if (size_t uSize = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = std::fread(
                pBuffer,
                1,
                uSize,
                pStream
            );
            Interpreter::gpr<ABI::INT_REG_1>().uQuad = Interpreter::gpr<ABI::INT_REG_0>().uQuad == uSize ?
                (uint64)ABI::ERR_NONE :
                (uint64)ERR_READ;
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = 0;
            Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_BAD_SIZE;
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = 0;
        Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NULL_PTR;
    }
}

/**
 * Writes up to <ABI::INT_REG_0>.q bytes from the buffer pointed to by <ABI::PTR_REG_1> to the stream pointed to by
 * <ABI::PTR_REG_0>. Returns the number of bytes read in <ABI::INT_REG_0>.q and an overall indicator of success/failure
 * in <ABI::INT_REG_1>.
 */
void writeStream() {
    std::FILE* pStream = Interpreter::gpr<ABI::PTR_REG_0>().address<std::FILE>();
    uint8*     pBuffer = Interpreter::gpr<ABI::PTR_REG_1>().puByte;
    if (pStream && pBuffer) {
        if (size_t uSize = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = std::fwrite(
                pBuffer,
                1,
                uSize,
                pStream
            );
            Interpreter::gpr<ABI::INT_REG_1>().uQuad = Interpreter::gpr<ABI::INT_REG_0>().uQuad == uSize ?
                (uint64)ABI::ERR_NONE :
                (uint64)ERR_WRITE;
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = 0;
            Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_BAD_SIZE;
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = 0;
        Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NULL_PTR;
    }
}

/**
 *
 */
void seekStream() {
    if (std::FILE* pStream = Interpreter::gpr<ABI::PTR_REG_0>().address<std::FILE>()) {
        int64      iOffset = Interpreter::gpr<ABI::INT_REG_0>().iQuad;
        unsigned   iMode   = Interpreter::gpr<ABI::INT_REG_1>().uByte;
        if (
            iMode <= FROM_END &&
            0 == std::fseek(pStream, iOffset, aSeekModes[iMode])
        ) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ERR_FILE;
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
    }
}

/**
 * Get location within a stream. Split out of hook().
 */
void tellStream() {
    if (std::FILE* pStream = Interpreter::gpr<ABI::PTR_REG_0>().address<std::FILE>()) {
        Interpreter::gpr<ABI::INT_REG_0>().iQuad = std::ftell(pStream);
        Interpreter::gpr<ABI::INT_REG_1>().uQuad = Interpreter::gpr<ABI::INT_REG_0>().iQuad >= 0 ?
            (uint64)ABI::ERR_NONE :
            (uint64)ERR_FILE;
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().iQuad = -1;
        Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NULL_PTR;
    }
}

void print() {
    if (char const* pText = Interpreter::gpr<ABI::PTR_REG_0>().sString) {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOWriteResult(
            std::fputs(pText, stdout)
        );
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
    }
}

void filePrint() {
    std::FILE*  pStream = Interpreter::gpr<ABI::PTR_REG_0>().address<std::FILE>();
    char const* pText   = Interpreter::gpr<ABI::PTR_REG_1>().sString;
    if (pStream && pText) {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOWriteResult(
            std::fputs(pText, pStream)
        );
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
    }
}

/**
 * IO::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {
    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {
        case INIT:
        case DONE:
            break;

        case PRINT_STRING:       print();                                        break;
        case PRINT_BYTE:         print<int8>(sByteFormat);                       break;
        case PRINT_WORD:         print<int16>(sWordFormat);                      break;
        case PRINT_LONG:         print<int32>(sLongFormat);                      break;
        case PRINT_QUAD:         print<int64>(sQuadFormat);                      break;
        case PRINT_SINGLE:       print<float32>(sSingleFormat);                  break;
        case PRINT_DOUBLE:       print<float64>(sDoubleFormat);                  break;
        case SET_FMT_BYTE:       setFormat(sByteFormat, sDefaultByteFormat);     break;
        case SET_FMT_WORD:       setFormat(sWordFormat, sDefaultWordFormat);     break;
        case SET_FMT_LONG:       setFormat(sLongFormat, sDefaultLongFormat);     break;
        case SET_FMT_QUAD:       setFormat(sQuadFormat, sDefaultQuadFormat);     break;
        case SET_FMT_SINGLE:     setFormat(sSingleFormat, sDefaultSingleFormat); break;
        case SET_FMT_DOUBLE:     setFormat(sDoubleFormat, sDefaultDoubleFormat); break;
        case FILE_OPEN:          openStream();                                   break;
        case FILE_SEEK:          seekStream();                                   break;
        case FILE_TELL:          tellStream();                                   break;
        case FILE_READ:          readStream();                                   break;
        case FILE_WRITE:         writeStream();                                  break;
        case FILE_CLOSE:         closeStream();                                  break;
        case FILE_PRINT_STRING:  filePrint();                                    break;
        case FILE_PRINT_BYTE:    filePrint<int8>(sByteFormat);                   break;
        case FILE_PRINT_WORD:    filePrint<int16>(sWordFormat);                  break;
        case FILE_PRINT_LONG:    filePrint<int32>(sLongFormat);                  break;
        case FILE_PRINT_QUAD:    filePrint<int64>(sQuadFormat);                  break;
        case FILE_PRINT_SINGLE:  filePrint<float32>(sSingleFormat);              break;
        case FILE_PRINT_DOUBLE:  filePrint<float64>(sDoubleFormat);              break;
        case FILE_PARSE_BYTE:    fileParse<int8>(sByteFormat);                   break;
        case FILE_PARSE_WORD:    fileParse<int16>(sWordFormat);                  break;
        case FILE_PARSE_LONG:    fileParse<int32>(sLongFormat);                  break;
        case FILE_PARSE_QUAD:    fileParse<int64>(sQuadFormat);                  break;
        case FILE_PARSE_SINGLE:  fileParse<float32>(sSingleFormat);              break;
        case FILE_PARSE_DOUBLE:  fileParse<float64>(sDoubleFormat);              break;
        case CBUF_FORMAT_BYTE:   format<int8>(sByteFormat);                      break;
        case CBUF_FORMAT_WORD:   format<int16>(sWordFormat);                     break;
        case CBUF_FORMAT_LONG:   format<int32>(sLongFormat);                     break;
        case CBUF_FORMAT_QUAD:   format<int64>(sQuadFormat);                     break;
        case CBUF_FORMAT_SINGLE: format<float32>(sSingleFormat);                 break;
        case CBUF_FORMAT_DOUBLE: format<float64>(sDoubleFormat);                 break;
        case CBUF_PARSE_BYTE:    parse<int8>(sByteFormat);                       break;
        case CBUF_PARSE_WORD:    parse<int16>(sWordFormat);                      break;
        case CBUF_PARSE_LONG:    parse<int32>(sLongFormat);                      break;
        case CBUF_PARSE_QUAD:    parse<int64>(sQuadFormat);                      break;
        case CBUF_PARSE_SINGLE:  parse<float32>(sSingleFormat);                  break;
        case CBUF_PARSE_DOUBLE:  parse<float64>(sDoubleFormat);                  break;

        default:
            std::fprintf(stderr, "Unknown IO operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

} // namespace
