#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MACROS_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MACROS_HPP__

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

namespace MC64K {
namespace StandardTestHost {
namespace VectorMath {

/**
 * Vec2 to Vec3 expand
 */
template<typename T>
void vec2_expand_vec3(const T* pfSrc, T* pfDst, T fValue, size_t uCount) {
    while (uCount--) {
        *pfDst++ = *pfSrc++;
        *pfDst++ = *pfSrc++;
        *pfDst++ = fValue;
    }
}

/**
 * Vec3 to Vec4 expand
 */
template<typename T>
void vec3_expand_vec4(const T* pfSrc, T* pfDst, T fValue, size_t uCount) {
    while (uCount--) {
        *pfDst++ = *pfSrc++;
        *pfDst++ = *pfSrc++;
        *pfDst++ = *pfSrc++;
        *pfDst++ = fValue;
    }
}

/**
 * Applies a Mat2x2 to an input set of Vec2
 */
template<typename T>
void vec2_transform_2x2(const T* pfMatrix, const T* pfSrc, T* pfDst, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfMatrix[0] * pfSrc[0] + pfMatrix[1] * pfSrc[1];
        *pfDst++ = pfMatrix[2] * pfSrc[0] + pfMatrix[3] * pfSrc[1];
        pfSrc += 2;
    }
}

/**
 * Applies a Mat3x3 to a input set of Vec2, substituting zero for the missing 3rd element of each one.
 * Output is also a set of Vec2.
 */
template<typename T>
void vec2_0_transform_3x3(const T* pfMatrix, const T* pfSrc, T* pfDst, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfMatrix[0] * pfSrc[0] + pfMatrix[1] * pfSrc[1];
        *pfDst++ = pfMatrix[3] * pfSrc[0] + pfMatrix[4] * pfSrc[1];
        *pfDst++ = pfMatrix[6] * pfSrc[0] + pfMatrix[7] * pfSrc[1];
        pfSrc += 2;
    }
}

/**
 * Applies a Mat3x3 to a input set of Vec2, substituting one for the missing 3rd element of each one.
 * Output is also a set of Vec2.
 */
template<typename T>
void vec3_1_transform_3x3(const T* pfMatrix, const T* pfSrc, T* pfDst, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfMatrix[0] * pfSrc[0] + pfMatrix[1] * pfSrc[1] + pfMatrix[2];
        *pfDst++ = pfMatrix[3] * pfSrc[0] + pfMatrix[4] * pfSrc[1] + pfMatrix[5];
        *pfDst++ = pfMatrix[6] * pfSrc[0] + pfMatrix[7] * pfSrc[1] + pfMatrix[8];
        pfSrc += 2;
    }
}

/**
 * Applies a Mat3x3 to an input set of Vec3
 */
template<typename T>
void vec3_transform_3x3(const T* pfMatrix, const T* pfSrc, T* pfDst, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfMatrix[0] * pfSrc[0] + pfMatrix[1] * pfSrc[1] + pfMatrix[2] * pfSrc[2];
        *pfDst++ = pfMatrix[3] * pfSrc[0] + pfMatrix[4] * pfSrc[1] + pfMatrix[5] * pfSrc[2];
        *pfDst++ = pfMatrix[6] * pfSrc[0] + pfMatrix[7] * pfSrc[1] + pfMatrix[8] * pfSrc[2];
        pfSrc += 3;
    }
}



/**
 *
 * Gnarly macros for 2-component vectors
 */
#define dot2(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1])

#define v2_splat(type, alias) { \
    type* pfDst = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## alias; \
    pfDst[0] = f; \
    pfDst[1] = f; \
}

#define v2_expand_v3(type, alias) \
vec2_expand_vec3( \
    aoGPR[ABI::PTR_REG_1].p ## alias, \
    aoGPR[ABI::PTR_REG_0].p ## alias, \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## alias, \
    Interpreter::gpr()[ABI::INT_REG_0].uLong \
)


