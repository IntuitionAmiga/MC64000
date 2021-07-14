#ifndef __MC64K_MACHINE_HPP__
#  define __MC64K_MACHINE_HPP__

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

/**
 * MC64K::Machine
 */
namespace MC64K {
    namespace Machine {

        #include "machine/register.hpp"

        class StaticInterpreter {

        private:

            static Register::GeneralPurpose aGPR[Register::GeneralPurpose::MAX];
            static Register::FloatingPoint  aFPR[Register::FloatingPoint::MAX];
            static const uint8* pProgramCounter;
            static void* pDstEA;
            static void* pSrcEA;
            static void* pTmpEA;

            static enum  OperationSize {
                SIZE_BYTE = 1,
                SIZE_WORD = 2,
                SIZE_LONG = 4,
                SIZE_QUAD = 8
            } eOperationSize;

        public:
            enum StateFlags {
                STATE_GPR = 1,
                STATE_FPR = 2,
                STATE_TMP = 4
            };

            static void dumpState(const int iFlags);

            static Register::GeneralPurpose& gpr(const unsigned int uReg) {
                return aGPR[uReg & Register::GeneralPurpose::MASK];
            }

            static Register::FloatingPoint& fpr(const unsigned int uReg) {
                return aFPR[uReg & Register::FloatingPoint::MASK];
            }

            static void setProgramCounter(const uint8* pNewProgramCounter) {
                pProgramCounter = pNewProgramCounter;
            }

            static void run();

            static void* decodeEffectiveAddress();
        private:

        };

    };
}
#endif
