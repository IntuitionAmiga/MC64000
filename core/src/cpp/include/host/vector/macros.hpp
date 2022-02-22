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

#include "./offsets.hpp"
#include "./templates.hpp"
#include "./vec2_macros.hpp"
#include "./vec3_macros.hpp"

namespace MC64K::StandardTestHost::VectorMath {

// // Explicit template instantiations
//
// template void vec2_transform_2x2(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
// template void vec2_0_transform_3x3(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
// template void vec2_1_transform_3x3(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
// template void vec2_expand_vec3(float32* pfDst, float32 const* pfSrc, float32 fValue, size_t uCount);
// template void vec3_transform_3x3(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
// template void vec3_0_transform_4x4(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
// template void vec3_1_transform_4x4(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
// template void vec3_expand_vec4(float32* pfDst, float32 const* pfSrc, float32 fValue, size_t uCount);
// template void vec4_transform_4x4(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif
