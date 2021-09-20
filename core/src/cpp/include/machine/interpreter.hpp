#ifndef __MC64K_MACHINE_INTERPETER_HPP__
#   define __MC64K_MACHINE_INTERPETER_HPP__

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
#include "register.hpp"
#include "error.hpp"

namespace MC64K {

/**
 * Forwards reference
 */
namespace Loader {
    class Executable;
}

namespace Machine {

/**
 * Interpreter
 *
 * Static, single threaded interpreter model.
 */
class Interpreter {

    public:
        /**
         * Status
         */
        typedef enum {
            UNINITIALISED = 0,
            INITIALISED,
            RUNNING,
            COMPLETED,
            CAUGHT_FIRE,
            UNIMPLEMENTED_OPCODE,
            UNIMPLEMENTED_EAMODE,
            UNKNOWN_HOST_CALL,
        } Status;

        /**
         * Debug dump options
         */
        enum DumpFlags {
            STATE_GPR   = 1,
            STATE_FPR   = 2,
            STATE_TMP   = 4,
            STATE_STACK = 8
        };

        enum Limits {
            MAX_HCF_VECTOR = 256
        };

        /**
         * HCF Vector (host native call triggered by HCF operation)
         */
        typedef Status (*HCFVector)();

        /**
         * Initialise the HCF vectors. Only a reference is taken so the supplied table must not go out of scope.
         *
         * @param const HCFVector*    pcHCFVectors
         * @param const unsigned int  uNumHCFVectors
         */
        static void initHCFVectors(const HCFVector* pcHCFVectors, const uint32 uNumHCFVectors);

        /**
         * Allocate the machine stack. The top of the stack will be assigned to r15 as the USP.
         *
         * @param  uint32 uStackSize
         * @throws
         */
        static void allocateStack(uint32 uStackSize);

        /**
         * Release the stack allocation
         */
        static void freeStack();

        /**
         * Specify the bytecode location to begin execution from
         */
        static void setProgramCounter(const uint8* puNewProgramCounter);

        /**
         * Run!
         */
        static void run();

        /**
         * Get a GRP register
         *
         * @param  const unsigned int uReg
         * @return GPRegister&
         */
        static GPRegister& gpr(const unsigned int uReg);

        /**
         * Get a FPR register
         *
         * @param  const unsigned int uReg
         * @return FPRegister&
         */
        static FPRegister& fpr(const unsigned int uReg);

        /**
         * Dump the machine state
         *
         * @param std::FILE* poStream
         * @param const unsigned int uFlags
         */
        static void dumpState(std::FILE* poStream, const unsigned int uFlags);

    private:
        static GPRegister       aoGPR[GPRegister::MAX];
        static FPRegister       aoFPR[FPRegister::MAX];
        static const uint8*     puProgramCounter;
        static void*            pDstEA;
        static void*            pSrcEA;
        static void*            pTmpEA;
        static uint8*           puStackTop;
        static uint8*           puStackBase;
        static const HCFVector* pcHCFVectors;
        static int32            iCallDepth;
        static uint32           uNumHCFVectors;

        /**
         * Operation size
         */
        static enum OperationSize {
            SIZE_BYTE = 1,
            SIZE_WORD = 2,
            SIZE_LONG = 4,
            SIZE_QUAD = 8
        } eOperationSize;

        /**
         * Machine status
         */
        static Status eStatus;

        /**
         * Decode the effective address currently under evaluation
         *
         * @return void*
         */
        static void* decodeEffectiveAddress();

        /**
         * Save the registers implied by the 32-bit mask, using the specified EA mode
         *
         * @param uint32 uMask
         * @param uint8  uEAMode
         */
        static void  saveRegisters(uint32 uMask, uint8 uEAMode);

        /**
         * Restore the registers implies by the 32-bit mask, using the specified EA mode
         *
         * @param uint32 uMask
         * @param uint8  uEAMode
         */
        static void  restoreRegisters(uint32 uMask, uint8 uEAMode);
};

}} // namespace
#endif
