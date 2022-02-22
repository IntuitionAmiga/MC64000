#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_4X4_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_4X4_HPP__

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

#include <machine/register.hpp>
#include "matrix_common.hpp"

#ifdef MATRIX_FORCE_DOUBLE
#define MT T
#else
#define MT float64
#endif

namespace MC64K::StandardTestHost::VectorMath {

/**
 * Apply a Mat4x4 to an input set of Vec4.
 */
template<typename T>
inline void vec4_transform_4x4(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M4_11] * pfSrc[V_X] + pfM[M4_12] * pfSrc[V_Y] + pfM[M4_13] * pfSrc[V_Z] + pfM[M4_14] * pfSrc[V_W];
        *pfDst++ = pfM[M4_21] * pfSrc[V_X] + pfM[M4_22] * pfSrc[V_Y] + pfM[M4_23] * pfSrc[V_Z] + pfM[M4_24] * pfSrc[V_W];
        *pfDst++ = pfM[M4_31] * pfSrc[V_X] + pfM[M4_32] * pfSrc[V_Y] + pfM[M4_33] * pfSrc[V_Z] + pfM[M4_34] * pfSrc[V_W];
        *pfDst++ = pfM[M4_41] * pfSrc[V_X] + pfM[M4_42] * pfSrc[V_Y] + pfM[M4_43] * pfSrc[V_Z] + pfM[M4_44] * pfSrc[V_W];
        pfSrc += 4;
    }
}

/**
 * Mat4x4 multiplication. Internally, each term is calculated using double precision regardless
 * of whether or not we are doing 32-bit or 64-bit precision. This is to minimise the accumulation
 * of error, particularly for single precision matrices.
 *
 * C = A x B, A is by row, B is by column
 */
