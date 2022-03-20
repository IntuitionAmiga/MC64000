#ifndef MC64K_STANDARD_TEST_HOST_VECTOR_MATH_HPP
    #define MC64K_STANDARD_TEST_HOST_VECTOR_MATH_HPP

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

namespace MC64K::StandardTestHost::VectorMath {

/**
 * VectorMath Namespace, for accelerated linear algebra on 2D and 3D vectors.
 *
 * Vectors are considered to be columnar with each element arranged sequentially in memory, e.g
 *
 * | x |
 * | y | => { x, y, z }
 * | z |
 *
 * Matrices are 1D memory contiguous blocks of sequential row vectors, e.g.
 *
 * | m11 m12 m13 |
 * | m21 m22 m23 |  => {m11, m12, m13, m21, m22, m23, m31, m32, m33}
 * | m31 m32 m33 |
 *
 * Example Matrices for homogenous 2D
 *
 * Scale:
 *   | Sx  0  0 |   | x |   | Sx * x |
 *   |  0 Sy  0 | . | y | = | Sy * y |
 *   |  0  0  1 |   | 1 |   |   1    |
 *
 * Translate:
 *   |  1  0  0 |   | x |   | x + Tx |
 *   |  0  1  0 | . | y | = | y + Ty |
 *   | Tx Ty  1 |   | 1 |   |   1    |
 *
 */

/**
 * Call
 *
 * Enumeration of calls in the VectorMath namespace.
 *
 */
enum Call {
    /**
     * Init and copy
     */
    VEC2F_SPLAT = 0,    // (a0) = fp0
    VEC2F_COPY,         // (a1) = (a0)

    /**
     * Multiply by scalar
     */
    VEC2F_SCALE_AS,     // (a0) *= fp0
    VEC2F_SCALE,        // (a1) = (a0) * fp0

    /**
     * Addition/subtraction
     */
    VEC2F_ADD_AS,       // (a1) += (a0)
    VEC2F_ADD,          // (a2) = (a1) + (a0)
    VEC2F_SUB_AS,       // (a1) -= (a0)
    VEC2F_SUB,          // (a2) = (a1) - (a0)

    /**
     * Multiplication
     */
    VEC2F_DOT,          // fp0  = (a0) . (a1)

    /**
     * Magnitude / normalisation
     */
    VEC2F_MAGN,         // fp0  = |(a0)|
    VEC2F_NORM_AS,      // (a0) = norm((a0))
    VEC2F_NORM,         // (a1) = norm((a0))

    /**
     * Interpolation
     */
    VEC2F_LERP,         // (a2) = lerp((a1) - (a0), fp0)

    /**
     * Transforms a set of Vec2F by a 2x2 matrix.
     */
    VEC2F_XFRM_2X2,     // (a2) = (a1)*[(a0)], count in r0

    /**
     * Transformation
     *
     * Transforms a set of Vec2F by a 3x3 matrix. The vector is interpreted as having three components with the
     * third set to 0.0. The output continues to be Vec2F, scaled, rotated according to the matrix. Any translation
     * term of the matrix is omitted, making this suitable for direction vectors.
     *
     * If you want a strictly homogeneous coordinate system for 2D, use the Vec3F vector type and M3X3F matrix.
     */
    VEC2F_0_XFRM_3X3,     // (a2) = (a1)*[(a0)], count in r0.

    /**
     * Transformation
     *
     * Transforms a set of Vec2F by a 3x3 matrix. The vector is interpreted as having three components with the
     * third set to 1.0. The output continues to be Vec2F, scaled, rotated and translated according to the matrix.
     * As all terms of the matrix are applied, this is suitable for position vectors.
     *
     * If you want a strictly homogeneous coordinate system for 2D, use the Vec3F vector type and M3X3F matrix.
     */
    VEC2F_1_XFRM_3X3,     // (a2) = (a1)*[(a0)], count in r0.

