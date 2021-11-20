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
#include "host/standard_test_host_vector_math_macros.hpp"
#include "machine/register.hpp"

using MC64K::Machine::Interpreter;

namespace MC64K {
namespace StandardTestHost {
namespace VectorMath {

/**
 * Mem::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {

    Machine::GPRegister* aoGPR = Interpreter::gpr();

    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {
        case VEC2F_SPLAT:       v2_splat(float32, Single);                  break;
        case VEC2F_COPY:        v2_copy(uint32, uLong);                     break;
        case VEC2F_SCALE_AS:    v2_scale_assign(float32, Single);           break;
        case VEC2F_SCALE:       v2_scale(float32, Single);                  break;
        case VEC2F_ADD_AS:      v2_add_assign(float32, Single);             break;
        case VEC2F_ADD:         v2_add(float32, Single);                    break;
        case VEC2F_SUB_AS:      v2_sub_assign(float32, Single);             break;
        case VEC2F_SUB:         v2_sub(float32, Single);                    break;
        case VEC2F_DOT:         v2_dot(float32, Single);                    break;
        case VEC2F_MAGN:        v2_magnitude(float32, Single);              break;
        case VEC2F_NORM_AS:     v2_normalise_assign(float32, Single, 1.0f); break;
        case VEC2F_NORM:        v2_normalise(float32, Single, 1.0f);        break;
        case VEC2F_LERP:        v2_interpolate(float32, Single);            break;
        case VEC2F_XFORM:       break;


        case VEC3F_SPLAT:       v3_splat(float32, Single);                  break;
        case VEC3F_COPY:        v3_copy(uint32, uLong);                     break;
        case VEC3F_SCALE_AS:    v3_scale_assign(float32, Single);           break;
        case VEC3F_SCALE:       v3_scale(float32, Single);                  break;
        case VEC3F_ADD_AS:      v3_add_assign(float32, Single);             break;
        case VEC3F_ADD:         v3_add(float32, Single);                    break;
        case VEC3F_SUB_AS:      v3_sub_assign(float32, Single);             break;
        case VEC3F_SUB:         v3_sub(float32, Single);                    break;
        case VEC3F_DOT:         v3_dot(float32, Single);                    break;
        case VEC3F_CROSS_AS:    v3_cross_assign(float32, Single);           break;
        case VEC3F_CROSS:       v3_cross(float32, Single);                  break;
        case VEC3F_MAGN:        v3_magnitude(float32, Single);              break;
        case VEC3F_NORM_AS:     v3_normalise_assign(float32, Single, 1.0f); break;
        case VEC3F_NORM:        v3_normalise(float32, Single, 1.0f);        break;
        case VEC3F_LERP:        v3_interpolate(float32, Single);            break;
        case VEC3F_XFORM:       break;

        case VEC2D_SPLAT:       v2_splat(float64, Double);                  break;
        case VEC2D_COPY:        v2_copy(uint64, uQuad);                     break;
        case VEC2D_SCALE_AS:    v2_scale_assign(float64, Double);           break;
        case VEC2D_SCALE:       v2_scale(float64, Double);                  break;
        case VEC2D_ADD_AS:      v2_add_assign(float64, Double);             break;
        case VEC2D_ADD:         v2_add(float64, Double);                    break;
        case VEC2D_SUB_AS:      v2_sub_assign(float64, Double);             break;
        case VEC2D_SUB:         v2_sub(float64, Double);                    break;
        case VEC2D_DOT:         v2_dot(float64, Double);                    break;
        case VEC2D_MAGN:        v2_magnitude(float64, Double);              break;
        case VEC2D_NORM_AS:     v2_normalise_assign(float64, Double, 1.0);  break;
        case VEC2D_NORM:        v2_normalise(float64, Double, 1.0);         break;
        case VEC2D_LERP:        v2_interpolate(float64, Double);            break;
        case VEC2D_XFORM:       break;


        case VEC3D_SPLAT:       v3_splat(float64, Double);                  break;
        case VEC3D_COPY:        v3_copy(uint64, uQuad);                     break;
        case VEC3D_SCALE_AS:    v3_scale_assign(float64, Double);           break;
        case VEC3D_SCALE:       v3_scale(float64, Double);                  break;
        case VEC3D_ADD_AS:      v3_add_assign(float64, Double);             break;
        case VEC3D_ADD:         v3_add(float64, Double);                    break;
        case VEC3D_SUB_AS:      v3_sub_assign(float64, Double);             break;
        case VEC3D_SUB:         v3_sub(float64, Double);                    break;
        case VEC3D_DOT:         v3_dot(float64, Double);                    break;
        case VEC3D_CROSS_AS:    v3_cross_assign(float64, Double);           break;
        case VEC3D_CROSS:       v3_cross(float64, Double);                  break;
        case VEC3D_MAGN:        v3_magnitude(float64, Double);              break;
        case VEC3D_NORM_AS:     v3_normalise_assign(float64, Double, 1.0);  break;
        case VEC3D_NORM:        v3_normalise(float64, Double, 1.0);         break;
        case VEC3D_LERP:        v3_interpolate(float64, Double);            break;
        case VEC3D_XFORM:       break;

        default:
            std::fprintf(stderr, "Unknown Mem operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

}}} // namespace