template<typename T>
inline void mat4x4_multiply(T* pfDst, T const* pfSrcA, T const* pfSrcB) {
    // Row 1
    // A: | 11 12 13 14 |   B: | 11 ... |
    //                         | 21 ... |
    //                         | 31 ... |
    //                         | 41 ... |
    pfDst[M4_11] = (T)(
        ((MT)pfSrcA[M4_11] * (MT)pfSrcB[M4_11]) +
        ((MT)pfSrcA[M4_12] * (MT)pfSrcB[M4_21]) +
        ((MT)pfSrcA[M4_13] * (MT)pfSrcB[M4_31]) +
        ((MT)pfSrcA[M4_14] * (MT)pfSrcB[M4_41])
    );

    // A: | 11 12 13 14 |   B: | ... 12 ... |
    //                         | ... 22 ... |
    //                         | ... 32 ... |
    //                         | ... 42 ... |
    pfDst[M4_12] = (T)(
        ((MT)pfSrcA[M4_11] * (MT)pfSrcB[M4_12]) +
        ((MT)pfSrcA[M4_12] * (MT)pfSrcB[M4_22]) +
        ((MT)pfSrcA[M4_13] * (MT)pfSrcB[M4_32]) +
        ((MT)pfSrcA[M4_14] * (MT)pfSrcB[M4_42])
    );

    // A: | 11 12 13 14 |   B: | ... 13 ... |
    //                         | ... 23 ... |
    //                         | ... 33 ... |
    //                         | ... 43 ... |
    pfDst[M4_13] = (T)(
        ((MT)pfSrcA[M4_11] * (MT)pfSrcB[M4_13]) +
        ((MT)pfSrcA[M4_12] * (MT)pfSrcB[M4_23]) +
        ((MT)pfSrcA[M4_13] * (MT)pfSrcB[M4_33]) +
        ((MT)pfSrcA[M4_14] * (MT)pfSrcB[M4_43])
    );

    // A: | 11 12 13 14 |   B: | ... 14 |
    //                         | ... 24 |
    //                         | ... 34 |
    //                         | ... 44 |
    pfDst[M4_14] = (T)(
        ((MT)pfSrcA[M4_11] * (MT)pfSrcB[M4_14]) +
        ((MT)pfSrcA[M4_12] * (MT)pfSrcB[M4_24]) +
        ((MT)pfSrcA[M4_13] * (MT)pfSrcB[M4_34]) +
        ((MT)pfSrcA[M4_14] * (MT)pfSrcB[M4_44])
    );

    // Row 2
    // A: | 21 22 23 24 |   B: | 11 ... |
    //                         | 21 ... |
    //                         | 31 ... |
    //                         | 41 ... |
    pfDst[M4_21] = (T)(
        ((MT)pfSrcA[M4_21] * (MT)pfSrcB[M4_11]) +
        ((MT)pfSrcA[M4_22] * (MT)pfSrcB[M4_21]) +
        ((MT)pfSrcA[M4_23] * (MT)pfSrcB[M4_31]) +
        ((MT)pfSrcA[M4_24] * (MT)pfSrcB[M4_41])
    );

    // A: | 21 22 23 24 |   B: | ... 12 ... |
    //                         | ... 22 ... |
    //                         | ... 32 ... |
    //                         | ... 42 ... |
    pfDst[M4_22] = (T)(
        ((MT)pfSrcA[M4_21] * (MT)pfSrcB[M4_12]) +
        ((MT)pfSrcA[M4_22] * (MT)pfSrcB[M4_22]) +
        ((MT)pfSrcA[M4_23] * (MT)pfSrcB[M4_32]) +
        ((MT)pfSrcA[M4_24] * (MT)pfSrcB[M4_42])
    );

    // A: | 21 22 23 24 |   B: | ... 13 ... |
    //                         | ... 23 ... |
    //                         | ... 33 ... |
    //                         | ... 43 ... |
    pfDst[M4_23] = (T)(
        ((MT)pfSrcA[M4_21] * (MT)pfSrcB[M4_13]) +
        ((MT)pfSrcA[M4_22] * (MT)pfSrcB[M4_23]) +
        ((MT)pfSrcA[M4_23] * (MT)pfSrcB[M4_33]) +
        ((MT)pfSrcA[M4_24] * (MT)pfSrcB[M4_43])
    );

    // A: | 21 22 23 24 |   B: | ... 14 |
    //                         | ... 24 |
    //                         | ... 34 |
    //                         | ... 44 |
    pfDst[M4_24] = (T)(
        ((MT)pfSrcA[M4_21] * (MT)pfSrcB[M4_14]) +
        ((MT)pfSrcA[M4_22] * (MT)pfSrcB[M4_24]) +
        ((MT)pfSrcA[M4_23] * (MT)pfSrcB[M4_34]) +
        ((MT)pfSrcA[M4_24] * (MT)pfSrcB[M4_44])
    );

    // Row 3
    // A: | 31 32 33 34 |   B: | 11 ... |
    //                         | 21 ... |
    //                         | 31 ... |
    //                         | 41 ... |
    pfDst[M4_31] = (T)(
        ((MT)pfSrcA[M4_31] * (MT)pfSrcB[M4_11]) +
        ((MT)pfSrcA[M4_32] * (MT)pfSrcB[M4_21]) +
        ((MT)pfSrcA[M4_33] * (MT)pfSrcB[M4_31]) +
        ((MT)pfSrcA[M4_34] * (MT)pfSrcB[M4_41])
    );

    // A: | 31 32 33 34 |   B: | ... 12 ... |
    //                         | ... 22 ... |
    //                         | ... 32 ... |
    //                         | ... 42 ... |
    pfDst[M4_32] = (T)(
        ((MT)pfSrcA[M4_31] * (MT)pfSrcB[M4_12]) +
        ((MT)pfSrcA[M4_32] * (MT)pfSrcB[M4_22]) +
        ((MT)pfSrcA[M4_33] * (MT)pfSrcB[M4_32]) +
        ((MT)pfSrcA[M4_34] * (MT)pfSrcB[M4_42])
    );

    // A: | 31 32 33 34 |   B: | ... 13 ... |
    //                         | ... 23 ... |
    //                         | ... 33 ... |
    //                         | ... 43 ... |
    pfDst[M4_33] = (T)(
        ((MT)pfSrcA[M4_31] * (MT)pfSrcB[M4_13]) +
        ((MT)pfSrcA[M4_32] * (MT)pfSrcB[M4_23]) +
        ((MT)pfSrcA[M4_33] * (MT)pfSrcB[M4_33]) +
        ((MT)pfSrcA[M4_34] * (MT)pfSrcB[M4_43])
    );

    // A: | 31 32 33 34 |   B: | ... 14 |
    //                         | ... 24 |
    //                         | ... 34 |
    //                         | ... 44 |
    pfDst[M4_34] = (T)(
        ((MT)pfSrcA[M4_31] * (MT)pfSrcB[M4_14]) +
        ((MT)pfSrcA[M4_32] * (MT)pfSrcB[M4_24]) +
        ((MT)pfSrcA[M4_33] * (MT)pfSrcB[M4_34]) +
        ((MT)pfSrcA[M4_34] * (MT)pfSrcB[M4_44])
    );

    // Row 4
    // A: | 41 42 43 44 |   B: | 11 ... |
    //                         | 21 ... |
    //                         | 31 ... |
    //                         | 41 ... |
    pfDst[M4_41] = (T)(
        ((MT)pfSrcA[M4_41] * (MT)pfSrcB[M4_11]) +
        ((MT)pfSrcA[M4_42] * (MT)pfSrcB[M4_21]) +
        ((MT)pfSrcA[M4_43] * (MT)pfSrcB[M4_31]) +
        ((MT)pfSrcA[M4_44] * (MT)pfSrcB[M4_41])
    );

    // A: | 41 42 43 44 |   B: | ... 12 ... |
    //                         | ... 22 ... |
    //                         | ... 32 ... |
    //                         | ... 42 ... |
    pfDst[M4_42] = (T)(
        ((MT)pfSrcA[M4_41] * (MT)pfSrcB[M4_12]) +
        ((MT)pfSrcA[M4_42] * (MT)pfSrcB[M4_22]) +
        ((MT)pfSrcA[M4_43] * (MT)pfSrcB[M4_32]) +
        ((MT)pfSrcA[M4_44] * (MT)pfSrcB[M4_42])
    );

    // A: | 41 42 43 44 |   B: | ... 13 ... |
    //                         | ... 23 ... |
    //                         | ... 33 ... |
    //                         | ... 43 ... |
    pfDst[M4_43] = (T)(
        ((MT)pfSrcA[M4_41] * (MT)pfSrcB[M4_13]) +
        ((MT)pfSrcA[M4_42] * (MT)pfSrcB[M4_23]) +
        ((MT)pfSrcA[M4_43] * (MT)pfSrcB[M4_33]) +
        ((MT)pfSrcA[M4_44] * (MT)pfSrcB[M4_43])
    );

    // A: | 41 42 43 44 |   B: | ... 14 |
    //                         | ... 24 |
    //                         | ... 34 |
    //                         | ... 44 |
    pfDst[M4_44] = (T)(
        ((MT)pfSrcA[M4_41] * (MT)pfSrcB[M4_14]) +
        ((MT)pfSrcA[M4_42] * (MT)pfSrcB[M4_24]) +
        ((MT)pfSrcA[M4_43] * (MT)pfSrcB[M4_34]) +
        ((MT)pfSrcA[M4_44] * (MT)pfSrcB[M4_44])
    );
}