    /**
     * Expansion
     *
     * Converts a set of Vec2F to Vec3F, setting the third component of each to the value in fp0. This can be used
     * to translate a set of 2D vectors into 3 component variants with the third component indicating direction or
     * position for use in strictly homogeneous transformations.
     */
    VEC2F_TO_VEC3F,       // (a1) = (a0), fp0, count in r0

    // vec3f float32[3]
    VEC3F_SPLAT,        // (a0) = fp0
    VEC3F_COPY,         // (a1) = (a0)
    VEC3F_SCALE_AS,     // (a0) *= fp0
    VEC3F_SCALE,        // (a1) = (a0) * fp0
    VEC3F_ADD_AS,       // (a1) += (a0)
    VEC3F_ADD,          // (a2) = (a1) + (a0)
    VEC3F_SUB_AS,       // (a1) -= (a0)
    VEC3F_SUB,          // (a2) = (a1) - (a0)
    VEC3F_DOT,          // fp0  = (a0) . (a1)
    VEC3F_CROSS_AS,     // (a1) = (a1) x (a0)
    VEC3F_CROSS,        // (a2) = (a1) x (a0)
    VEC3F_MAGN,         // fp0  = |(a0)|
    VEC3F_NORM_AS,      // (a0) = norm((a0))
    VEC3F_NORM,         // (a1) = norm((a0))
    VEC3F_LERP,         // (a2) = lerp((a1) - (a0), fp0)

    /**
     * Transforms a set of Vec3F by a 3x3 matrix.
     */
    VEC3F_XFRM_3X3,     // (a2) = (a1)*[(a0)], count in r0

    /**
     * Transformation
     *
     * Transforms a set of Vec3F by a 4x4 matrix. The vector is interpreted as having four components with the
     * foutth set to 0.0. The output continues to be Vec3F, scaled, rotated according to the matrix. Any translation
     * term of the matrix is omitted, making this suitable for direction vectors.
     *
     * If you want a strictly homogeneous coordinate system for 3D, use the Vec4F vector type and M4X4F matrix.
     */
    VEC3F_0_XFRM_4X4,     // (a2) = (a1)*[(a0)], count in r0.

    /**
     * Transformation
     *
     * Transforms a set of Vec3F by a 4x4 matrix. The vector is interpreted as having three components with the
     * fourth set to 1.0. The output continues to be Vec3F, scaled, rotated and translated according to the matrix.
     * As all terms of the matrix are applied, this is suitable for position vectors.
     *
     * If you want a strictly homogeneous coordinate system for 3D, use the Vec4F vector type and M4X4F matrix.
     */
    VEC3F_1_XFRM_4X4,     // (a2) = (a1)*[(a0)], count in r0.

    /**
     * Expansion
     *
     * Converts a set of Vec3F to Vec4F, setting the third component of each to the value in fp0. This can be used
     * to translate a set of 2D vectors into 3 component variants with the third component indicating direction or
     * position for use in strictly homogeneous transformations.
     */
    VEC3F_TO_VEC4F,       // (a1) = (a0), fp0, count in r0

    /**
     * Transformation
     *
     * Transforms a set of homogenous coordinate Vec4F by a 4x4 matrix.
     */
    VEC4F_XFORM_4X4,      // (a2) = (a1)*[(a0)], count in r0.

    // mat2x2f float32[2][2]
    M2X2F_IDENTITY,     // (a0) = identity
    M2X2F_COPY,         // (a1) = (a0)
    M2X2F_SCALE_AS,     // (a0) *= fp0
    M2X2F_SCALE,        // (a1) = (a0) * fp0
    M2X2F_ADD_AS,       // (a1) += (a0)
    M2X2F_ADD,          // (a2) = (a1) + (a0)
    M2X2F_SUB_AS,       // (a1) -= (a0)
    M2X2F_SUB,          // (a2) = (a1) - (a0)
    M2X2F_MULTIPLY_AS,  // (a1) *= (a0)
    M2X2F_MULTIPLY,     // (a2) = (a1) * (a0)
    M2X2F_TRANSPOSE,    // (a1) = Transpose(a0)
    M2X2F_DET,          // fp0  = Determinant(a0)
    M2X2F_INVERSE,      // (a1) = Inverse(a0)