#define v2_copy(type, alias) { \
    type*       pDst = aoGPR[ABI::PTR_REG_1].p ## alias; \
    type const* pSrc = aoGPR[ABI::PTR_REG_0].p ## alias; \
    pDst[0] = pSrc[0]; \
    pDst[1] = pSrc[1]; \
}

#define v2_scale_assign(type, alias) { \
    type* pfDst = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## alias; \
    pfDst[0] *= f; \
    pfDst[1] *= f; \
}

#define v2_scale(type, alias) { \
    type*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type        f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## alias; \
    pfDst[0] = f * pfSrc[0]; \
    pfDst[1] = f * pfSrc[1]; \
}

#define v2_add_assign(type, alias) { \
    type*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    pfDst[0] += pfSrc[0]; \
    pfDst[1] += pfSrc[1]; \
}

#define v2_add(type, alias) { \
    type*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## alias; \
    type const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    pfDst[0] = pfSrc1[0] + pfSrc2[0]; \
    pfDst[1] = pfSrc1[1] + pfSrc2[1]; \
}

#define v2_sub_assign(type, alias) { \
    type*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    pfDst[0] -= pfSrc[0]; \
    pfDst[1] -= pfSrc[1]; \
}

#define v2_sub(type, alias) { \
    type*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## alias; \
    type const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    pfDst[0] = pfSrc1[0] - pfSrc2[0]; \
    pfDst[1] = pfSrc1[1] - pfSrc2[1]; \
}

#define v2_dot(type, alias) { \
    type const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## alias = dot2(pfSrc1, pfSrc2); \
}

#define v2_magnitude(type, alias) { \
    type const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## alias = std::sqrt(dot2(pfSrc, pfSrc)); \
}

#define v2_normalise_assign(type, alias, fac) { \
    type* pfDst   = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type  fInvMag = fac / std::sqrt(dot2(pfDst, pfDst)); \
    pfDst[0] *= fInvMag; \
    pfDst[1] *= fInvMag; \
}

#define v2_normalise(type, alias, fac) { \
    type*       pfDst   = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc   = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type        fInvMag = fac / std::sqrt(dot2(pfSrc, pfSrc)); \
    pfDst[0] = fInvMag * pfSrc[0]; \
    pfDst[1] = fInvMag * pfSrc[1]; \
}

#define v2_interpolate(type, alias) { \
    type*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## alias; \
    type const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type        fLerp  = Interpreter::fpr()[ABI::FLT_REG_0].f ## alias; \
    pfDst[0] = pfSrc2[0] + (fLerp * (pfSrc1[0] - pfSrc2[0])); \
    pfDst[1] = pfSrc2[1] + (fLerp * (pfSrc1[1] - pfSrc2[1])); \
}


/**
 * Gnarly macros for 3-component vectors
 */
#define dot3(a, b) ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])

#define v3_splat(type, alias) { \
    type* pfDst = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## alias; \
    pfDst[0] = f; \
    pfDst[1] = f; \
    pfDst[2] = f; \
}

#define v3_copy(type, alias) { \
    type*       pDst = aoGPR[ABI::PTR_REG_1].p ## alias; \
    type const* pSrc = aoGPR[ABI::PTR_REG_0].p ## alias; \
    pDst[0] = pSrc[0]; \
    pDst[1] = pSrc[1]; \
    pDst[2] = pSrc[2]; \
}

#define v3_scale_assign(type, alias) { \
    type* pfDst = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type  f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## alias; \
    pfDst[0] *= f; \
    pfDst[1] *= f; \
    pfDst[2] *= f; \
}

#define v3_scale(type, alias) { \
    type*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type        f     = Interpreter::fpr()[ABI::FLT_REG_0].f ## alias; \
    pfDst[0] = f * pfSrc[0]; \
    pfDst[1] = f * pfSrc[1]; \
    pfDst[2] = f * pfSrc[2]; \
}

