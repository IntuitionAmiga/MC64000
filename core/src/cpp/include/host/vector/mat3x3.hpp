#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_3X3_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_3X3_HPP__

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

#include "templates.hpp"
#include <machine/register.hpp>

namespace MC64K::StandardTestHost::VectorMath {

template<typename T>
inline void m3x3_identity() {
    mat_identity<T, 3>(Interpreter::gpr<ABI::PTR_REG_0>().address<T>());
}

template<typename T>
inline void m3x3_copy() {
    mat_copy<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m3x3_scale_assign() {
    mat_scale_assign<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

template<typename T>
inline void m3x3_scale() {
    mat_scale<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

template<typename T>
inline void m3x3_add_assign() {
    mat_add_assign<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m3x3_add() {
    mat_add<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m3x3_sub_assign() {
    mat_sub_assign<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m3x3_sub() {
    mat_sub<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m3x3_multiply() {
    mat3x3_multiply<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m3x3_multiply_assign() {
    T* pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T pfTmp[9];
    mat_copy<T, 3>(pfTmp, pfDst);
    mat3x3_multiply<T>(
        pfDst,
        pfTmp,
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m3x3_transpose() {
    mat3x3_transpose<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m3x3_determinant() {
    Interpreter::fpr<ABI::FLT_REG_0>().value<T>() = mat3x3_determinant<T>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m3x3_inverse() {
    Interpreter::gpr<ABI::INT_REG_0>().uQuad = mat3x3_inverse<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
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

}

#endif

