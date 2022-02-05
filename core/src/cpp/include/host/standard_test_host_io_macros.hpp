#ifndef __MC64K_STANDARD_TEST_HOST_IO_MACROS_HPP__
    #define __MC64K_STANDARD_TEST_HOST_IO_MACROS_HPP__

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

namespace MC64K {
namespace StandardTestHost {
namespace IO {

/**
 * Gnarly
 */
#define setIOWriteResult(iResult, iReg) aoGPR[(iReg)].uQuad = (iResult) >= 0 ? (unsigned)ABI::ERR_NONE : (unsigned)ERR_WRITE;

#define setIOReadResult(iResult, iReg) aoGPR[(iReg)].uQuad = (iResult) >= 0 ? (unsigned)ABI::ERR_NONE : (unsigned)ERR_READ;

#define getFormat(iReg, sDef) aoGPR[(iReg)].sString ? aoGPR[(iReg)].sString : (sDef)

#define setFormat(sFmt, sDef) sFmt = aoGPR[ABI::PTR_REG_0].sString ? aoGPR[ABI::PTR_REG_0].sString : sDef;


/**
 * Macro for printing integers to stdout
 */
#define printInt(format, size) { \
    setIOWriteResult( \
        std::printf( \
            getFormat(ABI::PTR_REG_0, format), \
            aoGPR[ABI::INT_REG_0]. size \
        ), \
        ABI::INT_REG_0 \
    ) \
}

/**
 * Macro for printing floats to stdout
 */
#define printFloat(format, size) { \
    Machine::FPRegister* aoFPR = Interpreter::fpr(); \
    setIOWriteResult( \
        std::printf( \
            getFormat(ABI::PTR_REG_0, format), \
            aoFPR[ABI::INT_REG_0]. size \
        ), \
        ABI::INT_REG_0 \
    ) \
}

/**
 * Macro for printing integers to an open stream
 */
#define filePrintInt(format, size) { \
    if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) { \
        setIOWriteResult( \
            std::fprintf( \
                pStream, \
                getFormat(ABI::PTR_REG_1, format), \
                aoGPR[ABI::INT_REG_0]. size \
            ), \
            ABI::INT_REG_0 \
        ); \
    } else { \
        aoGPR[ABI::INT_REG_0].uQuad =  ABI::ERR_NULL_PTR; \
    } \
}

/**
 * Macro for printing floats to an open stream
 */
#define filePrintFloat(format, size) { \
    if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) { \
        Machine::FPRegister* aoFPR = Interpreter::fpr(); \
        setIOWriteResult( \
            std::fprintf( \
                pStream, \
                getFormat(ABI::PTR_REG_1, format), \
                aoFPR[ABI::INT_REG_0]. size \
            ), \
            ABI::INT_REG_0 \
        ); \
    } else { \
        aoGPR[ABI::INT_REG_0].uQuad =  ABI::ERR_NULL_PTR; \
    } \
}

/**
 * Macro for parsing integers from a text stream
 */
#define fileParseInt(format, size) { \
    if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) { \
        setIOReadResult( \
            std::fscanf( \
                pStream, \
                getFormat(ABI::PTR_REG_1, format), \
                &aoGPR[ABI::INT_REG_0]. size \
            ), \
            ABI::INT_REG_0 \
        ); \
    } else { \
        aoGPR[ABI::INT_REG_0].uQuad = ABI::ERR_NULL_PTR; \
    } \
}

/**
 * Macro for parsing floats from a text stream
 */
#define fileParseFloat(format, size) { \
    if ( std::FILE* pStream = (std::FILE*)aoGPR[ABI::PTR_REG_0].pAny ) { \
        Machine::FPRegister* aoFPR = Interpreter::fpr(); \
        setIOReadResult( \
            std::fscanf( \
                pStream, \
                getFormat(ABI::PTR_REG_1, format), \
                &aoFPR[ABI::INT_REG_0]. size \
            ), \
            ABI::INT_REG_0 \
        ); \
    } else { \
        aoGPR[ABI::INT_REG_0].uQuad =  ABI::ERR_NULL_PTR; \
    } \
}

/**
 * Macro for formatting integers into a character buffer
 */
#define formatInt(format, size) { \
    if ( char* sBuffer = aoGPR[ABI::PTR_REG_0].pChar ) { \
        setIOWriteResult( \
            std::sprintf( \
                sBuffer, \
                getFormat(ABI::PTR_REG_1, format), \
                aoGPR[ABI::INT_REG_0]. size \
            ), \
            ABI::INT_REG_0 \
        ); \
    } else { \
        aoGPR[ABI::INT_REG_0].uQuad = ABI::ERR_NULL_PTR; \
    } \
}

/**
 * Macro for parsing integers from a character buffer
 */
#define parseInt(format, size) { \
    if ( char const* sBuffer = aoGPR[ABI::PTR_REG_0].sString ) { \
        setIOReadResult( \
            std::sscanf( \
                sBuffer, \
                getFormat(ABI::PTR_REG_1, format), \
                &aoGPR[ABI::INT_REG_0]. size \
            ), \
            ABI::INT_REG_0 \
        ); \
    } else { \
        aoGPR[ABI::INT_REG_0].uQuad = ABI::ERR_NULL_PTR; \
    } \
}

/**
 * Macro for formatting floats into a character buffer
 */
#define formatFloat(format, size) { \
    if ( char* sBuffer = aoGPR[ABI::PTR_REG_0].pChar ) { \
        Machine::FPRegister* aoFPR = Interpreter::fpr(); \
        setIOWriteResult( \
            std::sprintf( \
                sBuffer, \
                getFormat(ABI::PTR_REG_1, format), \
                (double)aoFPR[ABI::FLT_REG_0]. size \
            ), \
            ABI::INT_REG_0 \
        ); \
    } else { \
        aoGPR[ABI::INT_REG_0].uQuad =  ABI::ERR_NULL_PTR; \
    } \
}

/**
 * Macro for parsing floats in a character buffer
 */
#define parseFloat(format, size) { \
    if ( char const* sBuffer = aoGPR[ABI::PTR_REG_0].sString ) { \
        Machine::FPRegister* aoFPR = Interpreter::fpr(); \
        setIOReadResult( \
            std::sscanf( \
                sBuffer, \
                getFormat(ABI::PTR_REG_1, format), \
                &aoFPR[ABI::INT_REG_0]. size \
            ), \
            ABI::INT_REG_0 \
        ); \
    } else { \
        aoGPR[ABI::INT_REG_0].uQuad =  ABI::ERR_NULL_PTR; \
    } \
}


}}} // namespace
#endif
