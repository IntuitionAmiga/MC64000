#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_VEC3_MACROS_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_VEC3_MACROS_HPP__

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
 * Gnarly macros for 3-component vectors
 */
#define dot3(pfV3A, pfV3B) (pfV3A[V_X] * pfV3B[V_X] + pfV3A[V_Y] * pfV3B[V_Y] + pfV3A[V_Z] * pfV3B[V_Z])

#define v3_splat(T, NAME) { \
    T* pfDst = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME; \
    pfDst[V_X] = f; \
    pfDst[V_Y] = f; \
    pfDst[V_Z] = f; \
}

#define v3_copy(T, NAME) { \
    T*       pDst = aoGPR[ABI::PTR_REG_1].p ## NAME; \
    T const* pSrc = aoGPR[ABI::PTR_REG_0].p ## NAME; \
    pDst[V_X] = pSrc[V_X]; \
    pDst[V_Y] = pSrc[V_Y]; \
    pDst[V_Z] = pSrc[V_Z]; \
}

#define v3_scale_assign(T, NAME) { \
    T* pfDst = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME; \
    pfDst[V_X] *= f; \
    pfDst[V_Y] *= f; \
    pfDst[V_Z] *= f; \
}

#define v3_scale(T, NAME) { \
    T*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T        f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME; \
    pfDst[V_X] = f * pfSrc[V_X]; \
    pfDst[V_Y] = f * pfSrc[V_Y]; \
    pfDst[V_Z] = f * pfSrc[V_Z]; \
}

#define v3_add_assign(T, NAME) { \
    T*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    pfDst[V_X] += pfSrc[V_X]; \
    pfDst[V_Y] += pfSrc[V_Y]; \
    pfDst[V_Z] += pfSrc[V_Z]; \
}

#define v3_add(T, NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    pfDst[V_X] = pfSrc1[V_X] + pfSrc2[V_X]; \
    pfDst[V_Y] = pfSrc1[V_Y] + pfSrc2[V_Y]; \
    pfDst[V_Z] = pfSrc1[V_Z] + pfSrc2[V_Z]; \
}

#define v3_sub_assign(T, NAME) { \
    T*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    pfDst[V_X] -= pfSrc[V_X]; \
    pfDst[V_Y] -= pfSrc[V_Y]; \
    pfDst[V_Z] -= pfSrc[V_Z]; \
}

#define v3_sub(T, NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    pfDst[V_X] = pfSrc1[V_X] - pfSrc2[V_X]; \
    pfDst[V_Y] = pfSrc1[V_Y] - pfSrc2[V_Y]; \
    pfDst[V_Z] = pfSrc1[V_Z] - pfSrc2[V_Z]; \
}

#define v3_dot(T, NAME) { \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME = dot3(pfSrc1, pfSrc2); \
}

// x =  v1.y * v2.z - v1.z * v2.y,
// y =  v1.z * v2.x - v1.x * v2.z,
// z =  v1.x * v2.y - v1.y * v2.x
#define v3_cross_assign(T, NAME) { \
    T* pfDst       = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T x = pfDst[V_Y] * pfSrc[V_Z] - pfDst[V_Z] * pfSrc[V_Y]; \
    T y = pfDst[V_Z] * pfSrc[V_X] - pfDst[V_X] * pfSrc[V_Z]; \
    T z = pfDst[V_X] * pfSrc[V_Y] - pfDst[V_Y] * pfSrc[V_X]; \
    pfDst[V_X] = x; \
    pfDst[V_Y] = y; \
    pfDst[V_Z] = z; \
}

#define v3_cross(T, NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    pfDst[V_X] = pfSrc1[V_Y] * pfSrc2[V_Z] - pfSrc1[V_Z] * pfSrc2[V_Y]; \
    pfDst[V_Y] = pfSrc1[V_Z] * pfSrc2[V_X] - pfSrc1[V_X] * pfSrc2[V_Z]; \
    pfDst[V_Z] = pfSrc1[V_X] * pfSrc2[V_Y] - pfSrc1[V_Y] * pfSrc2[V_X]; \
}

#define v3_magnitude(T, NAME) { \
    T const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME = std::sqrt(dot3(pfSrc, pfSrc)); \
}

#define v3_normalise_assign(T, NAME, fac) { \
    T* pfDst   = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T  fInvMag = fac / std::sqrt(dot3(pfDst, pfDst)); \
    pfDst[V_X] *= fInvMag; \
    pfDst[V_Y] *= fInvMag; \
    pfDst[V_Z] *= fInvMag; \
}

#define v3_normalise(T, NAME, fac) { \
    T*       pfDst   = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc   = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T        fInvMag = fac / std::sqrt(dot3(pfSrc, pfSrc)); \
    pfDst[V_X] = fInvMag * pfSrc[V_X]; \
    pfDst[V_Y] = fInvMag * pfSrc[V_Y]; \
    pfDst[V_Z] = fInvMag * pfSrc[V_Z]; \
}

#define v3_interpolate(T, NAME) { \
    T*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## NAME; \
    T const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## NAME; \
    T const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## NAME; \
    T        fLerp  = Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME; \
    pfDst[V_X] = pfSrc2[V_X] + (fLerp * (pfSrc1[V_X] - pfSrc2[V_X])); \
    pfDst[V_Y] = pfSrc2[V_Y] + (fLerp * (pfSrc1[V_Y] - pfSrc2[V_Y])); \
    pfDst[V_Z] = pfSrc2[V_Z] + (fLerp * (pfSrc1[V_Z] - pfSrc2[V_Z])); \
}

#define v3_transform_3x3(T, NAME) \
vec3_transform_3x3<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

#define v3_0_transform_4x4(T, NAME) \
vec3_0_transform_4x4<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

#define v3_1_transform_4x4(T, NAME) \
vec3_1_transform_4x4<T>( \
    aoGPR[ABI::PTR_REG_2].pf ## NAME, \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

#define v3_expand_v4(T, NAME) \
vec3_expand_vec4<T>( \
    aoGPR[ABI::PTR_REG_1].pf ## NAME, \
    aoGPR[ABI::PTR_REG_0].pf ## NAME, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## NAME, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)

}}} // namespace
#endif
