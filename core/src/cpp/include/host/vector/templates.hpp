#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_TEMPLATES_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_TEMPLATES_HPP__

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

#include <cstddef>
#include <cstring>
#include <cstdlib>
#include "./offsets.hpp"

namespace MC64K {
namespace StandardTestHost {
namespace VectorMath {

/**
 * Applies a Mat2x2 to an input set of Vec2
 */
template<typename T>
void vec2_transform_2x2(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M2_11] * pfSrc[V_X] + pfM[M2_12] * pfSrc[V_Y];
        *pfDst++ = pfM[M2_21] * pfSrc[V_X] + pfM[M2_22] * pfSrc[V_Y];
        pfSrc += 2;
    }
}

/**
 * Applies a Mat3x3 to a input set of Vec2, substituting zero for the missing 3rd element of each one.
 * Output is also a set of Vec2. Only the first 2 rows of the matrix are considered.
 */
template<typename T>
void vec2_0_transform_3x3(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M3_11] * pfSrc[V_X] + pfM[M3_12] * pfSrc[V_Y];
        *pfDst++ = pfM[M3_21] * pfSrc[V_X] + pfM[M3_22] * pfSrc[V_Y];
        pfSrc += 2;
    }
}

/**
 * Applies a Mat3x3 to a input set of Vec2, substituting one for the missing 3rd element of each one.
 * Output is also a set of Vec2. Only the first 2 rows of the matrix are considered.
 */
template<typename T>
void vec2_1_transform_3x3(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M3_11] * pfSrc[V_X] + pfM[M3_12] * pfSrc[V_Y] + pfM[M3_13];
        *pfDst++ = pfM[M3_21] * pfSrc[V_X] + pfM[M3_22] * pfSrc[V_Y] + pfM[M3_23];
        pfSrc += 2;
    }
}

/**
 * Vec2 to Vec3 expand
 */
template<typename T>
void vec2_expand_vec3(T* pfDst, T const* pfSrc, T fValue, size_t uCount) {
    while (uCount--) {
        *pfDst++ = *pfSrc++;
        *pfDst++ = *pfSrc++;
        *pfDst++ = fValue;
    }
}

/**
 * Applies a Mat3x3 to an input set of Vec3
 */
template<typename T>
void vec3_transform_3x3(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M3_11] * pfSrc[V_X] + pfM[M3_12] * pfSrc[V_Y] + pfM[M3_13] * pfSrc[V_Z];
        *pfDst++ = pfM[M3_21] * pfSrc[V_X] + pfM[M3_22] * pfSrc[V_Y] + pfM[M3_23] * pfSrc[V_Z];
        *pfDst++ = pfM[M3_31] * pfSrc[V_X] + pfM[M3_32] * pfSrc[V_Y] + pfM[M3_33] * pfSrc[V_Z];
        pfSrc += 3;
    }
}

/**
 * Applies a Mat4x4 to a input set of Vec3, substituting zero for the missing 4rd element of each one.
 * Output is also a set of Vec3. Only the first 3 rows of the matrix are considered.
 */
template<typename T>
void vec3_0_transform_4x4(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M4_11] * pfSrc[V_X] + pfM[M4_12] * pfSrc[V_Y] + pfM[M4_13] * pfSrc[V_Z];
        *pfDst++ = pfM[M4_21] * pfSrc[V_X] + pfM[M4_22] * pfSrc[V_Y] + pfM[M4_23] * pfSrc[V_Z];
        *pfDst++ = pfM[M4_31] * pfSrc[V_X] + pfM[M4_32] * pfSrc[V_Y] + pfM[M4_33] * pfSrc[V_Z];
        pfSrc += 3;
    }
}

/**
 * Applies a Mat4x4 to a input set of Vec3, substituting one for the missing 4rd element of each one.
 * Output is also a set of Vec3. Only the first thee rows of the matrix are considered.
 */
