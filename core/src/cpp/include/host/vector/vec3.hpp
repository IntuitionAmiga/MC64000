#ifndef MC64K_STANDARD_TEST_HOST_VECTOR_MATH_VEC_3_HPP
    #define MC64K_STANDARD_TEST_HOST_VECTOR_MATH_VEC_3_HPP

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
#include "offsets.hpp"

/**
 * 3D Vector Operations
 */
namespace MC64K::StandardTestHost::VectorMath {

/**
 * 3D Dot Product
 */
template<typename T>
inline constexpr T dot3(T const* pfVec3a, T const* pfVec3b) {
    return pfVec3a[V_X] * pfVec3b[V_X] + pfVec3a[V_Y] * pfVec3b[V_Y] + pfVec3a[V_Z] * pfVec3b[V_Z];
}


/**
 * Applies a Mat3x3 to an input set of Vec3
 */
template<typename T>
inline void vec3_transform_3x3(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M3_11] * pfSrc[V_X] + pfM[M3_12] * pfSrc[V_Y] + pfM[M3_13] * pfSrc[V_Z];
        *pfDst++ = pfM[M3_21] * pfSrc[V_X] + pfM[M3_22] * pfSrc[V_Y] + pfM[M3_23] * pfSrc[V_Z];
        *pfDst++ = pfM[M3_31] * pfSrc[V_X] + pfM[M3_32] * pfSrc[V_Y] + pfM[M3_33] * pfSrc[V_Z];
        pfSrc += 3;
    }
}

/**
 * Applies a Mat4x4 to a input set of Vec3, substituting zero for the missing 4rd element of each one.
 * Output is also a set of Vec3. Only the first 3 rows of the matrix are considered.
 */
template<typename T>
inline void vec3_0_transform_4x4(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M4_11] * pfSrc[V_X] + pfM[M4_12] * pfSrc[V_Y] + pfM[M4_13] * pfSrc[V_Z];
        *pfDst++ = pfM[M4_21] * pfSrc[V_X] + pfM[M4_22] * pfSrc[V_Y] + pfM[M4_23] * pfSrc[V_Z];
        *pfDst++ = pfM[M4_31] * pfSrc[V_X] + pfM[M4_32] * pfSrc[V_Y] + pfM[M4_33] * pfSrc[V_Z];
        pfSrc += 3;
    }
}

/**
 * Applies a Mat4x4 to a input set of Vec3, substituting one for the missing 4rd element of each one.
 * Output is also a set of Vec3. Only the first thee rows of the matrix are considered.
 */
template<typename T>
inline void vec3_1_transform_4x4(T* pfDst, T const* pfSrc, T const* pfM, size_t uCount) {
    while (uCount--) {
        *pfDst++ = pfM[M4_11] * pfSrc[V_X] + pfM[M4_12] * pfSrc[V_Y] + pfM[M4_13] * pfSrc[V_Z] + pfM[M4_14];
        *pfDst++ = pfM[M4_21] * pfSrc[V_X] + pfM[M4_22] * pfSrc[V_Y] + pfM[M4_23] * pfSrc[V_Z] + pfM[M4_24];
        *pfDst++ = pfM[M4_31] * pfSrc[V_X] + pfM[M4_32] * pfSrc[V_Y] + pfM[M4_44] * pfSrc[V_Z] + pfM[M4_34];
        pfSrc += 3;
    }
}

/**
 * Vec3 to Vec4 expand
 */
template<typename T>
inline void vec3_expand_vec4(T* pfDst, T const* pfSrc, T fValue, size_t uCount) {
    while (uCount--) {
        *pfDst++ = *pfSrc++;
        *pfDst++ = *pfSrc++;
        *pfDst++ = *pfSrc++;
        *pfDst++ = fValue;
    }
}

/**
 * vec3(a0) = fp0
 */
template<typename T>
inline void v3_splat() {
    T* pfDst = Interpreter::gpr<ABI::PTR_REG_0>().address<T>();
    T  fVal  = Interpreter::fpr<ABI::FLT_REG_0>().value<T>();
    pfDst[V_X] = fVal;
    pfDst[V_Y] = fVal;
    pfDst[V_Z] = fVal;
}

/**
 * vec3(a1) = vec3(a0)
 */
