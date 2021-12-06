#ifndef __MC64K_STANDARD_TEST_HOST_VEC3_HPP__
    #define __MC64K_STANDARD_TEST_HOST_VEC3_HPP__

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

#include "standard_test_host.hpp"

namespace MC64K {
namespace StandardTestHost {

/**
 * Vec3 Namespace, for accelerated operations on 3-component vectors
 *
 */
namespace Vec3 {

/**
 * Call
 *
 * Enumeration of calls in the Simd namespace
 */
typedef enum {
    VEC3F_SPLAT = 0,    // (a0) = fp0
    VEC3F_COPY,         // (a1) = (a0)
    VEC3F_SCALE_AS,     // (a0) *= fp0
    VEC3F_SCALE,        // (a1) = (a0) * fp0
    VEC3F_ADD_AS,       // (a1) += (a0)
    VEC3F_ADD,          // (a2) = (a1) + (a0)
    VEC3F_SUB_AS,       // (a1) -= (a0)
    VEC3F_SUB,          // (a2) = (a1) - (a0)
    VEC3F_DOT,          // fp0 = (a0) . (a1)
    VEC3F_CROSS_AS,     // (a1) = (a1) x (a0)
    VEC3F_CROSS,        // (a2) = (a1) x (a0)
    VEC3F_MAGN,         // fp0 = |(a0)|
    VEC3F_NORM_AS,      // (a0) = norm((a0))
    VEC3F_NORM,         // (a1) = norm((a0))
    VEC3F_LERP,         // (a2) = lerp((a1) - (a0), fp0)
    VEC3F_XFORM,        // reserved

    VEC3D_SPLAT,        // (a0) = fp0
    VEC3D_COPY,         // (a1) = (a0)
    VEC3D_SCALE_AS,     // (a0) *= fp0
    VEC3D_SCALE,        // (a1) = (a0) * fp0
    VEC3D_ADD_AS,       // (a1) += (a0)
    VEC3D_ADD,          // (a2) = (a1) + (a0)
    VEC3D_SUB_AS,       // (a1) -= (a0)
    VEC3D_SUB,          // (a2) = (a1) - (a0)
    VEC3D_DOT,          // fp0 = (a0) . (a1)
    VEC3D_CROSS_AS,     // (a1) = (a1) x (a0)
    VEC3D_CROSS,        // (a2) = (a1) x (a0)
    VEC3D_MAGN,         // fp0 = |(a0)|
    VEC3D_NORM_AS,      // (a0) = norm((a0))
    VEC3D_NORM,         // (a1) = norm((a0))
    VEC3D_LERP,         // (a2) = lerp((a1) - (a0), fp0)
    VEC3D_XFORM,        // reserved
} Call;

Interpreter::Status hostVector(uint8 uFunctionID);

}}} // namespace

#endif
