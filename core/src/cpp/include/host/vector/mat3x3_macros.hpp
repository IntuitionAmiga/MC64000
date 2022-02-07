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
    T        pfTmp[3*3]; \
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
    pDst[M3_21] = pSrc[M3_21]; \
    pDst[M3_22] = pSrc[M3_22]; \
    pDst[M3_23] = pSrc[M3_32]; \
    pDst[M3_31] = pSrc[M3_13]; \
    pDst[M3_32] = pSrc[M3_23]; \
    pDst[M3_33] = pSrc[M3_33]; \
}

#define swap_pair(a, b) { t = a; a = b; b = t; }

#define m3x3_transpose_assign(T, UNION_NAME) { \
    T* pDst = aoGPR[ABI::PTR_REG_0].p ## UNION_NAME; \
    T  t; \
    swap_pair(pDst[M3_12], pDst[M3_21]); \
    swap_pair(pDst[M3_13], pDst[M3_31]); \
    swap_pair(pDst[M3_23], pDst[M3_32]); \
}

/**
 *
 *  | a b c |
 *  | d e f |  => a | e f | - b | d f | + c | d e | => a(ei - fg) - b(di - fg) + c(dh - eg)
 *  | g h i |       | h i |     | g i |     | g h |
 *
 *  => aei + bfg + cdh - ceg - bdi - afh
 */
#define m3x3_determinant(T, UNION_NAME) { \
    T const* pfMtx = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME = \
    /*   aei */ (pfMtx[M3_11] * pfMtx[M3_22] * pfMtx[M3_33]) + \
    /* + bfg */ (pfMtx[M3_12] * pfMtx[M3_23] * pfMtx[M3_31]) + \
    /* + cdh */ (pfMtx[M3_13] * pfMtx[M3_21] * pfMtx[M3_32]) - \
    /* - ceg */ (pfMtx[M3_13] * pfMtx[M3_22] * pfMtx[M3_31]) - \
    /* - bdi */ (pfMtx[M3_12] * pfMtx[M3_21] * pfMtx[M3_33]) - \
    /* - afh */ (pfMtx[M3_11] * pfMtx[M3_23] * pfMtx[M3_32]);  \
}

#define m3x3_inverse_assign(T, UNION_NAME) {\
\
}

}}}

#endif

