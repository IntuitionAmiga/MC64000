#ifndef MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_3X3_HPP
    #define MC64K_STANDARD_TEST_HOST_VECTOR_MATH_MAT_3X3_HPP

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
 * Mat3x3 multiplication
 *
 * C = A x B, A is by row, B is by column
 */
template<typename T>
inline void mat3x3_multiply(T* pfDst, T const* pfSrcA, T const* pfSrcB) {

    // Row 1
    // A: | 11 12 13 |   B: | 11 ... |
    //                      | 21 ... |
    //                      | 31 ... |
    pfDst[M3_11] = (T)(
        ((MT)pfSrcA[M3_11] * (MT)pfSrcB[M3_11]) +
        ((MT)pfSrcA[M3_12] * (MT)pfSrcB[M3_21]) +
        ((MT)pfSrcA[M3_13] * (MT)pfSrcB[M3_31])
    );

    // A: | 11 12 13 |   B: | ... 12 ... |
    //                      | ... 22 ... |
    //                      | ... 32 ... |
    pfDst[M3_12] = (T)(
        ((MT)pfSrcA[M3_11] * (MT)pfSrcB[M3_12]) +
        ((MT)pfSrcA[M3_12] * (MT)pfSrcB[M3_22]) +
        ((MT)pfSrcA[M3_13] * (MT)pfSrcB[M3_32])
    );

    // A: | 11 12 13 |   B: | ... 13 |
    //                      | ... 23 |
    //                      | ... 33 |
    pfDst[M3_13] = (T)(
        ((MT)pfSrcA[M3_11] * (MT)pfSrcB[M3_13]) +
        ((MT)pfSrcA[M3_12] * (MT)pfSrcB[M3_23]) +
        ((MT)pfSrcA[M3_13] * (MT)pfSrcB[M3_33])
    );

    // Row 2
    // A: | 21 22 23 |   B: | 11 ... |
    //                      | 21 ... |
    //                      | 31 ... |
    pfDst[M3_21] = (T)(
        ((MT)pfSrcA[M3_21] * (MT)pfSrcB[M3_11]) +
        ((MT)pfSrcA[M3_22] * (MT)pfSrcB[M3_21]) +
        ((MT)pfSrcA[M3_23] * (MT)pfSrcB[M3_31])
    );

    // A: | 21 22 23 |   B: | ... 12 ... |
    //                      | ... 22 ... |
    //                      | ... 32 ... |
    pfDst[M3_22] = (T)(
        ((MT)pfSrcA[M3_21] * (MT)pfSrcB[M3_12]) +
        ((MT)pfSrcA[M3_22] * (MT)pfSrcB[M3_22]) +
        ((MT)pfSrcA[M3_23] * (MT)pfSrcB[M3_32])
    );

    // A: | 21 22 23 |   B: | ... 13 |
    //                      | ... 23 |
    //                      | ... 33 |
    pfDst[M3_23] = (T)(
        ((MT)pfSrcA[M3_21] * (MT)pfSrcB[M3_13]) +
        ((MT)pfSrcA[M3_22] * (MT)pfSrcB[M3_23]) +
        ((MT)pfSrcA[M3_23] * (MT)pfSrcB[M3_33])
    );

    // Row 3
    // A: | 31 32 33 |   B: | 11 ... |
    //                      | 21 ... |
    //                      | 31 ... |
    pfDst[M3_31] = (T)(
        ((MT)pfSrcA[M3_31] * (MT)pfSrcB[M3_11]) +
        ((MT)pfSrcA[M3_32] * (MT)pfSrcB[M3_21]) +
        ((MT)pfSrcA[M3_33] * (MT)pfSrcB[M3_31])
    );

    // A: | 31 32 33 |   B: | ... 12 ... |
    //                      | ... 22 ... |
    //                      | ... 32 ... |
    pfDst[M3_32] = (T)(
        ((MT)pfSrcA[M3_31] * (MT)pfSrcB[M3_12]) +
        ((MT)pfSrcA[M3_32] * (MT)pfSrcB[M3_22]) +
        ((MT)pfSrcA[M3_33] * (MT)pfSrcB[M3_32])
    );

    // A: | 31 32 33 |   B: | ... 13 |
    //                      | ... 23 |
    //                      | ... 33 |
    pfDst[M3_33] = (T)(
        ((MT)pfSrcA[M3_31] * (MT)pfSrcB[M3_13]) +
        ((MT)pfSrcA[M3_32] * (MT)pfSrcB[M3_23]) +
        ((MT)pfSrcA[M3_33] * (MT)pfSrcB[M3_33])
    );
}