template<typename T>
inline void mat4x4_transpose(T* pDst, T const* pSrc) {
    pDst[M4_11] = pSrc[M4_11];
    pDst[M4_12] = pSrc[M4_21];
    pDst[M4_13] = pSrc[M4_31];
    pDst[M4_14] = pSrc[M4_41];
    pDst[M4_21] = pSrc[M4_12];
    pDst[M4_22] = pSrc[M4_22];
    pDst[M4_23] = pSrc[M4_32];
    pDst[M4_24] = pSrc[M4_42];
    pDst[M4_31] = pSrc[M4_13];
    pDst[M4_32] = pSrc[M4_23];
    pDst[M4_33] = pSrc[M4_33];
    pDst[M4_34] = pSrc[M4_43];
    pDst[M4_41] = pSrc[M4_14];
    pDst[M4_42] = pSrc[M4_24];
    pDst[M4_43] = pSrc[M4_34];
    pDst[M4_44] = pSrc[M4_44];
}


/**
 * Mat4x4 Determinant - this gets painful
 */
template<typename T>
inline T mat4x4_determinant(T const* pfSrc) {

    MT pfTmp[4];

    pfTmp[0] = (
        ((MT)pfSrc[M4_22] * (MT)pfSrc[M4_33] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_22] * (MT)pfSrc[M4_34] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_32] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_32] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_43]) +
        ((MT)pfSrc[M4_42] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_34]) -
        ((MT)pfSrc[M4_42] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_33])
    );

    pfTmp[1] = (
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_34] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_33] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_34]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_33])
    );

    pfTmp[2] = (
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_32] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_34] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_42]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_34]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_32])
    );

    pfTmp[3] = (
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_33] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_32] * (MT)pfSrc[M4_43]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_33]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_32])
    );

    return (T)(
        (MT)pfSrc[M4_11] * pfTmp[0] +
        (MT)pfSrc[M4_12] * pfTmp[1] +
        (MT)pfSrc[M4_13] * pfTmp[2] +
        (MT)pfSrc[M4_14] * pfTmp[3]
    );
}