template<typename T>
inline void v3_copy() {
    T*       pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] = pfSrc[V_X];
    pfDst[V_Y] = pfSrc[V_Y];
    pfDst[V_Z] = pfSrc[V_Z];
}

/**
 * vec3(a0) *= fp0
 */
template<typename T>
inline void v3_scale_assign() {
    T* pfDst = Interpreter::gpr<ABI::PTR_REG_0>().address<T>();
    T  fVal  = Interpreter::fpr<ABI::FLT_REG_0>().value<T>();
    pfDst[V_X] *= fVal;
    pfDst[V_Y] *= fVal;
    pfDst[V_Z] *= fVal;
}

/**
 * vec3(a1) = fp0 * vec3(a0)
 */
template<typename T>
inline void v3_scale() {
    T*       pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    T        fVal  = Interpreter::fpr<ABI::FLT_REG_0>().value<T>();
    pfDst[V_X] = fVal * pfSrc[V_X];
    pfDst[V_Y] = fVal * pfSrc[V_Y];
    pfDst[V_Z] = fVal * pfSrc[V_Z];
}


/**
 * vec3(a1) += vec3(a0)
 */
template<typename T>
inline void v3_add_assign() {
    T*       pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] += pfSrc[V_X];
    pfDst[V_Y] += pfSrc[V_Y];
    pfDst[V_Z] += pfSrc[V_Z];
}

/**
 * vec3(a2) = vec3(a1) + vec3(a0)
 */
template<typename T>
inline void v3_add() {
    T*       pfDst  = Interpreter::gpr<ABI::PTR_REG_2>().address<T>();
    T const* pfSrc1 = Interpreter::gpr<ABI::PTR_REG_1>().address<T const>();
    T const* pfSrc2 = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] = pfSrc1[V_X] + pfSrc2[V_X];
    pfDst[V_Y] = pfSrc1[V_Y] + pfSrc2[V_Y];
    pfDst[V_Z] = pfSrc1[V_Z] + pfSrc2[V_Z];
}

/**
 * vec3(a1) -= vec3(a0)
 */
template<typename T>
inline void v3_sub_assign() {
    T*       pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] -= pfSrc[V_X];
    pfDst[V_Y] -= pfSrc[V_Y];
    pfDst[V_Z] -= pfSrc[V_Z];
}

/**
 * vec3(a2) = vec3(a1) - vec3(a0)
 */
template<typename T>
inline void v3_sub() {
    T*       pfDst  = Interpreter::gpr<ABI::PTR_REG_2>().address<T>();
    T const* pfSrc1 = Interpreter::gpr<ABI::PTR_REG_1>().address<T const>();
    T const* pfSrc2 = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] = pfSrc1[V_X] - pfSrc2[V_X];
    pfDst[V_Y] = pfSrc1[V_Y] - pfSrc2[V_Y];
    pfDst[V_Z] = pfSrc1[V_Z] - pfSrc2[V_Z];
}

/**
 * fp0 = vec3(a1) . vec3(a0)
 */