template<typename T>
inline void mat3x3_transpose(T* pDst, T const* pSrc) {
    pDst[M3_11] = pSrc[M3_11];
    pDst[M3_12] = pSrc[M3_21];
    pDst[M3_13] = pSrc[M3_31];
    pDst[M3_21] = pSrc[M3_12];
    pDst[M3_22] = pSrc[M3_22];
    pDst[M3_23] = pSrc[M3_32];
    pDst[M3_31] = pSrc[M3_13];
    pDst[M3_32] = pSrc[M3_23];
    pDst[M3_33] = pSrc[M3_33];
}

/**
 * Mat3x3 determinant
 *
 *  | a b c |
 *  | d e f |  => a | e f | - b | d f | + c | d e | => a(ei - fg) - b(di - fg) + c(dh - eg)
 *  | g h i |       | h i |     | g i |     | g h |
 *
 *  => aei + bfg + cdh - ceg - bdi - afh
 */
template<typename T>
inline T mat3x3_determinant(T const* pfMtx) {
    return
    /*   aei */ (pfMtx[M3_11] * pfMtx[M3_22] * pfMtx[M3_33]) +
    /* + bfg */ (pfMtx[M3_12] * pfMtx[M3_23] * pfMtx[M3_31]) +
    /* + cdh */ (pfMtx[M3_13] * pfMtx[M3_21] * pfMtx[M3_32]) -
    /* - ceg */ (pfMtx[M3_13] * pfMtx[M3_22] * pfMtx[M3_31]) -
    /* - bdi */ (pfMtx[M3_12] * pfMtx[M3_21] * pfMtx[M3_33]) -
    /* - afh */ (pfMtx[M3_11] * pfMtx[M3_23] * pfMtx[M3_32]);
}


/**
 *   | a b c | -1         1    | A B C |T            1    | A D G |
 *   | d e f |     =>   ------ | D E F |      =>   ------ | B E H |
 *   | g h i |          det(A) | G H I |           det(A) | C F I |
 *
 *  A =  (ei - fh),   D = -(bi - ch),   G =  (bf - ce)
 *  B = -(di - fg),   E =  (ai - cg),   H = -(af - cd)
 *  C =  (dh - eg),   F = -(ah - bg),   I =  (ae - bd)
 */
