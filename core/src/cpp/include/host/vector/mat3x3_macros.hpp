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

#define m3x3_identity(T, NAME) mat_identity<T, 3>(aoGPR[ABI::PTR_REG_0].pf ## NAME)

#define m3x3_copy(T, NAME) mat_copy<T, 3>( \
    aoGPR[ABI::PTR_REG_1].p ## NAME, \
    aoGPR[ABI::PTR_REG_0].p ## NAME \
)

#define m3x3_scale_assign(T, NAME) mat_scale_assign<T, 3>( \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME \
)

#define m3x3_scale(T, NAME) mat_scale<T, 3>( \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME \
)

#define m3x3_add_assign(T, NAME) mat_add_assign<T, 3>( \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m3x3_add(T, NAME) mat_add<T, 3>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m3x3_sub_assign(T, NAME) mat_sub_assign<T, 3>( \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m3x3_sub(T, NAME) mat_sub<T, 3>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m3x3_multiply(T, NAME) mat3x3_multiply<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m3x3_multiply_assign(T, NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T        pfTmp[3*3]; \
    mat_copy<T, 3>(pfTmp, pfDst); \
    mat3x3_multiply<T>( \
        pfDst, \
        pfTmp, \
        aoGPR[ABI::PTR_REG_0].pf ## NAME \
    ); \
}

#define m3x3_transpose(T, NAME) { \
    T*       pDst = aoGPR[ABI::PTR_REG_1].p ## NAME; \
    T const* pSrc = aoGPR[ABI::PTR_REG_0].p ## NAME; \
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

#define m3x3_transpose_assign(T, NAME) { \
    T* pDst = aoGPR[ABI::PTR_REG_0].p ## NAME; \
    T  t; \
    swap_pair(pDst[M3_12], pDst[M3_21]); \
    swap_pair(pDst[M3_13], pDst[M3_31]); \
    swap_pair(pDst[M3_23], pDst[M3_32]); \
}

}}}

#endif

