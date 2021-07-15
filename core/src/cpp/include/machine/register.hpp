#ifndef __MC64K_MACHINE_REGISTER_HPP__
#   ifndef __MC64K_MACHINE_HPP__
#       error "Header not included at correct scope"
#   endif
#   define __MC64K_MACHINE_REGISTER_HPP__

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

/**
 * MC64K::Register
 */
namespace Register {

    union GeneralPurpose {
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
        uint64*  pUQuad;
        int64*   pIQuad;
        uint32*  pULong;
        int32*   pILong;
        uint16*  pUWord;
        int16*   pIWord;
        uint8*   pUByte;
        int8*    pIByte;
        float64* pDouble;
        float32* pFloat;

        uint64   uQuad;
        int64    iQuad;
        uint32   uLong;
        int32    iLong;
        uint16   uWord;
        int16    iWord;
        uint8    uByte;
        int8     iByte;
        GeneralPurpose() : uQuad(0) {}
    };

    union FloatingPoint {
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
        FloatingPoint() : fDouble(0.0) {}
    };
};

#endif