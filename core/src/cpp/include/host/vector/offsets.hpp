#ifndef MC64K_STANDARD_TEST_HOST_VECTOR_MATH_OFFSETS_HPP
    #define MC64K_STANDARD_TEST_HOST_VECTOR_MATH_OFFSETS_HPP

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

namespace MC64K::StandardTestHost::VectorMath {

enum VectorOffset {
    V_X = 0,
    V_Y = 1,
    V_Z = 2,
    V_W = 3,
};

enum Mat2x2Offset {
    M2_11 = 0,
    M2_12 = 1,
    M2_21 = 2,
    M2_22 = 3,
};

enum Mat3x3Offset {
    M3_11 = 0,
    M3_12 = 1,
    M3_13 = 2,
    M3_21 = 3,
    M3_22 = 4,
    M3_23 = 5,
    M3_31 = 6,
    M3_32 = 7,
    M3_33 = 8,
};

enum Mat4x4Offset {
    M4_11 =  0,
    M4_12 =  1,
    M4_13 =  2,
    M4_14 =  3,
    M4_21 =  4,
    M4_22 =  5,
    M4_23 =  6,
    M4_24 =  7,
    M4_31 =  8,
    M4_32 =  9,
    M4_33 = 10,
    M4_34 = 11,
    M4_41 = 12,
    M4_42 = 13,
    M4_43 = 14,
    M4_44 = 15,
};

} // namespace
#endif