template<typename T>
void vec3_1_transform_4x4(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M4_11] * pfSrc[V_X] + pfM[M4_12] * pfSrc[V_Y] + pfM[M4_13] * pfSrc[V_Z] + pfM[M4_14];
        *pfDst++ = pfM[M4_21] * pfSrc[V_X] + pfM[M4_22] * pfSrc[V_Y] + pfM[M4_23] * pfSrc[V_Z] + pfM[M4_24];
        *pfDst++ = pfM[M4_31] * pfSrc[V_X] + pfM[M4_32] * pfSrc[V_Y] + pfM[M4_44] * pfSrc[V_Z] + pfM[M4_34];
        pfSrc += 3;
    }
}

/**
 * Vec3 to Vec4 expand
 */
template<typename T>
void vec3_expand_vec4(T* pfDst, T const* pfSrc, T fValue, size_t uCount) {
    while (uCount--) {
        *pfDst++ = *pfSrc++;
        *pfDst++ = *pfSrc++;
        *pfDst++ = *pfSrc++;
        *pfDst++ = fValue;
    }
}

/**
 * Apply a Mat4x4 to an input set of Vec4.
 */
template<typename T>
void vec4_transform_4x4(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M4_11] * pfSrc[V_X] + pfM[M4_12] * pfSrc[V_Y] + pfM[M4_13] * pfSrc[V_Z] + pfM[M4_14] * pfSrc[V_W];
        *pfDst++ = pfM[M4_21] * pfSrc[V_X] + pfM[M4_22] * pfSrc[V_Y] + pfM[M4_23] * pfSrc[V_Z] + pfM[M4_24] * pfSrc[V_W];
        *pfDst++ = pfM[M4_31] * pfSrc[V_X] + pfM[M4_32] * pfSrc[V_Y] + pfM[M4_33] * pfSrc[V_Z] + pfM[M4_34] * pfSrc[V_W];
        *pfDst++ = pfM[M4_41] * pfSrc[V_X] + pfM[M4_42] * pfSrc[V_Y] + pfM[M4_43] * pfSrc[V_Z] + pfM[M4_44] * pfSrc[V_W];
        pfSrc += 4;
    }
}

/**
 * Generic copy routine
 */
template<typename T, unsigned const uDim>
inline void mat_copy(T* pfDst, T const* pfSrc) {
    std::memcpy(pfDst, pfSrc, uDim * uDim * sizeof(T));
}

/**
 * Generic identity routine
 */
template<typename T, unsigned const uDim>
inline void mat_identity(T* pfDst) {
    std::memset(pfDst, 0, uDim * uDim * sizeof(T));
    const T fOne = 1.0;
    for (unsigned u = 0, uIndex = 0; u < uDim; ++u, uIndex += uDim + 1) {
        pfDst[uIndex] = fOne;
    }
}

/**
 * Generic scale assign
 */
template<typename T, unsigned const uDim>
inline void mat_scale_assign(T* pfDst, T const f) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] *= f;
    }
}

/**
 * Generic scale
 */
template<typename T, unsigned const uDim>
inline void mat_scale(T* pfDst, T const* pfSrc, T const f) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] = pfSrc[u] * f;
    }
}

/**
 * Generic add assign
 */
template<typename T, unsigned const uDim>
inline void mat_add_assign(T* pfDst, T const* pfSrc) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] += pfSrc[u];
    }
}

/**
 * Generic add
 */
template<typename T, unsigned const uDim>
inline void mat_add(T* pfDst, T const* pfSrc1, T const* pfSrc2) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] = pfSrc1[u] + pfSrc2[u];
    }
}

/**
 * Generic sub assign
 */
template<typename T, unsigned const uDim>
inline void mat_sub_assign(T* pfDst, T const* pfSrc) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] -= pfSrc[u];
    }
}

/**
 * Generic sub
 */
template<typename T, unsigned const uDim>
inline void mat_sub(T* pfDst, T const* pfSrc1, T const* pfSrc2) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] = pfSrc1[u] - pfSrc2[u];
    }
}

