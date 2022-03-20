#ifndef MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_2X2_HPP
    #define MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_2X2_HPP

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

#include <machine/register.hpp>
#include "matrix_common.hpp"

#ifdef MATRIX_FORCE_DOUBLE
#define MT T
#else
#define MT float64
#endif

namespace MC64K::StandardTestHost::VectorMath {

/**
 * Mat2x2 multiplication.
 *
 * C = A x B, A is by row, B is by column
 */
template<typename T>
inline void mat2x2_multiply(T* pfDst, T const* pfSrcA, T const* pfSrcB) {
    // Row 1
    pfDst[M2_11] = (T)(
        ((MT)pfSrcA[M2_11] * (MT)pfSrcB[M2_11]) +
        ((MT)pfSrcA[M2_12] * (MT)pfSrcB[M2_21])
    );
    pfDst[M2_12] = (T)(
        ((MT)pfSrcA[M2_11] * (MT)pfSrcB[M2_12]) +
        ((MT)pfSrcA[M2_12] * (MT)pfSrcB[M2_22])
    );

    // Row 2
    pfDst[M2_21] = (T)(
        ((MT)pfSrcA[M2_21] * (MT)pfSrcB[M2_11]) +
        ((MT)pfSrcA[M2_22] * (MT)pfSrcB[M2_21])
    );
    pfDst[M2_22] = (T)(
        ((MT)pfSrcA[M2_21] * (MT)pfSrcB[M2_12]) +
        ((MT)pfSrcA[M2_22] * (MT)pfSrcB[M2_22])
    );
}

template<typename T>
inline void mat2x2_transpose(T* pDst, T const* pSrc) {
    pDst[M2_11] = pSrc[M2_11];
    pDst[M2_12] = pSrc[M2_21];
    pDst[M2_21] = pSrc[M2_12];
    pDst[M2_22] = pSrc[M2_22];
}

template<typename T>
inline T mat2x2_determinant(T const* pfSrc) {
    return pfSrc[M2_11] * pfSrc[M2_22] - pfSrc[M2_12] * pfSrc[M2_21];
}

template<typename T>
inline uint64 mat2x2_inverse(T* pfDst, T const* pfSrc) {
    T fDeterminant = mat2x2_determinant<T>(pfSrc);
    if (fDeterminant) {
        fDeterminant = (T)1.0 / fDeterminant;
        pfDst[M2_11] = fDeterminant * pfSrc[M2_22];
        pfDst[M2_22] = fDeterminant * pfSrc[M2_11];
        pfDst[M2_12] = -fDeterminant * pfSrc[M2_12];
        pfDst[M2_21] = -fDeterminant * pfSrc[M2_21];
        return ABI::ERR_NONE;
    } else {
        return ERR_ZERO_DIVIDE;
    }
}

/**
 * m2x2(a0) = I
 */
template<typename T>
inline void m2x2_identity() {
    mat_identity<T, 2>(Interpreter::gpr<ABI::PTR_REG_0>().address<T>());
}

/**
 * m2x2(a1) = m2x2(a0)
 */
template<typename T>
inline void m2x2_copy() {
    mat_copy<T, 2>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m2x2(a0) *= fp0
 */
template<typename T>
inline void m2x2_scale_assign() {
    mat_scale_assign<T, 2>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

/**
 * m2x2(a1) = fp0 * m2x2(a0)
 */
template<typename T>
inline void m2x2_scale() {
    mat_scale<T, 2>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

/**
 * m2x2(a1) += m2x2(a0)
 */
template<typename T>
inline void m2x2_add_assign() {
    mat_add_assign<T, 2>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m2x2(a2) = m2x2(a1) + m2x2(a0)
 */
template<typename T>
inline void m2x2_add() {
    mat_add<T, 2>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m2x2(a1) -= m2x2(a0)
 */
template<typename T>
inline void m2x2_sub_assign() {
    mat_sub_assign<T, 2>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m2x2(a2) = m2x2(a1) - m2x2(a0)
 */
template<typename T>
inline void m2x2_sub() {
    mat_sub<T, 2>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m2x2(a2) = m2x2(a1) x m2x2(a0)
 */
template<typename T>
inline void m2x2_multiply() {
    mat2x2_multiply<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m2x2(a1) x= m2x2(a0)
 */
template<typename T>
inline void m2x2_multiply_assign() {
    T* pfDst = Interpreter::gpr<ABI::PTR_REG_1>().address<T>();
    T pfTmp[4];
    mat_copy<T, 2>(pfTmp, pfDst);
    mat2x2_multiply<T>(
        pfDst,
        pfTmp,
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m2x2(a1) = m2x2^T(a0)
 */
template<typename T>
inline void m2x2_transpose() {
    mat2x2_transpose<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * fp0 = det m2x2(a0)
 */
template<typename T>
inline void m2x2_determinant() {
    Interpreter::fpr<ABI::FLT_REG_0>().value<T>() = mat2x2_determinant<T>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m2x2(a1) = m2x2^-1(a0)
 */
template<typename T>
inline void m2x2_inverse() {
    Interpreter::gpr<ABI::INT_REG_0>().uQuad = mat2x2_inverse<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

}

#undef MT
#endif

