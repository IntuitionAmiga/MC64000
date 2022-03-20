#ifndef MC64K_MACHINE_REGISTER_HPP
    #define MC64K_MACHINE_REGISTER_HPP

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

#include <type_traits>
#include <misc/scalar.hpp>

namespace MC64K::Machine {

/**
 * GPRegister
 *
 * Basic general purpose (integer / addressing) register definition. Direct access to member fields is allowed
 * and generic accessors are provided for access from within other generic functions.
 */
union GPRegister {
    public:
        enum Names {
            R0   = 0,
            R1   = 1,
            R2   = 2,
            R3   = 3,
            R4   = 4,
            R5   = 5,
            R6   = 6,
            R7   = 7,
            R8   = 8,
            R9   = 9,
            R10  = 10,
            R11  = 11,
            R12  = 12,
            R13  = 13,
            R14  = 14,
            R15  = 15,

            D0   = R0,
            D1   = R1,
            D2   = R2,
            D3   = R3,
            D4   = R4,
            D5   = R5,
            D6   = R6,
            D7   = R7,
            A0   = R8,
            A1   = R9,
            A2   = R10,
            A3   = R11,
            A4   = R12,
            A5   = R13,
            A6   = R14,
            A7   = R15,
            SP   = R15,
            MAX  = 16,
            MASK = 0xF
        };

        void*    pAny;
        uint64*  puQuad;
        int64*   piQuad;
        uint32*  puLong;
        int32*   piLong;
        uint16*  puWord;
        int16*   piWord;
        uint8*   puByte;
        int8*    piByte;
        float64* pfDouble;
        float32* pfSingle;

        uint64   uQuad;
        int64    iQuad;
        uint32   uLong;
        int32    iLong;
        uint16   uWord;
        int16    iWord;
        uint8    uByte;
        int8     iByte;
        char*    pChar;
        char const* sString;

        GPRegister() : uQuad(0) {}

    /**
     * For template accessors where the operand size is a template parameter:
     *
     * template<typename T> void example(GPRegister& register, T iValue) {
     *     register.value<T>() = iValue;
     * }
     *
     * T must be an integer type.
     */
    template<typename T>
    inline T& value() {
        static_assert(std::is_integral<T>::value, "Invalid template type for integer register access");
        if constexpr(std::is_same<uint64, T>::value) {
            return uQuad;
        } else if constexpr(std::is_same<uint32, T>::value) {
            return uLong;
        } else if constexpr(std::is_same<uint16, T>::value) {
            return uWord;
        } else if constexpr(std::is_same<uint8, T>::value) {
            return uByte;
        } else if constexpr(std::is_same<int64, T>::value) {
            return iQuad;
        } else if constexpr(std::is_same<int32, T>::value) {
            return iLong;
        } else if constexpr(std::is_same<int16, T>::value) {
            return iWord;
        } else if constexpr(std::is_same<int8, T>::value) {
            return iByte;
        }
    }

    /**
     * For template accessors where the register size access is a template parameter:
     *
     */
    template<typename T>
    inline T* address() const {
        return (T*)pAny;
    }
};

/**
 * FPRegister
 *
 * Floating Point register definition.
 */
union FPRegister {
    public:
        enum Names {
            FP0  = 0,
            FP1  = 1,
            FP2  = 2,
            FP3  = 3,
            FP4  = 4,
            FP5  = 5,
            FP6  = 6,
            FP7  = 7,
            FP8  = 8,
            FP9  = 9,
            FP10 = 10,
            FP11 = 11,
            FP12 = 12,
            FP13 = 13,
            FP14 = 14,
            FP15 = 15,
            MAX  = 16,
            MASK = 0xF
        };
        uint64  uBinary;
        float64 fDouble;
        float32 fSingle;
        FPRegister() : fDouble(0.0) {}

    /**
     * For template accessors, where the operand size is the template parameter:
     *
     * template<typename T> void example(FPRegister& register, T fValue) {
     *     register.value<T>() = fValue;
     * }
     *
     * T must be a floating point type.
     */
    template<typename T>
    inline T& value() {
        static_assert(std::is_floating_point<T>::value, "Invalid template type for floating point register access");
        if constexpr(std::is_same<float64, T>::value) {
            return fDouble;
        } else if constexpr(std::is_same<float32, T>::value) {
            return fSingle;
        }
    }
};

} // namespace
#endif
