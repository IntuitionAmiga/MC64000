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
 * Forwards references
 */
namespace Loader {
    class Executable;
    class Symbol;
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
            INVALID_ENTRYPOINT
        } Status;

        /**
         * Debug dump options
         */
        enum DumpFlags {
            STATE_GPR   =  1,
            STATE_FPR   =  2,
            STATE_TMP   =  4,
            STATE_STACK =  8,
            STATE_HCF   = 16
        };

        /**
         * HCF Vector (host native call triggered by HCF operation)
         */
        typedef Status (*HCFVector)();

        /**
         * Initialise the HCF vectors. Only a reference is taken so the supplied table must not go out of scope.
         *
         * @param HCFVector const* pcHCFVectors
         * @param uint32 const     uNumHCFVectors
         */
        static void initHCFVectors(HCFVector const* pcHCFVectors, uint32 const uNumHCFVectors);

        /**
         * Initialise the imported symbols. Only a reference is taken so the supplied table must not go out of scope.
         *
         * @param Loader::Symbol* poImportSymbols
         * @param uint32 const    uNumImportSymbols
         */
        static void initImportSymbols(Loader::Symbol* poImportSymbols, uint32 const uNumImportSymbols);

        /**
         * Allocate the machine stack. The top of the stack will be assigned to r15 as the USP.
         *
         * @param  uint32 const uStackSize
         * @throws
         */
        static void allocateStack(uint32 const uStackSize);

        /**
         * Release the stack allocation
         */
        static void freeStack();

        /**
         * Specify the bytecode location to begin execution from
         */
        static void setProgramCounter(uint8 const* puNewProgramCounter);

        /**
         * Run!
         */
        static void run();

        /**
         * Get a GRP register (range checked)
         *
         * @param  unsigned int const uReg
         * @return GPRegister&
         */
        static GPRegister& gpr(unsigned int const uReg);

        /**
         * Get a FPR register (range checked)
         *
         * @param  unsigned int const uReg
         * @return FPRegister&
         */
        static FPRegister& fpr(unsigned int const uReg);

        /**
         * Get the GP register set (array access)
         */
        static GPRegister* gpr();

        /**
         * Get the FP register set (array access)
         */
        static FPRegister* fpr();

        /**
         * Dump the machine state
         *
         * @param std::FILE* poStream
         * @param unsigned const int uFlags
         */
        static void dumpState(std::FILE* poStream, unsigned int const uFlags);

        /**
         * Return the current interpreter status
         *
         * @return Status
         */
        static Status getStatus();

    private:
        static GPRegister       aoGPR[GPRegister::MAX];
        static FPRegister       aoFPR[FPRegister::MAX];
        static uint8 const*     puProgramCounter;
        static void*            pDstEA;
        static void*            pSrcEA;
        static void*            pTmpEA;
        static uint8*           puStackTop;
        static uint8*           puStackBase;
        static HCFVector const* pcHCFVectors;
        static Loader::Symbol*  poImportSymbols;
        static int32            iCallDepth;
        static uint32           uNumHCFVectors;
        static uint32           uNumImportSymbols;

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
         * @param uint32 const uMask
         * @param uint8  const uEAMode
         */
        static void  saveRegisters(uint32 const uMask, uint8 const uEAMode);

        /**
         * Restore the registers implies by the 32-bit mask, using the specified EA mode
         *
         * @param uint32 const uMask
         * @param uint8  const uEAMode
         */
        static void  restoreRegisters(uint32 const uMask, uint8 const uEAMode);
};

/**
 * @inheritDoc
 */
inline Interpreter::Status Interpreter::getStatus() {
    return eStatus;
}

/**
 * @inheritDoc
 */
inline GPRegister* Interpreter::gpr() {
    return aoGPR;
}

/**
 * @inheritDoc
 */
inline FPRegister* Interpreter::fpr() {
    return aoFPR;
}

/**
 * @inheritDoc
 */
inline GPRegister& Interpreter::gpr(unsigned int const uReg) {
    return aoGPR[uReg & GPRegister::MASK];
}

/**
 * @inheritDoc
 */
inline FPRegister& Interpreter::fpr(unsigned int const uReg) {
    return aoFPR[uReg & FPRegister::MASK];
}

}} // namespace
#endif
