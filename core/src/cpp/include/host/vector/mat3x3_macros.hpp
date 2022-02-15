#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_3X3_MACROS_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_3X3_MACROS_HPP__

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

#include "./offsets.hpp"
#include "./templates.hpp"

namespace MC64K {
namespace StandardTestHost {
namespace VectorMath {

#define m3x3_identity(T, UNION_NAME) mat_identity<T, 3>(aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME)

#define m3x3_copy(T, UNION_NAME) mat_copy<T, 3>( \
    aoGPR[ABI::PTR_REG_1].p ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].p ## UNION_NAME \
)

#define m3x3_scale_assign(T, UNION_NAME) mat_scale_assign<T, 3>( \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME \
)

#define m3x3_scale(T, UNION_NAME) mat_scale<T, 3>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME \
)

#define m3x3_add_assign(T, UNION_NAME) mat_add_assign<T, 3>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m3x3_add(T, UNION_NAME) mat_add<T, 3>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m3x3_sub_assign(T, UNION_NAME) mat_sub_assign<T, 3>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m3x3_sub(T, UNION_NAME) mat_sub<T, 3>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m3x3_multiply(T, UNION_NAME) mat3x3_multiply<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m3x3_multiply_assign(T, UNION_NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T        pfTmp[3 * 3]; \
    mat_copy<T, 3>(pfTmp, pfDst); \
    mat3x3_multiply<T>( \
        pfDst, \
        pfTmp, \
        aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
    ); \
}

#define m3x3_transpose(T, UNION_NAME) { \
    T*       pDst = aoGPR[ABI::PTR_REG_1].p ## UNION_NAME; \
    T const* pSrc = aoGPR[ABI::PTR_REG_0].p ## UNION_NAME; \
    pDst[M3_11] = pSrc[M3_11]; \
    pDst[M3_12] = pSrc[M3_21]; \
    pDst[M3_13] = pSrc[M3_31]; \
    pDst[M3_21] = pSrc[M3_12]; \
    pDst[M3_22] = pSrc[M3_22]; \
    pDst[M3_23] = pSrc[M3_32]; \
    pDst[M3_31] = pSrc[M3_13]; \
    pDst[M3_32] = pSrc[M3_23]; \
    pDst[M3_33] = pSrc[M3_33]; \
}

#define swap_pair(a, b) { t = a; a = b; b = t; }

#define m3x3_determinant(T, UNION_NAME) {                                              \
    T const* pfMtx = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME;                           \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME = mat3x3_determinant<T>(pfMtx); \
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

#define m3x3_inverse(T, UNION_NAME) {                                                  \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME;                           \
    T* pfDst = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME;                                 \
    T fDeterminant = mat3x3_determinant<T>(pfSrc);                                     \
    if (fDeterminant) {                                                                \
        /* A = (ei - fh) */                                                            \
        pfDst[M3_11] = (pfSrc[M3_22] * pfSrc[M3_33]) - (pfSrc[M3_23] * pfSrc[M3_32]);  \
        /* D = (ch - bi) */                                                            \
        pfDst[M3_12] = (pfSrc[M3_13] * pfSrc[M3_32]) - (pfSrc[M3_12] * pfSrc[M3_33]);  \
        /* G = (bf - ce) */                                                            \
        pfDst[M3_13] = (pfSrc[M3_12] * pfSrc[M3_23]) - (pfSrc[M3_13] * pfSrc[M3_22]);  \
        /* B = (fg - di) */                                                            \
        pfDst[M3_21] = (pfSrc[M3_23] * pfSrc[M3_31]) - (pfSrc[M3_21] * pfSrc[M3_33]);  \
        /* E = (ai - cg) */                                                            \
        pfDst[M3_22] = (pfSrc[M3_11] * pfSrc[M3_33]) - (pfSrc[M3_13] * pfSrc[M3_31]);  \
        /* H = (cd - af) */                                                            \
        pfDst[M3_23] = (pfSrc[M3_13] * pfSrc[M3_21]) - (pfSrc[M3_11] * pfSrc[M3_23]);  \
        /* C = (dh - eg) */                                                            \
        pfDst[M3_31] = (pfSrc[M3_21] * pfSrc[M3_32]) - (pfSrc[M3_22] * pfSrc[M3_31]);  \
        /* F = (bg - ah) */                                                            \
        pfDst[M3_32] = (pfSrc[M3_12] * pfSrc[M3_31]) - (pfSrc[M3_11] * pfSrc[M3_32]);  \
        /* I = (ae - bd) */                                                            \
        pfDst[M3_33] = (pfSrc[M3_11] * pfSrc[M3_22]) - (pfSrc[M3_12] * pfSrc[M3_21]);  \
        fDeterminant = (T)1.0 / fDeterminant;                                             \
        pfDst[M3_11] *= fDeterminant;                                                  \
        pfDst[M3_12] *= fDeterminant;                                                  \
        pfDst[M3_13] *= fDeterminant;                                                  \
        pfDst[M3_21] *= fDeterminant;                                                  \
        pfDst[M3_22] *= fDeterminant;                                                  \
        pfDst[M3_23] *= fDeterminant;                                                  \
        pfDst[M3_31] *= fDeterminant;                                                  \
        pfDst[M3_32] *= fDeterminant;                                                  \
        pfDst[M3_33] *= fDeterminant;                                                  \
        aoGPR[ABI::INT_REG_0].uQuad = ABI::ERR_NONE;                                   \
    } else {                                                                           \
        aoGPR[ABI::INT_REG_0].uQuad = ERR_ZERO_DIVIDE;                                 \
    }                                                                                  \
}


}}}

#endif