/**
 * Mat4x4 - Inverse. This is painful.
 */
template<typename T>
inline uint64 mat4x4_inverse(T* pfDst, T const* pfSrc) {

    MT pfTmp[16], fDeterminant;

    // Calculate the terms that are used for the determinant first.
    pfTmp[M4_11] = (
        ((MT)pfSrc[M4_22] * (MT)pfSrc[M4_33] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_22] * (MT)pfSrc[M4_34] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_32] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_32] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_43]) +
        ((MT)pfSrc[M4_42] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_34]) -
        ((MT)pfSrc[M4_42] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_33])
    );

    pfTmp[M4_21] = (
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_34] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_33] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_34]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_33])
    );

    pfTmp[M4_31] = (
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_32] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_34] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_42]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_34]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_32])
    );

    pfTmp[M4_41] = (
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_33] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_32] * (MT)pfSrc[M4_43]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_33]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_32])
    );

    fDeterminant =
        pfSrc[M4_11] * pfTmp[M4_11] +
        pfSrc[M4_12] * pfTmp[M4_21] +
        pfSrc[M4_13] * pfTmp[M4_31] +
        pfSrc[M4_14] * pfTmp[M4_41]
    ;

    if (fDeterminant == 0.0) {
        return ERR_ZERO_DIVIDE;
    }

    // Carry on now as it's definitely not pointless.
    pfTmp[M4_12] =
        ((MT)pfSrc[M4_12] * (MT)pfSrc[M4_34] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_12] * (MT)pfSrc[M4_33] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_32] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_32] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_42] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_34]) +
        ((MT)pfSrc[M4_42] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_33])
    ;

    pfTmp[M4_22] =
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_33] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_34] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_43]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_34]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_33])
    ;

    pfTmp[M4_32] =
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_34] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_32] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_34]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_32])
    ;

    pfTmp[M4_42] =
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_32] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_33] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_43]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_42]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_33]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_32])
    ;

    pfTmp[M4_13] =
        ((MT)pfSrc[M4_12] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_12] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_22] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_22] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_43]) +
        ((MT)pfSrc[M4_42] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_24]) -
        ((MT)pfSrc[M4_42] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_23])
    ;

    pfTmp[M4_23] =
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_24]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_23])
    ;

    pfTmp[M4_33] =
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_44]) -
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_44]) +
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_42]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_24]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_22])
    ;

    pfTmp[M4_43] =
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_43]) +
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_43]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_42]) -
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_23]) +
        ((MT)pfSrc[M4_41] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_22])
    ;

    pfTmp[M4_14] =
        ((MT)pfSrc[M4_12] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_33]) -
        ((MT)pfSrc[M4_12] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_34]) +
        ((MT)pfSrc[M4_22] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_34]) -
        ((MT)pfSrc[M4_22] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_33]) -
        ((MT)pfSrc[M4_32] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_24]) +
        ((MT)pfSrc[M4_32] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_23])
    ;

    pfTmp[M4_24] =
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_34]) -
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_33]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_34]) +
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_33]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_24]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_23])
    ;

    pfTmp[M4_34] =
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_24] * (MT)pfSrc[M4_32]) -
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_34]) +
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_34]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_32]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_24]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_14] * (MT)pfSrc[M4_22])
    ;

    pfTmp[M4_44] =
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_22] * (MT)pfSrc[M4_33]) -
        ((MT)pfSrc[M4_11] * (MT)pfSrc[M4_23] * (MT)pfSrc[M4_32]) -
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_33]) +
        ((MT)pfSrc[M4_21] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_32]) +
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_12] * (MT)pfSrc[M4_23]) -
        ((MT)pfSrc[M4_31] * (MT)pfSrc[M4_13] * (MT)pfSrc[M4_22])
    ;

    // Now scale by the inverse determinant to get the output
    fDeterminant = 1.0 / fDeterminant;
    for (int i = 0; i < 16; ++i) {
        pfDst[i] = (T)(pfTmp[i] * fDeterminant);
    }
    return ABI::ERR_NONE;
}

