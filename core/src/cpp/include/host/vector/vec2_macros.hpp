#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_VEC2_MACROS_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_VEC2_MACROS_HPP__

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

/**
 * Dot product of Vec2
 */
#define dot2(pfV2A, pfV2B) (pfV2A[V_X] * pfV2B[V_X] + pfV2A[V_Y] * pfV2B[V_Y])

/**
 * Splat fill Vec2: (a0)... = fp0
 */
#define v2_splat(T, NAME) { \
    T* pfDst = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME; \
    pfDst[V_X] = f; \
    pfDst[V_Y] = f; \
}

/**
 * Copy Vec2: (a1)... = (a0)...
 */
#define v2_copy(T, NAME) { \
    T*       pDst = aoGPR[ABI::PTR_REG_1].p ## NAME; \
    T const* pSrc = aoGPR[ABI::PTR_REG_0].p ## NAME; \
    pDst[V_X] = pSrc[V_Y]; \
    pDst[V_X] = pSrc[V_Y]; \
}

/**
 * Scale Vec2: (a0)... *= fp0
 */
#define v2_scale_assign(T, NAME) { \
    T* pfDst = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME; \
    pfDst[V_X] *= f; \
    pfDst[V_Y] *= f; \
}

/**
 * Scale Vec2: (a1)... = fp0 * (a0)...
 */
#define v2_scale(T, NAME) { \
    T*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T        f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME; \
    pfDst[V_X] = f * pfSrc[V_X]; \
    pfDst[V_Y] = f * pfSrc[V_Y]; \
}

/**
 * Add Vec2: (a1)... += (a0)...
 */
#define v2_add_assign(T, NAME) { \
    T*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    pfDst[V_X] += pfSrc[V_Y]; \
    pfDst[V_X] += pfSrc[V_Y]; \
}

/**
 * Add Vec2: (a2)... = (a1)... + (a0)...
 */
#define v2_add(T, NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    pfDst[V_X] = pfSrc1[V_X] + pfSrc2[V_X]; \
    pfDst[V_Y] = pfSrc1[V_Y] + pfSrc2[V_Y]; \
}

/**
 * Subtract Vec2: (a1)... -= (a0)...
 */
#define v2_sub_assign(T, NAME) { \
    T*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    pfDst[V_X] -= pfSrc[V_X]; \
    pfDst[V_Y] -= pfSrc[V_Y]; \
}

/**
 * Subtract Vec2: (a2)... = (a1)... - (a0)...
 */
#define v2_sub(T, NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    pfDst[V_X] = pfSrc1[V_X] - pfSrc2[V_X]; \
    pfDst[V_Y] = pfSrc1[V_Y] - pfSrc2[V_Y]; \
}

/**
 * dot product Vec2: fp0 = (a1)... . (a0)...
 */
#define v2_dot(T, NAME) { \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME = dot2(pfSrc1, pfSrc2); \
}

/**
 * Magnitude Vec2: fp0 = |(a0)...|
 */
#define v2_magnitude(T, NAME) { \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME = std::sqrt(dot2(pfSrc, pfSrc)); \
}

/**
 * Normalise Vec2: (a0)... = normalise((a0)...)
 */
#define v2_normalise_assign(T, NAME, fac) { \
    T* pfDst   = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T  fInvMag = fac / std::sqrt(dot2(pfDst, pfDst)); \
    pfDst[V_X] *= fInvMag; \
    pfDst[V_Y] *= fInvMag; \
}

/**
 * Normalise Vec2: (a1)... = normalise((a0)...)
 */
#define v2_normalise(T, NAME, fac) { \
    T*       pfDst   = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc   = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T        fInvMag = fac / std::sqrt(dot2(pfSrc, pfSrc)); \
    pfDst[V_X] = fInvMag * pfSrc[V_X]; \
    pfDst[V_Y] = fInvMag * pfSrc[V_Y]; \
}

/**
 * Normalise Vec2: (a2)... = lerp((a1)... - (a0), fp0)
 */
#define v2_interpolate(T, NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T        fLerp  = Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME; \
    pfDst[V_X] = pfSrc2[V_X] + (fLerp * (pfSrc1[V_X] - pfSrc2[V_X])); \
    pfDst[V_Y] = pfSrc2[V_Y] + (fLerp * (pfSrc1[V_Y] - pfSrc2[V_Y])); \
}

/**
 * Transform Vec2: (a2)... = (a1)... * [(a0)...]
 */
#define v2_transform_2x2(T, NAME) \
vec2_transform_2x2<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

/**
 * Transform Vec2: (a2)... = (a1)... * [(a0)...]
 */
#define v2_0_transform_3x3(T, NAME) \
vec2_0_transform_3x3<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

/**
 * Transform Vec2: (a2)... = (a1)... * [(a0)...]
 */
#define v2_1_transform_3x3(T, NAME) \
vec2_1_transform_3x3<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

/**
 * Expand Vec2: (a1)... = (a0)..., fp0
 */
#define v2_expand_v3(T, NAME) \
vec2_expand_vec3<T>( \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

}}}

#endif