/**
 * Mat2x2 multiplication.
 *
 * C = A x B, A is by row, B is by column
 */
template<typename T>
void mat2x2_multiply(T* pfC, T const* pfA, T const* pfB) {
    // Row 1
    pfC[M2_11] = pfA[M2_11]*pfB[M2_11] + pfA[M2_12]*pfB[M2_21];
    pfC[M2_12] = pfA[M2_11]*pfB[M2_12] + pfA[M2_12]*pfB[M2_22];

    // Row 2
    pfC[M2_21] = pfA[M2_21]*pfB[M2_11] + pfA[M2_22]*pfB[M2_21];
    pfC[M2_22] = pfA[M2_21]*pfB[M2_12] + pfA[M2_22]*pfB[M2_22];
}

/**
 * Mat3x3 multiplication
 *
 * C = A x B, A is by row, B is by column
 */
template<typename T>
inline void mat3x3_multiply(T* pfC, T const* pfA, T const* pfB) {

    // Row 1 //////////////////////////////////////////////////////////////////////////
    // A: | 11 12 13 |   B: | 11 ... |
    //                      | 21 ... |
    //                      | 31 ... |
    pfC[M3_11] = pfA[M3_11]*pfB[M3_11] + pfA[M3_12]*pfB[M3_21] + pfA[M3_13]*pfB[M3_31];

    // A: | 11 12 13 |   B: | ... 12 ... |
    //                      | ... 22 ... |
    //                      | ... 32 ... |
    pfC[M3_12] = pfA[M3_11]*pfB[M3_12] + pfA[M3_12]*pfB[M3_22] + pfA[M3_13]*pfB[M3_32];

    // A: | 11 12 13 |   B: | ... 13 |
    //                      | ... 23 |
    //                      | ... 33 |
    pfC[M3_13] = pfA[M3_11]*pfB[M3_13] + pfA[M3_12]*pfB[M3_23] + pfA[M3_13]*pfB[M3_33];

    // Row 2 //////////////////////////////////////////////////////////////////////////
    // A: | 21 22 23 |   B: | 11 ... |
    //                      | 21 ... |
    //                      | 31 ... |
    pfC[M3_21] = pfA[M3_21]*pfB[M3_11] + pfA[M3_22]*pfB[M3_21] + pfA[M3_23]*pfB[M3_31];

    // A: | 21 22 23 |   B: | ... 12 ... |
    //                      | ... 22 ... |
    //                      | ... 32 ... |
    pfC[M3_22] = pfA[M3_21]*pfB[M3_12] + pfA[M3_22]*pfB[M3_22] + pfA[M3_23]*pfB[M3_32];

    // A: | 21 22 23 |   B: | ... 13 |
    //                      | ... 23 |
    //                      | ... 33 |
    pfC[M3_23] = pfA[M3_21]*pfB[M3_13] + pfA[M3_22]*pfB[M3_23] + pfA[M3_23]*pfB[M3_33];

    // Row 3 //////////////////////////////////////////////////////////////////////////
    // A: | 31 32 33 |   B: | 11 ... |
    //                      | 21 ... |
    //                      | 31 ... |
    pfC[M3_31] = pfA[M3_31]*pfB[M3_11] + pfA[M3_32]*pfB[M3_21] + pfA[M3_33]*pfB[M3_31];

    // A: | 31 32 33 |   B: | ... 12 ... |
    //                      | ... 22 ... |
    //                      | ... 32 ... |
    pfC[M3_32] = pfA[M3_31]*pfB[M3_12] + pfA[M3_32]*pfB[M3_22] + pfA[M3_33]*pfB[M3_32];

    // A: | 31 32 33 |   B: | ... 13 |
    //                      | ... 23 |
    //                      | ... 33 |
    pfC[M3_33] = pfA[M3_31]*pfB[M3_13] + pfA[M3_32]*pfB[M3_23] + pfA[M3_33]*pfB[M3_33];
}