/**
 * m4x4(a0) = I
 */
template<typename T>
inline void m4x4_identity() {
    mat_identity<T, 4>(Interpreter::gpr<ABI::PTR_REG_0>().address<T>());
}

/**
 * m4x4(a1) = m4x4(a0)
 */
template<typename T>
inline void m4x4_copy() {
    mat_copy<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m4x4(a0) *= fp0
 */
template<typename T>
inline void m4x4_scale_assign() {
    mat_scale_assign<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

/**
 * m4x4(a1) = fp0 * m4x4(a0)
 */
template<typename T>
inline void m4x4_scale() {
    mat_scale<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

/**
 * m4x4(a1) += m4x4(a0)
 */
template<typename T>
inline void m4x4_add_assign() {
    mat_add_assign<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m4x4(a2) = m4x4(a1) + m4x4(a0)
 */
template<typename T>
inline void m4x4_add() {
    mat_add<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m4x4(a1) -= m4x4(a0)
 */
template<typename T>
inline void m4x4_sub_assign() {
    mat_sub_assign<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m4x4(a2) = m4x4(a1) - m4x4(a0)
 */
template<typename T>
inline void m4x4_sub() {
    mat_sub<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m4x4(a2) = m4x4(a1) x m4x4(a0)
 */
template<typename T>
inline void m4x4_multiply() {
    mat4x4_multiply<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m4x4(a1) x= m4x4(a0)
 */
template<typename T>
inline void m4x4_multiply_assign() {
    T* pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T pfTmp[16];
    mat_copy<T, 4>(pfTmp, pfDst);
    mat4x4_multiply<T>(
        pfDst,
        pfTmp,
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m4x4(a1) = m4x4^T(a0)
 */
template<typename T>
inline void m4x4_transpose() {
    mat4x4_transpose<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * fp0 = det m4x4(a0)
 */
template<typename T>
inline void m4x4_determinant() {
    Interpreter::fpr<ABI::FLT_REG_0>().value<T>() = mat4x4_determinant<T>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m4x4(a1) = m4x4^-1(a0)
 */
template<typename T>
inline void m4x4_inverse() {
    Interpreter::gpr<ABI::INT_REG_0>().uQuad = mat4x4_inverse<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * vec4[0 ... d0-1](a2) = mat4x4(a1) x vec4[0 ... d0-1](a0)
 */
template<typename T>
inline void v4_transform_4x4() {
    vec4_transform_4x4<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

} // namespace

#undef MT

#endif