    // mat3x3f float32[3][3]
    M3X3F_IDENTITY,     // (a0) = identity
    M3X3F_COPY,         // (a1) = (a0)
    M3X3F_SCALE_AS,     // (a0) *= fp0
    M3X3F_SCALE,        // (a1) = (a0) * fp0
    M3X3F_ADD_AS,       // (a1) += (a0)
    M3X3F_ADD,          // (a2) = (a1) + (a0)
    M3X3F_SUB_AS,       // (a1) -= (a0)
    M3X3F_SUB,          // (a2) = (a1) - (a0)
    M3X3F_MULTIPLY_AS,  // (a1) *= (a0)
    M3X3F_MULTIPLY,     // (a2) = (a1) * (a0)
    M3X3F_TRANSPOSE,    // (a1) = Transpose(a0)
    M3X3F_DET,          // fp0  = Determinant(a0)
    M3X3F_INVERSE,      // (a1) = Inverse(a0)

    // mat4x4f float32[4][4]
    M4X4F_IDENTITY,     // (a0) = identity
    M4X4F_COPY,         // (a1) = (a0)
    M4X4F_SCALE_AS,     // (a0) *= fp0
    M4X4F_SCALE,        // (a1) = (a0) * fp0
    M4X4F_ADD_AS,       // (a1) += (a0)
    M4X4F_ADD,          // (a2) = (a1) + (a0)
    M4X4F_SUB_AS,       // (a1) -= (a0)
    M4X4F_SUB,          // (a2) = (a1) - (a0)
    M4X4F_MULTIPLY_AS,  // (a1) *= (a0)
    M4X4F_MULTIPLY,     // (a2) = (a1) * (a0)
    M4X4F_TRANSPOSE,    // (a1) = Transpose(a0)
    M4X4F_DET,          // fp0  = Determinant(a0)
    M4X4F_INVERSE,      // (a1) = Inverse(a0)

