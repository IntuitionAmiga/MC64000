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
#include "host/standard_test_host_vector_math.hpp"
#include "host/vector/macros.hpp"
#include "machine/register.hpp"

using MC64K::Machine::Interpreter;

namespace MC64K {
namespace StandardTestHost {
namespace VectorMath {

// Explicit template instantiations

template void vec2_transform_2x2(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
template void vec2_0_transform_3x3(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
template void vec2_1_transform_3x3(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
template void vec2_expand_vec3(float32* pfDst, float32 const* pfSrc, float32 fValue, size_t uCount);
template void vec3_transform_3x3(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
template void vec3_0_transform_4x4(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
template void vec3_1_transform_4x4(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);
template void vec3_expand_vec4(float32* pfDst, float32 const* pfSrc, float32 fValue, size_t uCount);
template void vec4_transform_4x4(float32* pfDst, float32 const* pfSrc, float32 const* pfM, size_t uCount);

/**
 * Mem::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {

    Machine::GPRegister* aoGPR = Interpreter::gpr();

    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {

        // 2D vectors
        case VEC2F_SPLAT:           v2_splat(float32, Single);                  break;
        case VEC2F_COPY:            v2_copy(uint32, uLong);                     break;
        case VEC2F_SCALE_AS:        v2_scale_assign(float32, Single);           break;
        case VEC2F_SCALE:           v2_scale(float32, Single);                  break;
        case VEC2F_ADD_AS:          v2_add_assign(float32, Single);             break;
        case VEC2F_ADD:             v2_add(float32, Single);                    break;
        case VEC2F_SUB_AS:          v2_sub_assign(float32, Single);             break;
        case VEC2F_SUB:             v2_sub(float32, Single);                    break;
        case VEC2F_DOT:             v2_dot(float32, Single);                    break;
        case VEC2F_MAGN:            v2_magnitude(float32, Single);              break;
        case VEC2F_NORM_AS:         v2_normalise_assign(float32, Single, 1.0f); break;
        case VEC2F_NORM:            v2_normalise(float32, Single, 1.0f);        break;
        case VEC2F_LERP:            v2_interpolate(float32, Single);            break;
        case VEC2F_XFRM_2X2:        v2_transform_2x2(float32, Single);          break;
        case VEC2F_0_XFRM_3X3:      v2_0_transform_3x3(float32, Single);        break;
        case VEC2F_1_XFRM_3X3:      v2_1_transform_3x3(float32, Single);        break;
        case VEC2F_TO_VEC3F:        v2_expand_v3(float32, Single);              break;

        // 3D vectors
        case VEC3F_SPLAT:           v3_splat(float32, Single);                  break;
        case VEC3F_COPY:            v3_copy(uint32, uLong);                     break;
        case VEC3F_SCALE_AS:        v3_scale_assign(float32, Single);           break;
        case VEC3F_SCALE:           v3_scale(float32, Single);                  break;
        case VEC3F_ADD_AS:          v3_add_assign(float32, Single);             break;
        case VEC3F_ADD:             v3_add(float32, Single);                    break;
        case VEC3F_SUB_AS:          v3_sub_assign(float32, Single);             break;
        case VEC3F_SUB:             v3_sub(float32, Single);                    break;
        case VEC3F_DOT:             v3_dot(float32, Single);                    break;
        case VEC3F_CROSS_AS:        v3_cross_assign(float32, Single);           break;
        case VEC3F_CROSS:           v3_cross(float32, Single);                  break;
        case VEC3F_MAGN:            v3_magnitude(float32, Single);              break;
        case VEC3F_NORM_AS:         v3_normalise_assign(float32, Single, 1.0f); break;
        case VEC3F_NORM:            v3_normalise(float32, Single, 1.0f);        break;
        case VEC3F_LERP:            v3_interpolate(float32, Single);            break;
/*
        case VEC3F_XFRM_3X3:        v3_transform_3x3(float32, Single);          break;
        case VEC3F_0_XFRM_4X4:      v3_0_transform_4x4(float32, Single);        break;
        case VEC3F_1_XFRM_4X4:      v3_1_transform_4x4(float32, Single);        break;
        case VEC3F_TO_VEC4F:        v3_expand_v4(float32, Single);              break;
        case VEC4F_XFORM_4X4:       v4_transform_4x4(float32, Single);          break;

        case M2X2F_IDENTITY:        m2x2_identity(float32, Single);             break;
        case M2X2F_COPY:            m2x2_copy(uint32, uLong);                   break;
        case M2X2F_SCALE_AS:        m2x2_scale_assign(float32, Single);         break;
        case M2X2F_SCALE:           m2x2_scale(float32, Single);                break;
        case M2X2F_ADD_AS:          m2x2_add_assign(float32, Single);           break;
        case M2X2F_ADD:             m2x2_add(float32, Single);                  break;
        case M2X2F_SUB_AS:          m2x2_sub_assign(float32, Single);           break;
        case M2X2F_SUB:             m2x2_sub(float32, Single);                  break;
        case M2X2F_MULTIPLY_AS:     m2x2_multiply_assign(float32, Single);      break;
        case M2X2F_MULTIPLY:        m2x2_multiply(float32, Single);             break;
        case M2X2F_TRANSPOSE_AS:
        case M2X2F_TRANSPOSE:
        case M2X2F_INVERSE_AS:
        case M2X2F_INVERSE:
        case M2X2F_DET:
            break;

        case M3X3F_IDENTITY:        m3x3_identity(float32, Single);             break;
        case M3X3F_COPY:            m3x3_copy(uint32, uLong);                   break;
        case M3X3F_SCALE_AS:        m3x3_scale_assign(float32, Single);         break;
        case M3X3F_SCALE:           m3x3_scale(float32, Single);                break;
        case M3X3F_ADD_AS:          m3x3_add_assign(float32, Single);           break;
        case M3X3F_ADD:             m3x3_add(float32, Single);                  break;
        case M3X3F_SUB_AS:          m3x3_sub_assign(float32, Single);           break;
        case M3X3F_SUB:             m3x3_sub(float32, Single);                  break;
        case M3X3F_MULTIPLY_AS:     m3x3_multiply_assign(float32, Single);      break;
        case M3X3F_MULTIPLY:        m3x3_multiply(float32, Single);             break;
        case M3X3F_TRANSPOSE_AS:
        case M3X3F_TRANSPOSE:
        case M3X3F_INVERSE_AS:
        case M3X3F_INVERSE:
        case M3X3F_DET:
            break;

        case M4X4F_IDENTITY:        m4x4_identity(float32, Single);             break;
        case M4X4F_COPY:            m4x4_copy(uint32, uLong);                   break;
        case M4X4F_SCALE_AS:        m4x4_scale_assign(float32, Single);         break;
        case M4X4F_SCALE:           m4x4_scale(float32, Single);                break;
        case M4X4F_ADD_AS:          m4x4_add_assign(float32, Single);           break;
        case M4X4F_ADD:             m4x4_add(float32, Single);                  break;
        case M4X4F_SUB_AS:          m4x4_sub_assign(float32, Single);           break;
        case M4X4F_SUB:             m4x4_sub(float32, Single);                  break;
        case M4X4F_MULTIPLY_AS:     m4x4_multiply_assign(float32, Single);      break;
        case M4X4F_MULTIPLY:        m4x4_multiply(float32, Single);             break;
        case M4X4F_TRANSPOSE_AS:
        case M4X4F_TRANSPOSE:
        case M4X4F_INVERSE_AS:
        case M4X4F_INVERSE:
        case M4X4F_DET:
            break;
*/
        default:
            std::fprintf(stderr, "Unknown Mem operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

}}} // namespace

