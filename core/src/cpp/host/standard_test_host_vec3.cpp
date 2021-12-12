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
#include "host/standard_test_host_vec3.hpp"
#include "machine/register.hpp"

using MC64K::Machine::Interpreter;

namespace MC64K {
namespace StandardTestHost {
namespace Vec3 {

/**
 * Mem::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {

    Machine::GPRegister* aoGPR = Interpreter::gpr();

    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {
        case VEC3F_SPLAT: {
            float32* pfDst = aoGPR[ABI::PTR_REG_0].pfSingle;
            float32  f     = Interpreter::fpr()[0].fSingle;
            pfDst[0] = f;
            pfDst[1] = f;
            pfDst[2] = f;
            break;
        }

        case VEC3F_COPY: {
            uint32* puDst       = aoGPR[ABI::PTR_REG_1].puLong;
            uint32 const* puSrc = aoGPR[ABI::PTR_REG_0].puLong;
            puDst[0] = puSrc[0];
            puDst[1] = puSrc[1];
            puDst[2] = puSrc[2];
            break;
        }

        case VEC3F_SCALE_AS: {
            float32* pfDst = aoGPR[ABI::PTR_REG_0].pfSingle;
            float32  f     = Interpreter::fpr()[0].fSingle;
            pfDst[0] *= f;
            pfDst[1] *= f;
            pfDst[2] *= f;
            break;
        }

        case VEC3F_SCALE: {
            float32* pfDst       = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc = aoGPR[ABI::PTR_REG_0].pfSingle;
            float32  f          = Interpreter::fpr()[0].fSingle;
            pfDst[0] = f * pfSrc[0];
            pfDst[1] = f * pfSrc[1];
            pfDst[2] = f * pfSrc[2];
            break;
        }

        case VEC3F_ADD_AS: {
            float32* pfDst       = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc = aoGPR[ABI::PTR_REG_0].pfSingle;
            pfDst[0] += pfSrc[0];
            pfDst[1] += pfSrc[1];
            pfDst[2] += pfSrc[2];
            break;
        }

        case VEC3F_ADD: {
            float32* pfDst        = aoGPR[ABI::PTR_REG_2].pfSingle;
            float32 const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pfSingle;
            pfDst[0] = pfSrc1[0] + pfSrc2[0];
            pfDst[1] = pfSrc1[1] + pfSrc2[1];
            pfDst[2] = pfSrc1[2] + pfSrc2[2];
            break;
        }

        case VEC3F_SUB_AS: {
            float32* pfDst = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc = aoGPR[ABI::PTR_REG_0].pfSingle;
            pfDst[0] -= pfSrc[0];
            pfDst[1] -= pfSrc[1];
            pfDst[2] -= pfSrc[2];
            break;
        }

        case VEC3F_SUB: {
            float32* pfDst        = aoGPR[ABI::PTR_REG_2].pfSingle;
            float32 const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pfSingle;
            pfDst[0] = pfSrc1[0] - pfSrc2[0];
            pfDst[1] = pfSrc1[1] - pfSrc2[1];
            pfDst[2] = pfSrc1[2] - pfSrc2[2];
            break;
        }

        case VEC3F_DOT: {
            float32 const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pfSingle;
            Interpreter::fpr()[0].fSingle = pfSrc1[0] * pfSrc2[0] + pfSrc1[1] * pfSrc2[1] + pfSrc1[2] * pfSrc2[2];
            break;
        }

        case VEC3F_CROSS_AS: {
            // x =  v1.y * v2.z - v1.z * v2.y,
            // y =  v1.z * v2.x - v1.x * v2.z,
            // z =  v1.x * v2.y - v1.y * v2.x
            float32* pfDst       = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc = aoGPR[ABI::PTR_REG_0].pfSingle;
            float32 x = pfDst[1] * pfSrc[2] - pfDst[2] * pfSrc[1];
            float32 y = pfDst[2] * pfSrc[0] - pfDst[0] * pfSrc[2];
            float32 z = pfDst[0] * pfSrc[1] - pfDst[1] * pfSrc[0];
            pfDst[0] = x;
            pfDst[1] = y;
            pfDst[2] = z;
            break;
        }

        case VEC3F_CROSS: {
            // x =  v1.y * v2.z - v1.z * v2.y,
            // y =  v1.z * v2.x - v1.x * v2.z,
            // z =  v1.x * v2.y - v1.y * v2.x
            float32*       pfDst  = aoGPR[ABI::PTR_REG_2].pfSingle;
            float32 const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pfSingle;
            pfDst[0] = pfSrc1[1] * pfSrc2[2] - pfSrc1[2] * pfSrc2[1];
            pfDst[1] = pfSrc1[2] * pfSrc2[0] - pfSrc1[0] * pfSrc2[2];
            pfDst[2] = pfSrc1[0] * pfSrc2[1] - pfSrc1[1] * pfSrc2[0];
            break;
        }

        case VEC3F_MAGN: {
            float32 const* pfSrc = aoGPR[ABI::PTR_REG_0].pfSingle;
            Interpreter::fpr()[0].fSingle = std::sqrt(pfSrc[0]*pfSrc[0] + pfSrc[1]*pfSrc[1] + pfSrc[2]*pfSrc[2]);
            break;
        }

        case VEC3F_NORM_AS: {
            float32* pfDst   = aoGPR[ABI::PTR_REG_0].pfSingle;
            float32  fInvMag = 1.0f / std::sqrt(pfDst[0]*pfDst[0] + pfDst[1]*pfDst[1] + pfDst[2]*pfDst[2]);
            pfDst[0] *= fInvMag;
            pfDst[1] *= fInvMag;
            pfDst[2] *= fInvMag;
            break;
        }

        case VEC3F_NORM: {
            float32* pfDst       = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc = aoGPR[ABI::PTR_REG_0].pfSingle;
            float32  fInvMag = 1.0f / std::sqrt(pfSrc[0]*pfSrc[0] + pfSrc[1]*pfSrc[1] + pfSrc[2]*pfSrc[2]);
            pfDst[0] = fInvMag * pfDst[0];
            pfDst[1] = fInvMag * pfDst[1];
            pfDst[2] = fInvMag * pfDst[2];
            break;
        }

        case VEC3F_LERP: {
            float32*       pfDst  = aoGPR[ABI::PTR_REG_2].pfSingle;
            float32 const* pfSrc1 = aoGPR[ABI::PTR_REG_1].pfSingle;
            float32 const* pfSrc2 = aoGPR[ABI::PTR_REG_0].pfSingle;
            float32 fLerp         = Interpreter::fpr()[0].fSingle;
            pfDst[0] = pfSrc2[0] + (fLerp * (pfSrc1[0] - pfSrc2[0]));
            pfDst[1] = pfSrc2[1] + (fLerp * (pfSrc1[1] - pfSrc2[1]));
            pfDst[2] = pfSrc2[2] + (fLerp * (pfSrc1[2] - pfSrc2[2]));
            break;
        }

        case VEC3F_XFORM: {
            break;
        }

        default:
            std::fprintf(stderr, "Unknown Mem operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

}}} // namespace