    // Double precision
    VEC2D_SPLAT,        // (a0) = fp0
    VEC2D_COPY,         // (a1) = (a0)
    VEC2D_SCALE_AS,     // (a0) *= fp0
    VEC2D_SCALE,        // (a1) = (a0) * fp0
    VEC2D_ADD_AS,       // (a1) += (a0)
    VEC2D_ADD,          // (a2) = (a1) + (a0)
    VEC2D_SUB_AS,       // (a1) -= (a0)
    VEC2D_SUB,          // (a2) = (a1) - (a0)
    VEC2D_DOT,          // fp0  = (a0) . (a1)
    VEC2D_MAGN,         // fp0  = |(a0)|
    VEC2D_NORM_AS,      // (a0) = norm((a0))
    VEC2D_NORM,         // (a1) = norm((a0))
    VEC2D_LERP,         // (a2) = lerp((a1) - (a0), fp0)
    VEC2D_XFRM_2X2,     // (a2) = (a1)*[(a0)], count in r0
    VEC2D_0_XFRM_3X3,     // (a2) = (a1)*[(a0)], count in r0.
    VEC2D_1_XFRM_3X3,     // (a2) = (a1)*[(a0)], count in r0.
    VEC2D_TO_VEC3F,       // (a1) = (a0), fp0, count in r0
    VEC3D_SPLAT,        // (a0) = fp0
    VEC3D_COPY,         // (a1) = (a0)
    VEC3D_SCALE_AS,     // (a0) *= fp0
    VEC3D_SCALE,        // (a1) = (a0) * fp0
    VEC3D_ADD_AS,       // (a1) += (a0)
    VEC3D_ADD,          // (a2) = (a1) + (a0)
    VEC3D_SUB_AS,       // (a1) -= (a0)
    VEC3D_SUB,          // (a2) = (a1) - (a0)
    VEC3D_DOT,          // fp0  = (a0) . (a1)
    VEC3D_CROSS_AS,     // (a1) = (a1) x (a0)
    VEC3D_CROSS,        // (a2) = (a1) x (a0)
    VEC3D_MAGN,         // fp0  = |(a0)|
    VEC3D_NORM_AS,      // (a0) = norm((a0))
    VEC3D_NORM,         // (a1) = norm((a0))
    VEC3D_LERP,         // (a2) = lerp((a1) - (a0), fp0)
    VEC3D_XFRM_3X3,     // (a2) = (a1)*[(a0)], count in r0
    VEC3D_0_XFRM_4X4,     // (a2) = (a1)*[(a0)], count in r0.
    VEC3D_1_XFRM_4X4,     // (a2) = (a1)*[(a0)], count in r0.
    VEC3D_TO_VEC4F,       // (a1) = (a0), fp0, count in r0
    VEC4D_XFORM_4X4,      // (a2) = (a1)*[(a0)], count in r0.
    M2X2D_IDENTITY,     // (a0) = identity
    M2X2D_COPY,         // (a1) = (a0)
    M2X2D_SCALE_AS,     // (a0) *= fp0
    M2X2D_SCALE,        // (a1) = (a0) * fp0
    M2X2D_ADD_AS,       // (a1) += (a0)
    M2X2D_ADD,          // (a2) = (a1) + (a0)
    M2X2D_SUB_AS,       // (a1) -= (a0)
    M2X2D_SUB,          // (a2) = (a1) - (a0)
    M2X2D_MULTIPLY_AS,  // (a1) *= (a0)
    M2X2D_MULTIPLY,     // (a2) = (a1) * (a0)
    M2X2D_TRANSPOSE,    // (a1) = Transpose(a0)
    M2X2D_DET,          // fp0  = Determinant(a0)
    M2X2D_INVERSE,      // (a1) = Inverse(a0)
    M3X3D_IDENTITY,     // (a0) = identity
    M3X3D_COPY,         // (a1) = (a0)
    M3X3D_SCALE_AS,     // (a0) *= fp0
    M3X3D_SCALE,        // (a1) = (a0) * fp0
    M3X3D_ADD_AS,       // (a1) += (a0)
    M3X3D_ADD,          // (a2) = (a1) + (a0)
    M3X3D_SUB_AS,       // (a1) -= (a0)
    M3X3D_SUB,          // (a2) = (a1) - (a0)
    M3X3D_MULTIPLY_AS,  // (a1) *= (a0)
    M3X3D_MULTIPLY,     // (a2) = (a1) * (a0)
    M3X3D_TRANSPOSE,    // (a1) = Transpose(a0)
    M3X3D_DET,          // fp0  = Determinant(a0)
    M3X3D_INVERSE,      // (a1) = Inverse(a0)
    M4X4D_IDENTITY,     // (a0) = identity
    M4X4D_COPY,         // (a1) = (a0)
    M4X4D_SCALE_AS,     // (a0) *= fp0
    M4X4D_SCALE,        // (a1) = (a0) * fp0
    M4X4D_ADD_AS,       // (a1) += (a0)
    M4X4D_ADD,          // (a2) = (a1) + (a0)
    M4X4D_SUB_AS,       // (a1) -= (a0)
    M4X4D_SUB,          // (a2) = (a1) - (a0)
    M4X4D_MULTIPLY_AS,  // (a1) *= (a0)
    M4X4D_MULTIPLY,     // (a2) = (a1) * (a0)
    M4X4D_TRANSPOSE,    // (a1) = Transpose(a0)
    M4X4D_DET,          // fp0  = Determinant(a0)
    M4X4D_INVERSE,      // (a1) = Inverse(a0)

};

/**
 * Error return values
 */
enum Result {
    ERR_ZERO_DIVIDE = 1000
};


Interpreter::Status hostVector(uint8 uFunctionID);

} // namespace

#endif