template<typename T>
inline uint64 mat3x3_inverse(T* pfDst, T const* pfSrc) {
    T fDeterminant = mat3x3_determinant<T>(pfSrc);
    if (fDeterminant) {
        /* A = (ei - fh) */
        pfDst[M3_11] = (pfSrc[M3_22] * pfSrc[M3_33]) - (pfSrc[M3_23] * pfSrc[M3_32]);
        /* D = (ch - bi) */
        pfDst[M3_12] = (pfSrc[M3_13] * pfSrc[M3_32]) - (pfSrc[M3_12] * pfSrc[M3_33]);
        /* G = (bf - ce) */
        pfDst[M3_13] = (pfSrc[M3_12] * pfSrc[M3_23]) - (pfSrc[M3_13] * pfSrc[M3_22]);
        /* B = (fg - di) */
        pfDst[M3_21] = (pfSrc[M3_23] * pfSrc[M3_31]) - (pfSrc[M3_21] * pfSrc[M3_33]);
        /* E = (ai - cg) */
        pfDst[M3_22] = (pfSrc[M3_11] * pfSrc[M3_33]) - (pfSrc[M3_13] * pfSrc[M3_31]);
        /* H = (cd - af) */
        pfDst[M3_23] = (pfSrc[M3_13] * pfSrc[M3_21]) - (pfSrc[M3_11] * pfSrc[M3_23]);
        /* C = (dh - eg) */
        pfDst[M3_31] = (pfSrc[M3_21] * pfSrc[M3_32]) - (pfSrc[M3_22] * pfSrc[M3_31]);
        /* F = (bg - ah) */
        pfDst[M3_32] = (pfSrc[M3_12] * pfSrc[M3_31]) - (pfSrc[M3_11] * pfSrc[M3_32]);
        /* I = (ae - bd) */
        pfDst[M3_33] = (pfSrc[M3_11] * pfSrc[M3_22]) - (pfSrc[M3_12] * pfSrc[M3_21]);
        fDeterminant = (T)1.0 / fDeterminant;                                           \
        pfDst[M3_11] *= fDeterminant;
        pfDst[M3_12] *= fDeterminant;
        pfDst[M3_13] *= fDeterminant;
        pfDst[M3_21] *= fDeterminant;
        pfDst[M3_22] *= fDeterminant;
        pfDst[M3_23] *= fDeterminant;
        pfDst[M3_31] *= fDeterminant;
        pfDst[M3_32] *= fDeterminant;
        pfDst[M3_33] *= fDeterminant;
        return ABI::ERR_NONE;
    } else {
        return ERR_ZERO_DIVIDE;
    }
}

/**
 * m3x3(a0) = I
 */
template<typename T>
inline void m3x3_identity() {
    mat_identity<T, 3>(Interpreter::gpr<ABI::PTR_REG_0>().address<T>());
}

/**
 * m3x3(a1) = m3x3(a0)
 */
template<typename T>
inline void m3x3_copy() {
    mat_copy<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m3x3(a0) *= fp0
 */
template<typename T>
inline void m3x3_scale_assign() {
    mat_scale_assign<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

/**
 * m3x3(a1) = fp0 * m3x3(a0)
 */
template<typename T>
inline void m3x3_scale() {
    mat_scale<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>(),
        Interpreter::fpr<ABI::FLT_REG_0>().value<T>()
    );
}

/**
 * m3x3(a1) += m3x3(a0)
 */
template<typename T>
inline void m3x3_add_assign() {
    mat_add_assign<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m3x3(a2) = m3x3(a1) + m3x3(a0)
 */
template<typename T>
inline void m3x3_add() {
    mat_add<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m3x3(a1) -= m3x3(a0)
 */
template<typename T>
inline void m3x3_sub_assign() {
    mat_sub_assign<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m3x3(a2) = m3x3(a1) - m3x3(a0)
 */
template<typename T>
inline void m3x3_sub() {
    mat_sub<T, 3>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m3x3(a2) = m3x3(a1) x m3x3(a0)
 */
template<typename T>
inline void m3x3_multiply() {
    mat3x3_multiply<T>(
        Interpreter::gpr<ABI::PTR_REG_2>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_1>().address<T const>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m3x3(a1) x= m3x3(a0)
 */
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

/**
 * m3x3(a1) = m3x3^T(a0)
 */
template<typename T>
inline void m3x3_transpose() {
    mat3x3_transpose<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * fp0 = det m3x3(a0)
 */
template<typename T>
inline void m3x3_determinant() {
    Interpreter::fpr<ABI::FLT_REG_0>().value<T>() = mat3x3_determinant<T>(
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

/**
 * m3x3(a1) = m3x3^-1(a0)
 */
template<typename T>
inline void m3x3_inverse() {
    Interpreter::gpr<ABI::INT_REG_0>().uQuad = mat3x3_inverse<T>(
        Interpreter::gpr<ABI::PTR_REG_1>().address<T>(),
        Interpreter::gpr<ABI::PTR_REG_0>().address<T const>()
    );
}

}

#undef MT
#endif

