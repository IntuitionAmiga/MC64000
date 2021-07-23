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
 * Collection of gnarly macros. Bite me.
 */
#define initDisplacement() \
    union { \
        uint32 uMask; \
        int32  iDisplacement; \
        uint8  uBytes[4]; \
    };

#define readDisplacement() \
    uBytes[0] = *pProgramCounter++; \
    uBytes[1] = *pProgramCounter++; \
    uBytes[2] = *pProgramCounter++; \
    uBytes[3] = *pProgramCounter++;

#define readMask() readDisplacement()


#define branchByte()  { int8 iShortDisplacement = (int8)*pProgramCounter++; pProgramCounter += iShortDisplacement; }
#define branchLong()  { readDisplacement(); pProgramCounter += iDisplacement; }
#define bcc(c) if (c) { pProgramCounter += iDisplacement; }

#define monadic(size) \
    eOperationSize = size; \
    pDstEA = decodeEffectiveAddress();

#define dyadic(size) \
    eOperationSize = size; \
    pDstEA = decodeEffectiveAddress(); \
    pSrcEA = decodeEffectiveAddress();

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
#define asBitPos(ea, m) (1 << (asUByte(ea) & m))
#define todo() eStatus = UNIMPLEMENTED_OPCODE; break;

#define pushProgramCounter() \
    aGPR[GPRegister::SP].pUByte -= 8; \
    *(aGPR[GPRegister::SP].pUQuad) = (uint64)pProgramCounter;

#define popProgramCounter() \
    pProgramCounter = (const uint8*)(*(aGPR[GPRegister::SP].pUQuad)); \
    aGPR[GPRegister::SP].pUByte += 8;
