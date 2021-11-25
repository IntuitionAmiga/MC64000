#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_2X2_MACROS_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_2X2_MACROS_HPP__

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

#define m2x2_identity(T, NAME) mat_identity<T, 2>(aoGPR[ABI::PTR_REG_0].pf ## NAME)

#define m2x2_copy(T, NAME) mat_copy<T, 2>( \
    aoGPR[ABI::PTR_REG_1].p ## NAME, \
    aoGPR[ABI::PTR_REG_0].p ## NAME \
)

#define m2x2_scale_assign(T, NAME) mat_scale_assign<T, 2>( \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME \
)

#define m2x2_scale(T, NAME) mat_scale<T, 2>( \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME \
)

#define m2x2_add_assign(T, NAME) mat_add_assign<T, 2>( \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m2x2_add(T, NAME) mat_add<T, 2>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m2x2_sub_assign(T, NAME) mat_sub_assign<T, 2>( \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m2x2_sub(T, NAME) mat_sub<T, 2>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m2x2_multiply(T, NAME) mat2x2_multiply<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME \
)

#define m2x2_multiply_assign(T, NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T        pfTmp[2*2]; \
    mat_copy<T, 2>(pfTmp, pfDst); \
    mat2x2_multiply<T>( \
        pfDst, \
        pfTmp, \
        aoGPR[ABI::PTR_REG_0].pf ## NAME \
    ); \
}

}}}

#endif

