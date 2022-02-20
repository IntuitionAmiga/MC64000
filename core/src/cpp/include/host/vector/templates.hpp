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

#ifdef MATRIX_FORCE_DOUBLE
#define MT T
#else
#define MT float64
#endif

/**
 * Applies a Mat2x2 to an input set of Vec2
 */
template<typename T>
inline void vec2_transform_2x2(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
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
inline void vec2_0_transform_3x3(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
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
inline void vec2_1_transform_3x3(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
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
inline void vec2_expand_vec3(T* pfDst, T const* pfSrc, T fValue, size_t uCount) {
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
inline void vec3_transform_3x3(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
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
inline void vec3_0_transform_4x4(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
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
inline void vec3_1_transform_4x4(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
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
inline void vec3_expand_vec4(T* pfDst, T const* pfSrc, T fValue, size_t uCount) {
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
inline void mat_add(T* pfDst, T const* pfSrcA, T const* pfSrcB) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] = pfSrcA[u] + pfSrcB[u];
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
inline void mat_sub(T* pfDst, T const* pfSrcA, T const* pfSrcB) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] = pfSrcA[u] - pfSrcB[u];
    }
}

template<typename T, unsigned const uDim>
inline void mat_print(T const* pfMtx) {
    std::putchar('\n');
    for (unsigned r = 0; r < uDim; ++r) {
        std::printf("| ");
        for (unsigned c = 0; c < uDim; ++c) {
            std::printf("%0.6f ", (double) *pfMtx++);
        }
        std::printf("|\n");
    }
    std::putchar('\n');
}

/**
 * Mat2x2 multiplication.
 *
 * C = A x B, A is by row, B is by column
 */
template<typename T>
inline void mat2x2_multiply(T* pfDst, T const* pfSrcA, T const* pfSrcB) {
    // Row 1
    pfDst[M2_11] = (T)(
        ((MT)pfSrcA[M2_11] * (MT)pfSrcB[M2_11]) +
        ((MT)pfSrcA[M2_12] * (MT)pfSrcB[M2_21])
    );
    pfDst[M2_12] = (T)(
        ((MT)pfSrcA[M2_11] * (MT)pfSrcB[M2_12]) +
        ((MT)pfSrcA[M2_12] * (MT)pfSrcB[M2_22])
    );

    // Row 2
    pfDst[M2_21] = (T)(
        ((MT)pfSrcA[M2_21] * (MT)pfSrcB[M2_11]) +
        ((MT)pfSrcA[M2_22] * (MT)pfSrcB[M2_21])
    );
    pfDst[M2_22] = (T)(
        ((MT)pfSrcA[M2_21] * (MT)pfSrcB[M2_12]) +
        ((MT)pfSrcA[M2_22] * (MT)pfSrcB[M2_22])
    );
}

template<typename T>
inline void mat2x2_transpose(T* pDst, T const* pSrc) {
    pDst[M2_11] = pSrc[M2_11];
    pDst[M2_12] = pSrc[M2_21];
    pDst[M2_21] = pSrc[M2_12];
    pDst[M2_22] = pSrc[M2_22];
}

template<typename T>
inline T mat2x2_determinant(T const* pfSrc) {
    return pfSrc[M2_11] * pfSrc[M2_22] - pfSrc[M2_12] * pfSrc[M2_21];
}

template<typename T>
inline uint64 mat2x2_inverse(T* pfDst, T const* pfSrc) {
    T fDeterminant = mat2x2_determinant<T>(pfSrc);
    if (fDeterminant) {
        fDeterminant = (T)1.0 / fDeterminant;
        pfDst[M2_11] = fDeterminant * pfSrc[M2_22];
        pfDst[M2_22] = fDeterminant * pfSrc[M2_11];
        pfDst[M2_12] = -fDeterminant * pfSrc[M2_12];
        pfDst[M2_21] = -fDeterminant * pfSrc[M2_21];
        return ABI::ERR_NONE;
    } else {
        return ERR_ZERO_DIVIDE;
    }
}
/**
 * Mat3x3 multiplication
 *
 * C = A x B, A is by row, B is by column
 */
template<typename T>
inline void mat3x3_multiply(T* pfDst, T const* pfSrcA, T const* pfSrcB) {

    // Row 1
    // A: | 11 12 13 |   B: | 11 ... |
    //                      | 21 ... |
    //                      | 31 ... |
    pfDst[M3_11] = (T)(
        ((MT)pfSrcA[M3_11] * (MT)pfSrcB[M3_11]) +
        ((MT)pfSrcA[M3_12] * (MT)pfSrcB[M3_21]) +
        ((MT)pfSrcA[M3_13] * (MT)pfSrcB[M3_31])
    );

    // A: | 11 12 13 |   B: | ... 12 ... |
    //                      | ... 22 ... |
    //                      | ... 32 ... |
    pfDst[M3_12] = (T)(
        ((MT)pfSrcA[M3_11] * (MT)pfSrcB[M3_12]) +
        ((MT)pfSrcA[M3_12] * (MT)pfSrcB[M3_22]) +
        ((MT)pfSrcA[M3_13] * (MT)pfSrcB[M3_32])
    );

    // A: | 11 12 13 |   B: | ... 13 |
    //                      | ... 23 |
    //                      | ... 33 |
    pfDst[M3_13] = (T)(
        ((MT)pfSrcA[M3_11] * (MT)pfSrcB[M3_13]) +
        ((MT)pfSrcA[M3_12] * (MT)pfSrcB[M3_23]) +
        ((MT)pfSrcA[M3_13] * (MT)pfSrcB[M3_33])
    );

    // Row 2
    // A: | 21 22 23 |   B: | 11 ... |
    //                      | 21 ... |
    //                      | 31 ... |
    pfDst[M3_21] = (T)(
        ((MT)pfSrcA[M3_21] * (MT)pfSrcB[M3_11]) +
        ((MT)pfSrcA[M3_22] * (MT)pfSrcB[M3_21]) +
        ((MT)pfSrcA[M3_23] * (MT)pfSrcB[M3_31])
    );

    // A: | 21 22 23 |   B: | ... 12 ... |
    //                      | ... 22 ... |
    //                      | ... 32 ... |
    pfDst[M3_22] = (T)(
        ((MT)pfSrcA[M3_21] * (MT)pfSrcB[M3_12]) +
        ((MT)pfSrcA[M3_22] * (MT)pfSrcB[M3_22]) +
        ((MT)pfSrcA[M3_23] * (MT)pfSrcB[M3_32])
    );

    // A: | 21 22 23 |   B: | ... 13 |
    //                      | ... 23 |
    //                      | ... 33 |
    pfDst[M3_23] = (T)(
        ((MT)pfSrcA[M3_21] * (MT)pfSrcB[M3_13]) +
        ((MT)pfSrcA[M3_22] * (MT)pfSrcB[M3_23]) +
        ((MT)pfSrcA[M3_23] * (MT)pfSrcB[M3_33])
    );

    // Row 3
    // A: | 31 32 33 |   B: | 11 ... |
    //                      | 21 ... |
    //                      | 31 ... |
    pfDst[M3_31] = (T)(
        ((MT)pfSrcA[M3_31] * (MT)pfSrcB[M3_11]) +
        ((MT)pfSrcA[M3_32] * (MT)pfSrcB[M3_21]) +
        ((MT)pfSrcA[M3_33] * (MT)pfSrcB[M3_31])
    );

    // A: | 31 32 33 |   B: | ... 12 ... |
    //                      | ... 22 ... |
    //                      | ... 32 ... |
    pfDst[M3_32] = (T)(
        ((MT)pfSrcA[M3_31] * (MT)pfSrcB[M3_12]) +
        ((MT)pfSrcA[M3_32] * (MT)pfSrcB[M3_22]) +
        ((MT)pfSrcA[M3_33] * (MT)pfSrcB[M3_32])
    );

    // A: | 31 32 33 |   B: | ... 13 |
    //                      | ... 23 |
    //                      | ... 33 |
    pfDst[M3_33] = (T)(
        ((MT)pfSrcA[M3_31] * (MT)pfSrcB[M3_13]) +
        ((MT)pfSrcA[M3_32] * (MT)pfSrcB[M3_23]) +
        ((MT)pfSrcA[M3_33] * (MT)pfSrcB[M3_33])
    );
}

template<typename T>
inline void mat3x3_transpose(T* pDst, T const* pSrc) {
    pDst[M3_11] = pSrc[M3_11];
    pDst[M3_12] = pSrc[M3_21];
    pDst[M3_13] = pSrc[M3_31];
    pDst[M3_21] = pSrc[M3_12];
    pDst[M3_22] = pSrc[M3_22];
    pDst[M3_23] = pSrc[M3_32];
    pDst[M3_31] = pSrc[M3_13];
    pDst[M3_32] = pSrc[M3_23];
    pDst[M3_33] = pSrc[M3_33];
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
 *   | a b c | -1         1    | A B C |T            1    | A D G |
 *   | d e f |     =>   ------ | D E F |      =>   ------ | B E H |
 *   | g h i |          det(A) | G H I |           det(A) | C F I |
 *
 *  A =  (ei - fh),   D = -(bi - ch),   G =  (bf - ce)
 *  B = -(di - fg),   E =  (ai - cg),   H = -(af - cd)
 *  C =  (dh - eg),   F = -(ah - bg),   I =  (ae - bd)
 */
template<typename T>
inline uint64 mat3x3_inverse(T* pfDst, T const* pfSrc) {
    T fDeterminant = mat3x3_determinant<T>(pfSrc);
    if (fDeterminant) {
        /* A = (ei - fh) */
        pfDst[M3_11] = (pfSrc[M3_22] * pfSrc[M3_33]) - (pfSrc[M3_23] * pfSrc[M3_32]);
        /* D = (ch - bi) */
        pfDst[M3_12] = (pfSrc[M3_13] * pfSrc[M3_32]) - (pfSrc[M3_12] * pfSrc[M3_33]);
        /* G = (bf - ce) */
        pfDst[M3_13] = (pfSrc[M3_12] * pfSrc[M3_23]) - (pfSrc[M3_13] * pfSrc[M3_22]);
        /* B = (fg - di) */
        pfDst[M3_21] = (pfSrc[M3_23] * pfSrc[M3_31]) - (pfSrc[M3_21] * pfSrc[M3_33]);
        /* E = (ai - cg) */
        pfDst[M3_22] = (pfSrc[M3_11] * pfSrc[M3_33]) - (pfSrc[M3_13] * pfSrc[M3_31]);
        /* H = (cd - af) */
        pfDst[M3_23] = (pfSrc[M3_13] * pfSrc[M3_21]) - (pfSrc[M3_11] * pfSrc[M3_23]);
        /* C = (dh - eg) */
        pfDst[M3_31] = (pfSrc[M3_21] * pfSrc[M3_32]) - (pfSrc[M3_22] * pfSrc[M3_31]);
        /* F = (bg - ah) */
        pfDst[M3_32] = (pfSrc[M3_12] * pfSrc[M3_31]) - (pfSrc[M3_11] * pfSrc[M3_32]);
        /* I = (ae - bd) */
        pfDst[M3_33] = (pfSrc[M3_11] * pfSrc[M3_22]) - (pfSrc[M3_12] * pfSrc[M3_21]);
        fDeterminant = (T)1.0 / fDeterminant;                                           \
        pfDst[M3_11] *= fDeterminant;
        pfDst[M3_12] *= fDeterminant;
        pfDst[M3_13] *= fDeterminant;
        pfDst[M3_21] *= fDeterminant;
        pfDst[M3_22] *= fDeterminant;
        pfDst[M3_23] *= fDeterminant;
        pfDst[M3_31] *= fDeterminant;
        pfDst[M3_32] *= fDeterminant;
        pfDst[M3_33] *= fDeterminant;
        return ABI::ERR_NONE;
    } else {
        return ERR_ZERO_DIVIDE;
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

}}}
#endif