template<typename T>
inline void v3_dot() {
    Interpreter::fpr<ABI::FLT_REG_0>().value<T>() = dot3(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}



/**
 * vec3(a1) x= vec3(a0)
 */
template<typename T>
inline void v3_cross_assign() {
    T*       pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    T fX = pfDst[V_Y] * pfSrc[V_Z] - pfDst[V_Z] * pfSrc[V_Y]; // x = v1.y * v2.z - v1.z * v2.y
    T fY = pfDst[V_Z] * pfSrc[V_X] - pfDst[V_X] * pfSrc[V_Z]; // y = v1.z * v2.x - v1.x * v2.z
    T fZ = pfDst[V_X] * pfSrc[V_Y] - pfDst[V_Y] * pfSrc[V_X]; // z = v1.x * v2.y - v1.y * v2.x
    pfDst[V_X] = fX;
    pfDst[V_Y] = fY;
    pfDst[V_Z] = fZ;
}

/**
 * vec3(a2) = vec3(a1) x vec3(a0)
 */
template<typename T>
inline void v3_cross() {
    T*       pfDst  = Interpreter::gpr<ABI::PTR_REG_2>().address<T>();
    T const* pfSrc1 = Interpreter::gpr<ABI::PTR_REG_1>().address<T const>();
    T const* pfSrc2 = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    pfDst[V_X] = pfSrc1[V_Y] * pfSrc2[V_Z] - pfSrc1[V_Z] * pfSrc2[V_Y]; // x = v1.y * v2.z - v1.z * v2.y
    pfDst[V_Y] = pfSrc1[V_Z] * pfSrc2[V_X] - pfSrc1[V_X] * pfSrc2[V_Z]; // y = v1.z * v2.x - v1.x * v2.z
    pfDst[V_Z] = pfSrc1[V_X] * pfSrc2[V_Y] - pfSrc1[V_Y] * pfSrc2[V_X]; // z = v1.x * v2.y - v1.y * v2.x
}

/**
 * fp0 = |vec3(a0)|
 */
template<typename T>
inline void v3_magnitude() {
    T const* pfSrc = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    Interpreter::fpr<ABI::FLT_REG_0>().value<T>() = (T)std::sqrt(dot3(pfSrc, pfSrc));
}

/**
 * vec3(a0) *= 1.0 / |vec3(a0)|
 */
template<typename T>
inline void v3_normalise_assign() {
    T const fOne    = 1;
    T*      pfDst   = Interpreter::gpr<ABI::PTR_REG_0>().address<T>();
    T       fInvMag = fOne / std::sqrt(dot3(pfDst, pfDst));
    pfDst[V_X] *= fInvMag;
    pfDst[V_Y] *= fInvMag;
    pfDst[V_Z] *= fInvMag;
}

/**
 * vec3(a1) = vec3(a0) * (1.0 / |vec3(a0)|)
 */
template<typename T>
inline void v3_normalise() {
    T const  fOne    = 1;
    T*       pfDst   = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T const* pfSrc   = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    T        fInvMag = fOne / std::sqrt(dot3(pfSrc, pfSrc));
    pfDst[V_X] = fInvMag * pfSrc[V_X];
    pfDst[V_Y] = fInvMag * pfSrc[V_Y];
    pfDst[V_Z] = fInvMag * pfSrc[V_Y];
}

/**
 * vec3(a2) = lerp(vec3(a1) - vec3(a0), vec3(a0), fp0)
 */
template<typename T>
inline void v3_interpolate() {
    T*       pfDst  = Interpreter::gpr<ABI::PTR_REG_2>().address<T>();
    T const* pfSrc1 = Interpreter::gpr<ABI::PTR_REG_1>().address<T const>();
    T const* pfSrc2 = Interpreter::gpr<ABI::PTR_REG_0>().address<T const>();
    T        fLerp  = Interpreter::fpr<ABI::FLT_REG_0>().value<T>();
    pfDst[V_X] = pfSrc2[V_X] + (fLerp * (pfSrc1[V_X] - pfSrc2[V_X]));
    pfDst[V_Y] = pfSrc2[V_Y] + (fLerp * (pfSrc1[V_Y] - pfSrc2[V_Y]));
    pfDst[V_Z] = pfSrc2[V_Z] + (fLerp * (pfSrc1[V_Z] - pfSrc2[V_Z]));
}

/**
 * vec3[0 ... d0-1](a2) = m3x3(a1) x vec3[0 ... d0-1](a1)
 */
template<typename T>
inline void v3_transform_3x3() {
    vec3_transform_3x3<T>( \
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

/**
 * vec3[0 ... d0-1](a2) = m4x4(a0) x vec3[0 ... d0-1](a1)
 *
 * Treats vec3 as having implicit W = 0.
 */
template<typename T>
inline void v3_0_transform_4x4() {
    vec3_0_transform_4x4<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

/**
 * vec3[0 ... d0-1](a2) = m4x4(a0) x vec3[0 ... d0-1](a1)
 *
 * Treats vec3 as having implicit W = 1.
 */
template<typename T>
inline void v3_1_transform_4x4() {
    vec3_1_transform_4x4<T>( \
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

/**
 * vec4[0 ... d0-1](a1) = vec3[0 ... d0-1](a0)
 *
 * Populates W component from fp0
 */
template<typename T>
inline void v3_expand_v4() {
    vec3_expand_vec4<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

}
#endif

