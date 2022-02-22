#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_VEC_2_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_VEC_2_HPP__

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

#include <cmath>
#include <machine/register.hpp>
#include <host/standard_test_host.hpp>
#include "offsets.hpp"
/**
 * 2D Vector Operations
 */
namespace MC64K::StandardTestHost::VectorMath {

/**
 * 2D Dot Product
 */
template<typename T>
inline constexpr T dot2(T const* pfVec2a, T const* pfVec2b) {
    return pfVec2a[V_X] * pfVec2b[V_X] + pfVec2a[V_Y] * pfVec2b[V_Y];
}

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
 * vec2(a0) = fp0
 */
template<typename T>
inline void v2_splat() {
    T* pfDst = Interpreter::gpr<ABI::PTR_REG_0>().address<T>();
    T  fVal  = Interpreter::fpr<ABI::FLT_REG_0>().value<T>();
    pfDst[V_X] = fVal;
    pfDst[V_Y] = fVal;
}

/**
 * vec2(a1) = vec2(a0)
 */
template<typename T>
inline void v2_copy() {
    T*       pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] = pfSrc[V_X];
    pfDst[V_Y] = pfSrc[V_Y];
}

/**
 * vec2(a0) *= fp0
 */
template<typename T>
inline void v2_scale_assign() {
    T* pfDst = Interpreter::gpr<ABI::PTR_REG_0>().address<T>();
    T  fVal  = Interpreter::fpr<ABI::FLT_REG_0>().value<T>();
    pfDst[V_X] *= fVal;
    pfDst[V_Y] *= fVal;
}

/**
 * vec2(a1) = fp0 * vec2(a0)
 */
template<typename T>
inline void v2_scale() {
    T*       pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    T        fVal  = Interpreter::fpr<ABI::FLT_REG_0>().value<T>();
    pfDst[V_X] = fVal * pfSrc[V_X];
    pfDst[V_Y] = fVal * pfSrc[V_Y];
}

/**
 * vec2(a1) += vec2(a0)
 */
template<typename T>
inline void v2_add_assign() {
    T*       pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] += pfSrc[V_X];
    pfDst[V_Y] += pfSrc[V_Y];
}

/**
 * vec2(a2) = vec2(a1) + vec2(a0)
 */
template<typename T>
inline void v2_add() {
    T*       pfDst  = Interpreter::gpr<ABI::PTR_REG_2>().address<T>();
    T const* pfSrc1 = Interpreter::gpr<ABI::PTR_REG_1>().address<T const>();
    T const* pfSrc2 = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] = pfSrc1[V_X] + pfSrc2[V_X];
    pfDst[V_Y] = pfSrc1[V_Y] + pfSrc2[V_Y];
}

/**
 * vec2(a1) -= vec2(a0)
 */
template<typename T>
inline void v2_sub_assign() {
    T*       pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] -= pfSrc[V_X];
    pfDst[V_Y] -= pfSrc[V_Y];
}

/**
 * vec2(a2) = vec2(a1) - vec2(a0)
 */
template<typename T>
inline void v2_sub() {
    T*       pfDst  = Interpreter::gpr<ABI::PTR_REG_2>().address<T>();
    T const* pfSrc1 = Interpreter::gpr<ABI::PTR_REG_1>().address<T const>();
    T const* pfSrc2 = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] = pfSrc1[V_X] - pfSrc2[V_X];
    pfDst[V_Y] = pfSrc1[V_Y] - pfSrc2[V_Y];
}

/**
 * fp0 = vec2(a1) . vec2(a0)
 */
template<typename T>
inline void v2_dot() {
    Interpreter::fpr<ABI::FLT_REG_0>().value<T>() = dot2(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * fp0 = |vec2(a0)|
 */
template<typename T>
inline void v2_magnitude() {
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    Interpreter::fpr<ABI::FLT_REG_0>().value<T>() = (T)std::sqrt(dot2(pfSrc, pfSrc));
}

/**
 * vec2(a0) *= 1.0 / |vec2(a0)|
 */
template<typename T>
inline void v2_normalise_assign() {
    T const fOne    = 1;
    T*      pfDst   = Interpreter::gpr<ABI::PTR_REG_0>().address<T>();
    T       fInvMag = fOne / std::sqrt(dot2(pfDst, pfDst));
    pfDst[V_X] *= fInvMag;
    pfDst[V_Y] *= fInvMag;
}

/**
 * vec2(a1) = vec2(a0) * (1.0 / |vec2(a0)|)
 */
template<typename T>
inline void v2_normalise() {
    T const  fOne    = 1;
    T*       pfDst   = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc   = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    T        fInvMag = fOne / std::sqrt(dot2(pfSrc, pfSrc));
    pfDst[V_X] = fInvMag * pfSrc[V_X];
    pfDst[V_Y] = fInvMag * pfSrc[V_Y];
}

/**
 * vec2(a2) = lerp(vec2(a1) - vec2(a0), vec2(a0), fp0)
 */
template<typename T>
inline void v2_interpolate() {
    T*       pfDst  = Interpreter::gpr<ABI::PTR_REG_2>().address<T>();
    T const* pfSrc1 = Interpreter::gpr<ABI::PTR_REG_1>().address<T const>();
    T const* pfSrc2 = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    T        fLerp  = Interpreter::fpr<ABI::FLT_REG_0>().value<T>();
    pfDst[V_X] = pfSrc2[V_X] + (fLerp * (pfSrc1[V_X] - pfSrc2[V_X]));
    pfDst[V_Y] = pfSrc2[V_Y] + (fLerp * (pfSrc1[V_Y] - pfSrc2[V_Y]));
}

/**
 * vec2[0 ... d0-1](a2) = m2x2(a1) x vec2[0 ... d0-1](a1)
 */
template<typename T>
inline void v2_transform_2x2() {
    vec2_transform_2x2<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

/**
 * vec2[0 ... d0-1](a2) = m3x3(a0) x vec2[0 ... d0-1](a1)
 *
 * Treats vec2 as having implicit Z = 0.
 */
template<typename T>
inline void v2_0_transform_3x3() {
    vec2_0_transform_3x3<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

/**
 * vec2[0 ... d0-1](a2) = m3x3(a0) x vec2[0 ... d0-1](a1)
 *
 * Treats vec2 as having implicit Z = 1.
 */
template<typename T>
inline void v2_1_transform_3x3() {
    vec2_1_transform_3x3<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

/**
 * vec3[0 ... d0-1](a1) = vec2[0 ... d0-1](a0)
 *
 * Populates Z component from fp0
 */
template<typename T>
inline void v2_expand_v3() {
    vec2_expand_vec3<T>( \
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

} // namespace

#endif

