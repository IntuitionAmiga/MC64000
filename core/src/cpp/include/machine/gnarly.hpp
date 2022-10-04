#ifndef MC64K_MACHINE_GNARLY_HPP
    #define MC64K_MACHINE_GNARLY_HPP

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
 * Collection of horrible, gnarly macros. Bite me.
 */

#define NOINLINE __attribute__((noinline))

/**
 * Sets up a union of temporaries involved in handling displacement values in the opcode stream.
 */
#define initDisplacement() \
    union { \
        uint32 uMask; \
        uint32 uIndex; \
        int32  iDisplacement; \
        uint8  auBytes[4]; \
    };

/**
 * Reads a 4 byte displacement value from the opcode stream into the initialised temporary.
 */
#ifdef ALLOW_MISALIGNED_IMMEDIATE
#define readDisplacement() \
    iDisplacement = *((int32*)puProgramCounter); puProgramCounter += 4;
#else
#define readDisplacement() \
    auBytes[0] = *puProgramCounter++; \
    auBytes[1] = *puProgramCounter++; \
    auBytes[2] = *puProgramCounter++; \
    auBytes[3] = *puProgramCounter++;
#endif

/**
 * Alias of readDisplacment() for when the value represents a mask value and not a displacement.
 */
#define readMask() readDisplacement()

#define readSymbolIndex() readDisplacement()

/**
 * Reads the next byte of the opcode stream as a short immediate displacement and updates the program counter.
 */
#define branchByte() { int8 iShortDisplacement = (int8)*puProgramCounter++; puProgramCounter += iShortDisplacement; }

/**
 * Reads the immediate 4-byte displacement from the opcode stream and updates the program counter.
 */
#define branchLong() { readDisplacement(); puProgramCounter += iDisplacement; }

/**
 * Tests the condition and if true, updates the program counter with the already loaded displacement.
 */
#define bcc(c) if ((c)) { puProgramCounter += iDisplacement; }

/**
 * Decodes a single effective address for a monadic operation, updating the destination EA address.
 */
#define monadic(size) \
    eOperationSize = (size); \
    pDstEA = decodeEffectiveAddress();

/**
 * Decodes a pair of effective addresses for a dyadic operation, updating source and destination EA addresses.
 */
#define dyadic(size) \
    eOperationSize = (size); \
    pDstEA = decodeEffectiveAddress(); \
    pSrcEA = decodeEffectiveAddress();

/**
 * Decodes a pair of effective addresses for a dyadic operation with asymmetric operand sizes, updating the source and
 * destination EA addresses.
 */
#define dyadic2(size_dst, size_src) \
    eOperationSize = (size_dst); \
    pDstEA = decodeEffectiveAddress(); \
    eOperationSize = (size_src); \
    pSrcEA = decodeEffectiveAddress();

/**
 * Type casting for access to decoded effective addresses
 */
#define asByte(ea)   *((int8*)(ea))
#define asWord(ea)   *((int16*)(ea))
#define asLong(ea)   *((int32*)(ea))
#define asQuad(ea)   *((int64*)(ea))
#define asUByte(ea)  *((uint8*)(ea))
#define asUWord(ea)  *((uint16*)(ea))
#define asULong(ea)  *((uint32*)(ea))
#define asUQuad(ea)  *((uint64*)(ea))
#define asSingle(ea) *((float32*)(ea))
#define asDouble(ea) *((float64*)(ea))
#define asBitPos(ea, m) (1 << (asUByte(ea) & (m)))

#define unpackGPR() { \
    pDstEA = &aoGPR[(*puProgramCounter++) & 0xF]; \
}

#define unpackFPR() { \
    pDstEA = &aoFPR[(*puProgramCounter++) & 0xF]; \
}


/**
 * Unpack a byte as a dest/src GPR pair and set the EA pointers directly.
 */
#define unpackGPRPair() { \
    uint8 uRegPair  = *puProgramCounter++; \
    pDstEA          = &aoGPR[uRegPair & 0xF]; \
    pSrcEA          = &aoGPR[uRegPair >> 4]; \
}

/**
 * Unpack a byte as a dest/src FPR pair and set the EA pointers directly.
 */
#define unpackFPRPair() { \
    uint8 uRegPair  = *puProgramCounter++; \
    pDstEA          = &aoFPR[uRegPair & 0xF]; \
    pSrcEA          = &aoFPR[uRegPair >> 4]; \
}

/**
 * Exit triger for unimplemented instructions.
 */
#define todo() eStatus = UNIMPLEMENTED_OPCODE; break;

/**
 * Saves the program counter onto the stack.
 */
#define pushProgramCounter() \
    aoGPR[GPRegister::SP].puByte -= 8; \
    *(aoGPR[GPRegister::SP].puQuad) = (uint64)puProgramCounter;

/**
 * Restores the program counter from the stack.
 */
#define popProgramCounter() \
    puProgramCounter = (uint8 const*)(*(aoGPR[GPRegister::SP].puQuad)); \
    aoGPR[GPRegister::SP].puByte += 8;

#ifdef REPORT_MIPS
    #define initMIPSReport() \
        std::fprintf(stderr, "Beginning run at PC:%p...\n", puProgramCounter); \
        uint64 uInstructionCount = 0; \
        Nanoseconds::Value uStart = Nanoseconds::mark();

    #define updateMIPS() ++uInstructionCount;

    #define outputMIPSReport() \
        Nanoseconds::Value uElapsed = Nanoseconds::mark() - uStart; \
        float64 fMIPS = (1000.0 * (float64)uInstructionCount) / (float64)uElapsed; \
        std::fprintf( \
            stderr, \
            "Total instructions %lu in %lu nanoseconds, %.2f MIPS\n", \
            uInstructionCount, \
            uElapsed, \
            fMIPS \
        );
#else
    #define initMIPSReport()
    #define updateMIPS()
    #define outputMIPSReport()
#endif

#endif

