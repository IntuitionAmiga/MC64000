#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_4X4_MACROS_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_4X4_MACROS_HPP__

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

#define m4x4_identity(T, UNION_NAME) mat_identity<T, 4>(aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME)

#define m4x4_copy(T, UNION_NAME) mat_copy<T, 4>( \
    aoGPR[ABI::PTR_REG_1].p ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].p ## UNION_NAME \
)

#define m4x4_scale_assign(T, UNION_NAME) mat_scale_assign<T, 4>( \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME \
)

#define m4x4_scale(T, UNION_NAME) mat_scale<T, 4>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME \
)

#define m4x4_add_assign(T, UNION_NAME) mat_add_assign<T, 4>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m4x4_add(T, UNION_NAME) mat_add<T, 4>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m4x4_sub_assign(T, UNION_NAME) mat_sub_assign<T, 4>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m4x4_sub(T, UNION_NAME) mat_sub<T, 4>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m4x4_multiply(T, UNION_NAME) mat4x4_multiply<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
)

#define m4x4_multiply_assign(T, UNION_NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T        pfTmp[4*4]; \
    mat_copy<T, 4>(pfTmp, pfDst); \
    mat4x4_multiply<T>( \
        pfDst, \
        pfTmp, \
        aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME \
    ); \
}

#define m4x4_transpose(T, UNION_NAME) { \
    T*       pDst = aoGPR[ABI::PTR_REG_1].p ## UNION_NAME; \
    T const* pSrc = aoGPR[ABI::PTR_REG_0].p ## UNION_NAME; \
    pDst[M4_11] = pSrc[M4_11]; \
    pDst[M4_12] = pSrc[M4_21]; \
    pDst[M4_13] = pSrc[M4_31]; \
    pDst[M4_14] = pSrc[M4_41]; \
    pDst[M4_21] = pSrc[M4_12]; \
    pDst[M4_22] = pSrc[M4_22]; \
    pDst[M4_23] = pSrc[M4_32]; \
    pDst[M4_24] = pSrc[M4_42]; \
    pDst[M4_31] = pSrc[M4_13]; \
    pDst[M4_32] = pSrc[M4_23]; \
    pDst[M4_33] = pSrc[M4_33]; \
    pDst[M4_34] = pSrc[M4_43]; \
    pDst[M4_41] = pSrc[M4_14]; \
    pDst[M4_42] = pSrc[M4_24]; \
    pDst[M4_43] = pSrc[M4_34]; \
    pDst[M4_44] = pSrc[M4_44]; \
}

#define swap_pair(a, b) { t = a; a = b; b = t; }

#define m4x4_transpose_assign(T, UNION_NAME) { \
    T* pDst = aoGPR[ABI::PTR_REG_0].p ## UNION_NAME; \
    T  t; \
    swap_pair(pDst[M4_12], pDst[M4_21]); \
    swap_pair(pDst[M4_13], pDst[M4_31]); \
    swap_pair(pDst[M4_14], pDst[M4_41]); \
    swap_pair(pDst[M4_23], pDst[M4_32]); \
    swap_pair(pDst[M4_24], pDst[M4_42]); \
    swap_pair(pDst[M4_34], pDst[M4_43]); \
}

#define v4_transform_4x4(T, UNION_NAME) \
vec4_transform_4x4<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

}}}

#endif

