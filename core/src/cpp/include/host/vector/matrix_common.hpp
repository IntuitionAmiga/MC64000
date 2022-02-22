#ifndef __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_TEMPLATES_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VECTOR_MATH_TEMPLATES_HPP__

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

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace MC64K::StandardTestHost::VectorMath {

/**
 * Generic copy routine
 */
template<typename T, unsigned const uDim>
inline void mat_copy(T* pfDst, T const* pfSrc) {
    std::memcpy(pfDst, pfSrc, uDim * uDim * sizeof(T));
}

/**
 * Generic identity routine
 */
template<typename T, unsigned const uDim>
inline void mat_identity(T* pfDst) {
    std::memset(pfDst, 0, uDim * uDim * sizeof(T));
    const T fOne = 1.0;
    for (unsigned u = 0, uIndex = 0; u < uDim; ++u, uIndex += uDim + 1) {
        pfDst[uIndex] = fOne;
    }
}

/**
 * Generic scale assign
 */
template<typename T, unsigned const uDim>
inline void mat_scale_assign(T* pfDst, T const f) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] *= f;
    }
}

/**
 * Generic scale
 */
template<typename T, unsigned const uDim>
inline void mat_scale(T* pfDst, T const* pfSrc, T const f) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] = pfSrc[u] * f;
    }
}

/**
 * Generic add assign
 */
template<typename T, unsigned const uDim>
inline void mat_add_assign(T* pfDst, T const* pfSrc) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] += pfSrc[u];
    }
}

/**
 * Generic add
 */
template<typename T, unsigned const uDim>
inline void mat_add(T* pfDst, T const* pfSrcA, T const* pfSrcB) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] = pfSrcA[u] + pfSrcB[u];
    }
}

/**
 * Generic sub assign
 */
template<typename T, unsigned const uDim>
inline void mat_sub_assign(T* pfDst, T const* pfSrc) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] -= pfSrc[u];
    }
}

/**
 * Generic sub
 */
template<typename T, unsigned const uDim>
inline void mat_sub(T* pfDst, T const* pfSrcA, T const* pfSrcB) {
    for (unsigned u=0; u < uDim * uDim; ++u) {
        pfDst[u] = pfSrcA[u] - pfSrcB[u];
    }
}

template<typename T, unsigned const uDim>
inline void mat_print(T const* pfMtx) {
    std::putchar('\n');
    for (unsigned r = 0; r < uDim; ++r) {
        std::printf("| ");
        for (unsigned c = 0; c < uDim; ++c) {
            std::printf("%0.6f ", (double) *pfMtx++);
        }
        std::printf("|\n");
    }
    std::putchar('\n');
}

} // namespace
#endif
