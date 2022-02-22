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

#include <cstdio>
#include <cmath>
#include "machine/register.hpp"
#include "host/standard_test_host_vector_math.hpp"
#include "host/vector/vec2.hpp"
#include "host/vector/vec3.hpp"
#include "host/vector/mat2x2.hpp"
#include "host/vector/mat3x3.hpp"
#include "host/vector/mat4x4.hpp"

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::VectorMath {

/**
 * VectorMath::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {

    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {

        // Single Precision

        // 2D vectors
        case VEC2F_SPLAT:           v2_splat<float32>();             break;
        case VEC2F_COPY:            v2_copy<uint32>();               break;
        case VEC2F_SCALE_AS:        v2_scale_assign<float32>();      break;
        case VEC2F_SCALE:           v2_scale<float32>();             break;
        case VEC2F_ADD_AS:          v2_add_assign<float32>();        break;
        case VEC2F_ADD:             v2_add<float32>();               break;
        case VEC2F_SUB_AS:          v2_sub_assign<float32>();        break;
        case VEC2F_SUB:             v2_sub<float32>();               break;
        case VEC2F_DOT:             v2_dot<float32>();               break;
        case VEC2F_MAGN:            v2_magnitude<float32>();         break;
        case VEC2F_NORM_AS:         v2_normalise_assign<float32>();  break;
        case VEC2F_NORM:            v2_normalise<float32>();         break;
        case VEC2F_LERP:            v2_interpolate<float32>();       break;
        case VEC2F_XFRM_2X2:        v2_transform_2x2<float32>();     break;
        case VEC2F_0_XFRM_3X3:      v2_0_transform_3x3<float32>();   break;
        case VEC2F_1_XFRM_3X3:      v2_1_transform_3x3<float32>();   break;
        case VEC2F_TO_VEC3F:        v2_expand_v3<float32>();         break;

        // 3D vectors
        case VEC3F_SPLAT:           v3_splat<float32>();             break;
        case VEC3F_COPY:            v3_copy<uint32>();               break;
        case VEC3F_SCALE_AS:        v3_scale_assign<float32>();      break;
        case VEC3F_SCALE:           v3_scale<float32>();             break;
        case VEC3F_ADD_AS:          v3_add_assign<float32>();        break;
        case VEC3F_ADD:             v3_add<float32>();               break;
        case VEC3F_SUB_AS:          v3_sub_assign<float32>();        break;
        case VEC3F_SUB:             v3_sub<float32>();               break;
        case VEC3F_DOT:             v3_dot<float32>();               break;
        case VEC3F_CROSS_AS:        v3_cross_assign<float32>();      break;
        case VEC3F_CROSS:           v3_cross<float32>();             break;
        case VEC3F_MAGN:            v3_magnitude<float32>();         break;
        case VEC3F_NORM_AS:         v3_normalise_assign<float32>();  break;
        case VEC3F_NORM:            v3_normalise<float32>();         break;
        case VEC3F_LERP:            v3_interpolate<float32>();       break;
        case VEC3F_XFRM_3X3:        v3_transform_3x3<float32>();     break;
        case VEC3F_0_XFRM_4X4:      v3_0_transform_4x4<float32>();   break;
        case VEC3F_1_XFRM_4X4:      v3_1_transform_4x4<float32>();   break;
        case VEC3F_TO_VEC4F:        v3_expand_v4<float32>();         break;
        case VEC4F_XFORM_4X4:       v4_transform_4x4<float32>();     break;

        // 2x2 Matrices
        case M2X2F_IDENTITY:        m2x2_identity<float32>();        break;
        case M2X2F_COPY:            m2x2_copy<uint32>();             break;
        case M2X2F_SCALE_AS:        m2x2_scale_assign<float32>();    break;
        case M2X2F_SCALE:           m2x2_scale<float32>();           break;
        case M2X2F_ADD_AS:          m2x2_add_assign<float32>();      break;
        case M2X2F_ADD:             m2x2_add<float32>();             break;
        case M2X2F_SUB_AS:          m2x2_sub_assign<float32>();      break;
        case M2X2F_SUB:             m2x2_sub<float32>();             break;
        case M2X2F_MULTIPLY_AS:     m2x2_multiply_assign<float32>(); break;
        case M2X2F_MULTIPLY:        m2x2_multiply<float32>();        break;
        case M2X2F_TRANSPOSE:       m2x2_transpose<uint32>();        break;
        case M2X2F_DET:             m2x2_determinant<float32>();     break;
        case M2X2F_INVERSE:         m2x2_inverse<float32>();         break;

        // 3x3 Matrices
        case M3X3F_COPY:            m3x3_copy<uint32>();             break;
        case M3X3F_SCALE_AS:        m3x3_scale_assign<float32>();    break;
        case M3X3F_SCALE:           m3x3_scale<float32>();           break;
        case M3X3F_ADD_AS:          m3x3_add_assign<float32>();      break;
        case M3X3F_ADD:             m3x3_add<float32>();             break;
        case M3X3F_SUB_AS:          m3x3_sub_assign<float32>();      break;
        case M3X3F_SUB:             m3x3_sub<float32>();             break;
        case M3X3F_MULTIPLY_AS:     m3x3_multiply_assign<float32>(); break;
        case M3X3F_MULTIPLY:        m3x3_multiply<float32>();        break;
        case M3X3F_TRANSPOSE:       m3x3_transpose<uint32>();        break;
        case M3X3F_DET:             m3x3_determinant<float32>();     break;
        case M3X3F_INVERSE:         m3x3_inverse<float32>();         break;
        case M3X3F_IDENTITY:        m3x3_identity<float32>();        break;

        // 4x4 Matrices
        case M4X4F_IDENTITY:        m4x4_identity<float32>();        break;
        case M4X4F_COPY:            m4x4_copy<uint32>();             break;
        case M4X4F_SCALE_AS:        m4x4_scale_assign<float32>();    break;
        case M4X4F_SCALE:           m4x4_scale<float32>();           break;
        case M4X4F_ADD_AS:          m4x4_add_assign<float32>();      break;
        case M4X4F_ADD:             m4x4_add<float32>();             break;
        case M4X4F_SUB_AS:          m4x4_sub_assign<float32>();      break;
        case M4X4F_SUB:             m4x4_sub<float32>();             break;
        case M4X4F_MULTIPLY_AS:     m4x4_multiply_assign<float32>(); break;
        case M4X4F_MULTIPLY:        m4x4_multiply<float32>();        break;
        case M4X4F_TRANSPOSE:       m4x4_transpose<uint32>();        break;
        case M4X4F_DET:             m4x4_determinant<float32>();     break;
        case M4X4F_INVERSE:         m4x4_inverse<float32>();         break;

        // Double Precision

        // 2D vectors
        case VEC2D_SPLAT:           v2_splat<float64>();             break;
        case VEC2D_COPY:            v2_copy<uint64>();               break;
        case VEC2D_SCALE_AS:        v2_scale_assign<float64>();      break;
        case VEC2D_SCALE:           v2_scale<float64>();             break;
        case VEC2D_ADD_AS:          v2_add_assign<float64>();        break;
        case VEC2D_ADD:             v2_add<float64>();               break;
        case VEC2D_SUB_AS:          v2_sub_assign<float64>();        break;
        case VEC2D_SUB:             v2_sub<float64>();               break;
        case VEC2D_DOT:             v2_dot<float64>();               break;
        case VEC2D_MAGN:            v2_magnitude<float64>();         break;
        case VEC2D_NORM_AS:         v2_normalise_assign<float64>();  break;
        case VEC2D_NORM:            v2_normalise<float64>();         break;
        case VEC2D_LERP:            v2_interpolate<float64>();       break;
        case VEC2D_XFRM_2X2:        v2_transform_2x2<float64>();     break;
        case VEC2D_0_XFRM_3X3:      v2_0_transform_3x3<float64>();   break;
        case VEC2D_1_XFRM_3X3:      v2_1_transform_3x3<float64>();   break;
        case VEC2D_TO_VEC3F:        v2_expand_v3<float64>();         break;

        // 3D vectors
        case VEC3D_SPLAT:           v3_splat<float64>();             break;
        case VEC3D_COPY:            v3_copy<uint64>();               break;
        case VEC3D_SCALE_AS:        v3_scale_assign<float64>();      break;
        case VEC3D_SCALE:           v3_scale<float64>();             break;
        case VEC3D_ADD_AS:          v3_add_assign<float64>();        break;
        case VEC3D_ADD:             v3_add<float64>();               break;
        case VEC3D_SUB_AS:          v3_sub_assign<float64>();        break;
        case VEC3D_SUB:             v3_sub<float64>();               break;
        case VEC3D_DOT:             v3_dot<float64>();               break;
        case VEC3D_CROSS_AS:        v3_cross_assign<float64>();      break;
        case VEC3D_CROSS:           v3_cross<float64>();             break;
        case VEC3D_MAGN:            v3_magnitude<float64>();         break;
        case VEC3D_NORM_AS:         v3_normalise_assign<float64>();  break;
        case VEC3D_NORM:            v3_normalise<float64>();         break;
        case VEC3D_LERP:            v3_interpolate<float64>();       break;

        case VEC3D_XFRM_3X3:        v3_transform_3x3<float64>();     break;
        case VEC3D_0_XFRM_4X4:      v3_0_transform_4x4<float64>();   break;
        case VEC3D_1_XFRM_4X4:      v3_1_transform_4x4<float64>();   break;
        case VEC3D_TO_VEC4F:        v3_expand_v4<float64>();         break;
        case VEC4D_XFORM_4X4:       v4_transform_4x4<float64>();     break;

        // 2x2 Matrices
        case M2X2D_IDENTITY:        m2x2_identity<float64>();        break;
        case M2X2D_COPY:            m2x2_copy<uint64>();             break;
        case M2X2D_SCALE_AS:        m2x2_scale_assign<float64>();    break;
        case M2X2D_SCALE:           m2x2_scale<float64>();           break;
        case M2X2D_ADD_AS:          m2x2_add_assign<float64>();      break;
        case M2X2D_ADD:             m2x2_add<float64>();             break;
        case M2X2D_SUB_AS:          m2x2_sub_assign<float64>();      break;
        case M2X2D_SUB:             m2x2_sub<float64>();             break;
        case M2X2D_MULTIPLY_AS:     m2x2_multiply_assign<float64>(); break;
        case M2X2D_MULTIPLY:        m2x2_multiply<float64>();        break;
        case M2X2D_TRANSPOSE:       m2x2_transpose<uint64>();        break;
        case M2X2D_DET:             m2x2_determinant<float64>();     break;
        case M2X2D_INVERSE:         m2x2_inverse<float64>();         break;

        // 3x3 Matrices
        case M3X3D_IDENTITY:        m3x3_identity<float64>();        break;
        case M3X3D_COPY:            m3x3_copy<uint64>();             break;
        case M3X3D_SCALE_AS:        m3x3_scale_assign<float64>();    break;
        case M3X3D_SCALE:           m3x3_scale<float64>();           break;
        case M3X3D_ADD_AS:          m3x3_add_assign<float64>();      break;
        case M3X3D_ADD:             m3x3_add<float64>();             break;
        case M3X3D_SUB_AS:          m3x3_sub_assign<float64>();      break;
        case M3X3D_SUB:             m3x3_sub<float64>();             break;
        case M3X3D_MULTIPLY_AS:     m3x3_multiply_assign<float64>(); break;
        case M3X3D_MULTIPLY:        m3x3_multiply<float64>();        break;
        case M3X3D_TRANSPOSE:       m3x3_transpose<uint64>();        break;
        case M3X3D_DET:             m3x3_determinant<float64>();     break;
        case M3X3D_INVERSE:         m3x3_inverse<float64>();         break;

        // 4x4 Matrices
        case M4X4D_IDENTITY:        m4x4_identity<float64>();        break;
        case M4X4D_COPY:            m4x4_copy<uint64>();             break;
        case M4X4D_SCALE_AS:        m4x4_scale_assign<float64>();    break;
        case M4X4D_SCALE:           m4x4_scale<float64>();           break;
        case M4X4D_ADD_AS:          m4x4_add_assign<float64>();      break;
        case M4X4D_ADD:             m4x4_add<float64>();             break;
        case M4X4D_SUB_AS:          m4x4_sub_assign<float64>();      break;
        case M4X4D_SUB:             m4x4_sub<float64>();             break;
        case M4X4D_MULTIPLY_AS:     m4x4_multiply_assign<float64>(); break;
        case M4X4D_MULTIPLY:        m4x4_multiply<float64>();        break;
        case M4X4D_TRANSPOSE:       m4x4_transpose<uint64>();        break;
        case M4X4D_DET:             m4x4_determinant<float64>();     break;
        case M4X4D_INVERSE:         m4x4_inverse<float64>();         break;

        default:
            std::fprintf(stderr, "Unknown operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

} // namespace

