#ifndef MC64K_STANDARD_TEST_HOST_IO_INLINE_HPP
    #define MC64K_STANDARD_TEST_HOST_IO_INLINE_HPP

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
#include <machine/register.hpp>
#include <host/standard_test_host.hpp>
#include <host/standard_test_host_io.hpp>

namespace MC64K::StandardTestHost::IO {

inline constexpr uint64 getIOWriteResult(int iResult) {
    return (iResult >= 0) ? (uint64)ABI::ERR_NONE : (uint64)ERR_WRITE;
}

inline constexpr uint64 getIOReadResult(int iResult) {
    return (iResult) >= 0 ? (uint64)ABI::ERR_NONE : (uint64)ERR_READ;
}

inline constexpr char const* getTemplate(Machine::GPRegister const& oGPR, char const* sDefaultFormat) {
    return oGPR.sString ? oGPR.sString : sDefaultFormat;
}

inline void setFormat(char const*& sTemplate, char const* sDefaultFormat) {
    sTemplate = Interpreter::gpr<ABI::PTR_REG_0>().sString ?
        Interpreter::gpr<ABI::PTR_REG_0>().sString :
        sDefaultFormat;
}

/**
 * Prints the numeric type in d0 / fp0 to stdout using the optional formatting template in a0 or the provided
 * default.
 *
 * T can be any integer or floating point type.
 */
template<typename T>
inline void print(char const* sDefaultFormat) {
    static_assert(std::is_arithmetic<T>::value, "Invalid type for print<T>()");
    if constexpr (std::is_integral<T>::value) {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOWriteResult(
            std::printf(
                getTemplate(
                    Interpreter::gpr<ABI::PTR_REG_0>(),
                    sDefaultFormat
                ),
                Interpreter::gpr<ABI::INT_REG_0>().value<T>()
            )
        );
    } else if constexpr (std::is_floating_point<T>::value) {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOWriteResult(
            std::printf(
                getTemplate(
                    Interpreter::gpr<ABI::PTR_REG_0>(),
                    sDefaultFormat
                ),
                Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
            )
        );
    }
}

/**
 * Prints a numeric type in d0 / fp0 to a file stream pointed to by a0 using the optional formatting
 * template in a1 or the provided default.
 *
 * T can be any integer or floating point type.
 */
template<typename T>
inline void filePrint(char const* sDefaultFormat) {
    static_assert(std::is_arithmetic<T>::value, "Invalid type for filePrint<T>()");
    if (std::FILE* pStream = Interpreter::gpr<ABI::PTR_REG_0>().address<std::FILE>()) {
        if constexpr (std::is_integral<T>::value) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOWriteResult(
                std::fprintf(
                    pStream,
                    getTemplate(
                        Interpreter::gpr<ABI::PTR_REG_1>(),
                        sDefaultFormat
                    ),
                    Interpreter::gpr<ABI::INT_REG_0>().value<T>()
                )
            );
        } else if constexpr (std::is_floating_point<T>::value) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOWriteResult(
                std::fprintf(
                    pStream,
                    getTemplate(
                        Interpreter::gpr<ABI::PTR_REG_1>(),
                        sDefaultFormat
                    ),
                    Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
                )
            );
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
    }
}

/**
 * Prints a numeric type in d0 / fp0 to a buffer pointed to by a0 using the optional formatting
 * template in a1 or the provided default.
 *
 * T can be any integer or floating point type.
 */
template<typename T>
inline void format(char const* sDefaultFormat) {
    static_assert(std::is_arithmetic<T>::value, "Invalid type for format<T>()");
    if (char* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().address<char>()) {
        if constexpr (std::is_integral<T>::value) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOWriteResult(
                std::sprintf(
                    pBuffer,
                    getTemplate(
                        Interpreter::gpr<ABI::PTR_REG_1>(),
                        sDefaultFormat
                    ),
                    Interpreter::gpr<ABI::INT_REG_0>().value<T>()
                )
            );
        } else if constexpr (std::is_floating_point<T>::value) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOWriteResult(
                std::sprintf(
                    pBuffer,
                    getTemplate(
                        Interpreter::gpr<ABI::PTR_REG_1>(),
                        sDefaultFormat
                    ),
                    Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
                )
            );
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
    }
}

template<typename T>
inline void parse(char const* sDefaultFormat) {
    static_assert(std::is_arithmetic<T>::value, "Invalid type for parse<T>()");
    if (char const* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().address<char const>()) {
        if constexpr (std::is_integral<T>::value) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOReadResult(
                std::sscanf(
                    pBuffer,
                    getTemplate(
                        Interpreter::gpr<ABI::PTR_REG_1>(),
                        sDefaultFormat
                    ),
                    &(Interpreter::gpr<ABI::INT_REG_0>().value<T>())
                )
            );
        } else if constexpr (std::is_floating_point<T>::value) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOReadResult(
                std::sscanf(
                    pBuffer,
                    getTemplate(
                        Interpreter::gpr<ABI::PTR_REG_1>(),
                        sDefaultFormat
                    ),
                    &(Interpreter::fpr<ABI::FLT_REG_0>().value<T>())
                )
            );
        }
    }
}

template<typename T>
inline void fileParse(char const* sDefaultFormat) {
    static_assert(std::is_arithmetic<T>::value, "Invalid type for fileParse<T>()");
    if (std::FILE* pStream = Interpreter::gpr<ABI::PTR_REG_0>().address<std::FILE>()) {
        if constexpr (std::is_integral<T>::value) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOReadResult(
                std::fscanf(
                    pStream,
                    getTemplate(
                        Interpreter::gpr<ABI::PTR_REG_1>(),
                        sDefaultFormat
                    ),
                    &(Interpreter::gpr<ABI::INT_REG_0>().value<T>())
                )
            );
        } else if constexpr (std::is_floating_point<T>::value) {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = getIOReadResult(
                std::fscanf(
                    pStream,
                    getTemplate(
                        Interpreter::gpr<ABI::PTR_REG_1>(),
                        sDefaultFormat
                    ),
                    &(Interpreter::fpr<ABI::FLT_REG_0>().value<T>())
                )
            );
        }
    }
}

}
#endif
