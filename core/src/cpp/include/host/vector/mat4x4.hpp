#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_4X4_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_4X4_HPP__

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
inline void m4x4_identity() {
    mat_identity<T, 4>(Interpreter::gpr<ABI::PTR_REG_0>().address<T>());
}

template<typename T>
inline void m4x4_copy() {
    mat_copy<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m4x4_scale_assign() {
    mat_scale_assign<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

template<typename T>
inline void m4x4_scale() {
    mat_scale<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

template<typename T>
inline void m4x4_add_assign() {
    mat_add_assign<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m4x4_add() {
    mat_add<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m4x4_sub_assign() {
    mat_sub_assign<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m4x4_sub() {
    mat_sub<T, 4>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m4x4_multiply() {
    mat4x4_multiply<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m4x4_multiply_assign() {
    T* pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T pfTmp[16];
    mat_copy<T, 4>(pfTmp, pfDst);
    mat4x4_multiply<T>(
        pfDst,
        pfTmp,
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m4x4_transpose() {
    mat4x4_transpose<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m4x4_determinant() {
    Interpreter::fpr<ABI::FLT_REG_0>().value<T>() = mat4x4_determinant<T>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void m4x4_inverse() {
    Interpreter::gpr<ABI::INT_REG_0>().uQuad = mat4x4_inverse<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

template<typename T>
inline void v4_transform_4x4() {
    vec4_transform_4x4<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::gpr<ABI::INT_REG_0>().uLong
    );
}

}

#endif

