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
#define v2_splat(T, UNION_NAME) { \
    T* pfDst = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    T  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME; \
    pfDst[V_X] = f; \
    pfDst[V_Y] = f; \
}

/**
 * Copy Vec2: (a1)... = (a0)...
 */
#define v2_copy(T, UNION_NAME) { \
    T*       pDst = aoGPR[ABI::PTR_REG_1].p ## UNION_NAME; \
    T const* pSrc = aoGPR[ABI::PTR_REG_0].p ## UNION_NAME; \
    pDst[V_X] = pSrc[V_X]; \
    pDst[V_Y] = pSrc[V_Y]; \
}

/**
 * Scale Vec2: (a0)... *= fp0
 */
#define v2_scale_assign(T, UNION_NAME) { \
    T* pfDst = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    T  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME; \
    pfDst[V_X] *= f; \
    pfDst[V_Y] *= f; \
}

/**
 * Scale Vec2: (a1)... = fp0 * (a0)...
 */
#define v2_scale(T, UNION_NAME) { \
    T*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    T        f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME; \
    pfDst[V_X] = f * pfSrc[V_X]; \
    pfDst[V_Y] = f * pfSrc[V_Y]; \
}

/**
 * Add Vec2: (a1)... += (a0)...
 */
#define v2_add_assign(T, UNION_NAME) { \
    T*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    pfDst[V_X] += pfSrc[V_X]; \
    pfDst[V_Y] += pfSrc[V_Y]; \
}

/**
 * Add Vec2: (a2)... = (a1)... + (a0)...
 */
#define v2_add(T, UNION_NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    pfDst[V_X] = pfSrc1[V_X] + pfSrc2[V_X]; \
    pfDst[V_Y] = pfSrc1[V_Y] + pfSrc2[V_Y]; \
}

/**
 * Subtract Vec2: (a1)... -= (a0)...
 */
#define v2_sub_assign(T, UNION_NAME) { \
    T*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    pfDst[V_X] -= pfSrc[V_X]; \
    pfDst[V_Y] -= pfSrc[V_Y]; \
}

/**
 * Subtract Vec2: (a2)... = (a1)... - (a0)...
 */
#define v2_sub(T, UNION_NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    pfDst[V_X] = pfSrc1[V_X] - pfSrc2[V_X]; \
    pfDst[V_Y] = pfSrc1[V_Y] - pfSrc2[V_Y]; \
}

/**
 * dot product Vec2: fp0 = (a1)... . (a0)...
 */
#define v2_dot(T, UNION_NAME) { \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME = dot2(pfSrc1, pfSrc2); \
}

/**
 * Magnitude Vec2: fp0 = |(a0)...|
 */
#define v2_magnitude(T, UNION_NAME) { \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME = std::sqrt(dot2(pfSrc, pfSrc)); \
}

/**
 * Normalise Vec2: (a0)... = normalise((a0)...)
 */
#define v2_normalise_assign(T, UNION_NAME, fac) { \
    T* pfDst   = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    T  fInvMag = fac / std::sqrt(dot2(pfDst, pfDst)); \
    pfDst[V_X] *= fInvMag; \
    pfDst[V_Y] *= fInvMag; \
}

/**
 * Normalise Vec2: (a1)... = normalise((a0)...)
 */
#define v2_normalise(T, UNION_NAME, fac) { \
    T*       pfDst   = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T const* pfSrc   = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    T        fInvMag = fac / std::sqrt(dot2(pfSrc, pfSrc)); \
    pfDst[V_X] = fInvMag * pfSrc[V_X]; \
    pfDst[V_Y] = fInvMag * pfSrc[V_Y]; \
}

/**
 * Normalise Vec2: (a2)... = lerp((a1)... - (a0), fp0)
 */
#define v2_interpolate(T, UNION_NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME; \
    T        fLerp  = Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME; \
    pfDst[V_X] = pfSrc2[V_X] + (fLerp * (pfSrc1[V_X] - pfSrc2[V_X])); \
    pfDst[V_Y] = pfSrc2[V_Y] + (fLerp * (pfSrc1[V_Y] - pfSrc2[V_Y])); \
}

/**
 * Transform Vec2: (a2)... = (a1)... * [(a0)...]
 */
#define v2_transform_2x2(T, UNION_NAME) \
vec2_transform_2x2<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

/**
 * Transform Vec2: (a2)... = (a1)... * [(a0)...]
 */
#define v2_0_transform_3x3(T, UNION_NAME) \
vec2_0_transform_3x3<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

/**
 * Transform Vec2: (a2)... = (a1)... * [(a0)...]
 */
#define v2_1_transform_3x3(T, UNION_NAME) \
vec2_1_transform_3x3<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

/**
 * Expand Vec2: (a1)... = (a0)..., fp0
 */
#define v2_expand_v3(T, UNION_NAME) \
vec2_expand_vec3<T>( \
    aoGPR[ABI::PTR_REG_1].pf ## UNION_NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## UNION_NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## UNION_NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

}}}

#endif