#define v3_add_assign(type, alias) { \
    type*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    pfDst[0] += pfSrc[0]; \
    pfDst[1] += pfSrc[1]; \
    pfDst[2] += pfSrc[2]; \
}

#define v3_add(type, alias) { \
    type*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## alias; \
    type const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    pfDst[0] = pfSrc1[0] + pfSrc2[0]; \
    pfDst[1] = pfSrc1[1] + pfSrc2[1]; \
    pfDst[2] = pfSrc1[2] + pfSrc2[2]; \
}

#define v3_sub_assign(type, alias) { \
    type*       pfDst = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    pfDst[0] -= pfSrc[0]; \
    pfDst[1] -= pfSrc[1]; \
    pfDst[2] -= pfSrc[2]; \
}

#define v3_sub(type, alias) { \
    type*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## alias; \
    type const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    pfDst[0] = pfSrc1[0] - pfSrc2[0]; \
    pfDst[1] = pfSrc1[1] - pfSrc2[1]; \
    pfDst[2] = pfSrc1[2] - pfSrc2[2]; \
}

#define v3_dot(type, alias) { \
    type const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## alias = dot3(pfSrc1, pfSrc2); \
}

// x =  v1.y * v2.z - v1.z * v2.y,
// y =  v1.z * v2.x - v1.x * v2.z,
// z =  v1.x * v2.y - v1.y * v2.x
#define v3_cross_assign(type, alias) { \
    type* pfDst       = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type x = pfDst[1] * pfSrc[2] - pfDst[2] * pfSrc[1]; \
    type y = pfDst[2] * pfSrc[0] - pfDst[0] * pfSrc[2]; \
    type z = pfDst[0] * pfSrc[1] - pfDst[1] * pfSrc[0]; \
    pfDst[0] = x; \
    pfDst[1] = y; \
    pfDst[2] = z; \
}

#define v3_cross(type, alias) { \
    type*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## alias; \
    type const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    pfDst[0] = pfSrc1[1] * pfSrc2[2] - pfSrc1[2] * pfSrc2[1]; \
    pfDst[1] = pfSrc1[2] * pfSrc2[0] - pfSrc1[0] * pfSrc2[2]; \
    pfDst[2] = pfSrc1[0] * pfSrc2[1] - pfSrc1[1] * pfSrc2[0]; \
}

#define v3_magnitude(type, alias) { \
    type const* pfSrc = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    Interpreter::fpr()[ABI::FLT_REG_0].f ## alias = std::sqrt(dot3(pfSrc, pfSrc)); \
}

#define v3_normalise_assign(type, alias, fac) { \
    type* pfDst   = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type  fInvMag = fac / std::sqrt(dot3(pfDst, pfDst)); \
    pfDst[0] *= fInvMag; \
    pfDst[1] *= fInvMag; \
    pfDst[2] *= fInvMag; \
}

#define v3_normalise(type, alias, fac) { \
    type*       pfDst   = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc   = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type        fInvMag = fac / std::sqrt(dot3(pfSrc, pfSrc)); \
    pfDst[0] = fInvMag * pfSrc[0]; \
    pfDst[1] = fInvMag * pfSrc[1]; \
    pfDst[2] = fInvMag * pfSrc[2]; \
}

#define v3_interpolate(type, alias) { \
    type*       pfDst  = aoGPR[ABI::PTR_REG_2].pf ## alias; \
    type const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pf ## alias; \
    type const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pf ## alias; \
    type        fLerp  = Interpreter::fpr()[ABI::FLT_REG_0].f ## alias; \
    pfDst[0] = pfSrc2[0] + (fLerp * (pfSrc1[0] - pfSrc2[0])); \
    pfDst[1] = pfSrc2[1] + (fLerp * (pfSrc1[1] - pfSrc2[1])); \
    pfDst[2] = pfSrc2[2] + (fLerp * (pfSrc1[2] - pfSrc2[2])); \
}

}}} // namespace
#endif