/**
 * Mat3x3 determinant
 *
 *  | a b c |
 *  | d e f |  => a | e f | - b | d f | + c | d e | => a(ei - fg) - b(di - fg) + c(dh - eg)
 *  | g h i |       | h i |     | g i |     | g h |
 *
 *  => aei + bfg + cdh - ceg - bdi - afh
 */
template<typename T>
inline T mat3x3_determinant(T const* pfMtx) {
    return
    /*   aei */ (pfMtx[M3_11] * pfMtx[M3_22] * pfMtx[M3_33]) +
    /* + bfg */ (pfMtx[M3_12] * pfMtx[M3_23] * pfMtx[M3_31]) +
    /* + cdh */ (pfMtx[M3_13] * pfMtx[M3_21] * pfMtx[M3_32]) -
    /* - ceg */ (pfMtx[M3_13] * pfMtx[M3_22] * pfMtx[M3_31]) -
    /* - bdi */ (pfMtx[M3_12] * pfMtx[M3_21] * pfMtx[M3_33]) -
    /* - afh */ (pfMtx[M3_11] * pfMtx[M3_23] * pfMtx[M3_32]);
}

/**
 * Mat4x4 multiplication
 *
 * C = A x B, A is by row, B is by column
 */
template<typename T>
inline void mat4x4_multiply(T* pfC, T const* pfA, T const* pfB) {
    // Row 1 //////////////////////////////////////////////////////////////////////////
    // A: | 11 12 13 14 |   B: | 11 ... |
    //                         | 21 ... |
    //                         | 31 ... |
    //                         | 41 ... |
    pfC[M4_11] = pfA[M4_11]*pfB[M4_11] + pfA[M4_12]*pfB[M4_21] + pfA[M4_13]*pfB[M4_31] + pfA[M4_14]*pfB[M4_41];

    // A: | 11 12 13 14 |   B: | ... 12 ... |
    //                         | ... 22 ... |
    //                         | ... 32 ... |
    //                         | ... 42 ... |
    pfC[M4_12] = pfA[M4_11]*pfB[M4_12] + pfA[M4_12]*pfB[M4_22] + pfA[M4_13]*pfB[M4_32] + pfA[M4_14]*pfB[M4_42];

    // A: | 11 12 13 14 |   B: | ... 13 ... |
    //                         | ... 23 ... |
    //                         | ... 33 ... |
    //                         | ... 43 ... |
    pfC[M4_13] = pfA[M4_11]*pfB[M4_13] + pfA[M4_12]*pfB[M4_23] + pfA[M4_13]*pfB[M4_33] + pfA[M4_14]*pfB[M4_43];

    // A: | 11 12 13 14 |   B: | ... 14 |
    //                         | ... 24 |
    //                         | ... 34 |
    //                         | ... 44 |
    pfC[M4_14] = pfA[M4_11]*pfB[M4_14] + pfA[M4_12]*pfB[M4_24] + pfA[M4_13]*pfB[M4_34] + pfA[M4_14]*pfB[M4_44];

    // Row 2 //////////////////////////////////////////////////////////////////////////
    // A: | 21 22 23 24 |   B: | 11 ... |
    //                         | 21 ... |
    //                         | 31 ... |
    //                         | 41 ... |
    pfC[M4_21] = pfA[M4_21]*pfB[M4_11] + pfA[M4_22]*pfB[M4_21] + pfA[M4_23]*pfB[M4_31] + pfA[M4_24]*pfB[M4_41];

    // A: | 21 22 23 24 |   B: | ... 12 ... |
    //                         | ... 22 ... |
    //                         | ... 32 ... |
    //                         | ... 42 ... |
    pfC[M4_22] = pfA[M4_21]*pfB[M4_12] + pfA[M4_22]*pfB[M4_22] + pfA[M4_23]*pfB[M4_32] + pfA[M4_24]*pfB[M4_42];

    // A: | 21 22 23 24 |   B: | ... 13 ... |
    //                         | ... 23 ... |
    //                         | ... 33 ... |
    //                         | ... 43 ... |
    pfC[M4_23] = pfA[M4_21]*pfB[M4_13] + pfA[M4_22]*pfB[M4_23] + pfA[M4_23]*pfB[M4_33] + pfA[M4_24]*pfB[M4_43];

    // A: | 21 22 23 24 |   B: | ... 14 |
    //                         | ... 24 |
    //                         | ... 34 |
    //                         | ... 44 |
    pfC[M4_24] = pfA[M4_21]*pfB[M4_14] + pfA[M4_22]*pfB[M4_24] + pfA[M4_23]*pfB[M4_34] + pfA[M4_24]*pfB[M4_44];

    // Row 3 //////////////////////////////////////////////////////////////////////////
    // A: | 31 32 33 34 |   B: | 11 ... |
    //                         | 21 ... |
    //                         | 31 ... |
    //                         | 41 ... |
    pfC[M4_31] = pfA[M4_31]*pfB[M4_11] + pfA[M4_32]*pfB[M4_21] + pfA[M4_33]*pfB[M4_31] + pfA[M4_34]*pfB[M4_41];

    // A: | 31 32 33 34 |   B: | ... 12 ... |
    //                         | ... 22 ... |
    //                         | ... 32 ... |
    //                         | ... 42 ... |
    pfC[M4_32] = pfA[M4_31]*pfB[M4_12] + pfA[M4_32]*pfB[M4_22] + pfA[M4_33]*pfB[M4_32] + pfA[M4_34]*pfB[M4_42];

    // A: | 31 32 33 34 |   B: | ... 13 ... |
    //                         | ... 23 ... |
    //                         | ... 33 ... |
    //                         | ... 43 ... |
    pfC[M4_33] = pfA[M4_31]*pfB[M4_13] + pfA[M4_32]*pfB[M4_23] + pfA[M4_33]*pfB[M4_33] + pfA[M4_34]*pfB[M4_43];

    // A: | 31 32 33 34 |   B: | ... 14 |
    //                         | ... 24 |
    //                         | ... 34 |
    //                         | ... 44 |
    pfC[M4_34] = pfA[M4_31]*pfB[M4_14] + pfA[M4_32]*pfB[M4_24] + pfA[M4_23]*pfB[M4_34] + pfA[M4_34]*pfB[M4_44];

    // Row 4 //////////////////////////////////////////////////////////////////////////
    // A: | 41 42 43 44 |   B: | 11 ... |
    //                         | 21 ... |
    //                         | 31 ... |
    //                         | 41 ... |
    pfC[M4_41] = pfA[M4_41]*pfB[M4_11] + pfA[M4_42]*pfB[M4_21] + pfA[M4_43]*pfB[M4_31] + pfA[M4_44]*pfB[M4_41];

    // A: | 41 42 43 44 |   B: | ... 12 ... |
    //                         | ... 22 ... |
    //                         | ... 32 ... |
    //                         | ... 42 ... |
    pfC[M4_42] = pfA[M4_41]*pfB[M4_12] + pfA[M4_42]*pfB[M4_22] + pfA[M4_43]*pfB[M4_32] + pfA[M4_44]*pfB[M4_42];

    // A: | 41 42 43 44 |   B: | ... 13 ... |
    //                         | ... 23 ... |
    //                         | ... 33 ... |
    //                         | ... 43 ... |
    pfC[M4_43] = pfA[M4_41]*pfB[M4_13] + pfA[M4_42]*pfB[M4_23] + pfA[M4_43]*pfB[M4_33] + pfA[M4_44]*pfB[M4_43];

    // A: | 41 42 43 44 |   B: | ... 14 |
    //                         | ... 24 |
    //                         | ... 34 |
    //                         | ... 44 |
    pfC[M4_44] = pfA[M4_41]*pfB[M4_14] + pfA[M4_42]*pfB[M4_24] + pfA[M4_43]*pfB[M4_34] + pfA[M4_44]*pfB[M4_44];

}

}}}
#endif
