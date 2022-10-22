#ifndef MC64K_MACHINE_OPCODE_HANDLERS_HPP
    #define MC64K_MACHINE_OPCODE_HANDLERS_HPP
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
 * Declares the actual handlers for the main opcodes.
 *
 * This file is intended to be by a source and requires the following macros are defined:
 *
 * defOp(NAME) - This defines the handler. This could generate a case statement, label,
 *               function definition, depending on how the interpreter build is configured.
 *               The parameter is expected to match the Opcode:: enumerated operation names.
 *
 * end()       - This macro defines code that exits from the handler with the explicit
 *               requrement to halt further execution.
 *
 * status()    - This macro defines code that exits from the handler with the explicit
 *               requirement to check the status register before continuing, e.g. that the
 *               handler could have set an error condition.
 *
 * next()      - This macro defines code that exits from the handler with the indication that
 *               the handler does not modify the the status register and so the interpreter
 *               may choose to skip the check.
 *
 */

/**
 * Stop execution
 */
defOp(STOP) {
    eStatus = CAUGHT_FIRE;
    end();
}

/**
 * Call host vetor
 */
defOp(HOST) {
    handleHost();
    status();
}

/**
 * Unconditional branch, byte displacement.
 */
defOp(BRA_B) {
    branchByte();
    next();
}

/**
 * Unconditional branch, long displacement.
 */
defOp(BRA) {
    branchLong();
    next();
}

/**
 * Branch to subroutine, short displacement.
 */
defOp(BSR_B) {
    int8 iShortDisplacement = (int8)*puProgramCounter++;
    pushProgramCounter();
    puProgramCounter += iShortDisplacement;
    ++iCallDepth;
    next();
}

/**
 * Branch to subroutine, long displacement.
 */
defOp(BSR) {
    readDisplacement();
    pushProgramCounter();
    puProgramCounter += iDisplacement;
    ++iCallDepth;
    next();
}

/**
 * Jump to location.
 */
defOp(JMP) {
    monadic(SIZE_QUAD);
    puProgramCounter = (uint8 const*)pDstEA;
    status();
}

/**
 * Jump to subroutine.
 */
defOp(JSR) {
    monadic(SIZE_QUAD);
    pushProgramCounter();
    puProgramCounter = (uint8 const*)pDstEA;
    ++iCallDepth;
    status();
}

/**
 * Return from subroutine.
 */
defOp(RTS) {
    if (0 == --iCallDepth) {
        eStatus = COMPLETED;
        end();
    } else {
        popProgramCounter();
        next();
    }
}

/**
 * Compare operand to zero and branch on condition.
 */
defOp(BMC) {
    handleBMC();
    status();
}

/**
 * Compare operands and branch on condition.
 */
defOp(BDC) {
    handleBDC();
    status();
}

/**
 * Decrement operand and branch if not zero (unsigned 32-bit)
 */
defOp(DBNZ) {
    monadic(SIZE_WORD);
    readDisplacement();
    bcc(--asULong(pDstEA));
    status();
}

/**
 * Compare operand to zero and branch on condition.
 * Register optimised version.
 */
defOp(R_BMC) {
    handleRBMC();
    next();
}

/**
 * Compare operands and branch on condition.
 * Register optimised version.
 */
defOp(R2R_BDC) {
    handleR2RBDC();
    next();
}

/**
 * Decrement register and branch if not zero (unsigned 32-bit)
 */
defOp(R_DBNZ) {
    readRegPair();
    readDisplacement();
    bcc(--dstGPRULong());
    next();
}

/**
 * Move long.
 * Register to Register optimised version.
 */
defOp(R2R_MOVE_L) {
    readRegPair();
    dstGPRULong() = srcGPRULong();
    next();
}

/**
 * Move quad.
 * Register to Register optimised version.
 */
defOp(R2R_MOVE_Q) {
    readRegPair();
    dstGPRUQuad() = srcGPRUQuad();
    next();
}

/**
 * Move single.
 * Register to Register optimised version.
 */
defOp(R2R_FMOVE_S) {
    readRegPair();
    dstFPRULong() = srcFPRULong();
    next();
}

/**
 * Move double.
 * Register to Register optimised version.
 */
defOp(R2R_FMOVE_D) {
    readRegPair();
    dstFPRUQuad() = srcFPRUQuad();
    next();
}

/**
 * Clear long.
 * Register optimised version.
 */
defOp(R2R_CLR_L) {
    readRegPair();
    dstGPRULong() = 0;
    next();
}

/**
 * Clear quad.
 * Register optimised version.
 */
defOp(R2R_CLR_Q) {
    readRegPair();
    dstGPRUQuad() = 0;
    next();
}

/**
 * Exchange integer register pair.
 */
defOp(R2R_EXG) {
    readRegPair();
    uint64 uTemp  = srcGPRUQuad();
    srcGPRUQuad() = dstGPRUQuad();
    dstGPRUQuad() = uTemp;
    next();
}

/**
 * Exchange float register pair.
 */
defOp(R2R_FEXG) {
    readRegPair();
    uint64 uTemp  = srcFPRUQuad();
    srcFPRUQuad() = dstFPRUQuad();
    dstFPRUQuad() = uTemp;
    next();
}

/**
 * 32-bit halfword swap
 */
defOp(R2R_SWAP) {
    readRegPair();
    uint32 uTemp  = srcGPRULong();
    dstGPRULong() = uTemp >> 16 | uTemp << 16;
    next();
}

/**
 * 32-bit byteswap
 */
defOp(R2R_SWAP_L) {
    readRegPair();
    dstGPRULong() = __builtin_bswap32(srcGPRULong());
    next();
}

/**
 * 64-bit byteswap
 */
defOp(R2R_SWAP_Q) {
    readRegPair();
    dstGPRUQuad() = __builtin_bswap64(srcGPRUQuad());
    next();
}

/**
 * Bitwise AND long.
 * Register to Register optimised version.
 */
defOp(R2R_AND_L) {
    readRegPair();
    dstGPRULong() &= srcGPRULong();
    next();
}

/**
 * Bitwise AND quad.
 * Register to Register optimised version.
 */
defOp(R2R_AND_Q) {
    readRegPair();
    dstGPRUQuad() &= srcGPRUQuad();
    next();
}

/**
 * Bitwise OR long.
 * Register to Register optimised version.
 */
defOp(R2R_OR_L) {
    readRegPair();
    dstGPRULong() |= srcGPRULong();
    next();
}

/**
 * Bitwise AND quad.
 * Register to Register optimised version.
 */
defOp(R2R_OR_Q) {
    readRegPair();
    dstGPRUQuad() |= srcGPRUQuad();
    next();
}

/**
 * Bitwise Exclusive OR long.
 * Register to Register optimised version.
 */
defOp(R2R_EOR_L) {
    readRegPair();
    dstGPRULong() ^= srcGPRULong();
    next();
}

/**
 * Bitwise Exclusive OR quad.
 * Register to Register optimised version.
 */
defOp(R2R_EOR_Q) {
    readRegPair();
    dstGPRUQuad() ^= srcGPRUQuad();
    next();
}

/**
 * Bitwise NOT long.
 * Register to Register optimised version.
 */
defOp(R2R_NOT_L) {
    readRegPair();
    dstGPRULong() = ~srcGPRULong();
    next();
}

/**
 * Bitwise NOT quad.
 * Register to Register optimised version.
 */
defOp(R2R_NOT_Q) {
    readRegPair();
    dstGPRUQuad() = ~srcGPRUQuad();
    next();
}

/**
 * Logical Shift Left long.
 * Register to Register optimised version.
 */
defOp(R2R_LSL_L) {
    readRegPair();
    dstGPRULong() <<= (srcGPRULong() & 31);
    next();
}

/**
 * Logical Shift Left quad.
 * Register to Register optimised version.
 */
defOp(R2R_LSL_Q) {
    readRegPair();
    dstGPRUQuad() <<= (srcGPRUQuad() & 63);
    next();
}

/**
 * Logical Shift Right long.
 * Register to Register optimised version.
 */
defOp(R2R_LSR_L) {
    readRegPair();
    dstGPRULong() >>= (srcGPRULong() & 31);
    next();
}

/**
 * Logical Shift Right quad.
 * Register to Register optimised version.
 */
defOp(R2R_LSR_Q) {
    readRegPair();
    dstGPRUQuad() >>= (srcGPRUQuad() & 63);
    next();
}

/**
 * Bit Field Find First One (whole register)
 */
defOp(BFFFO) {
    readRegPair();
    uint64 pTemp  = srcGPRUQuad();
    dstGPRUQuad() = pTemp ? (63 - __builtin_clzll(pTemp)) : ~pTemp;
    status();
}

/**
 * Bit Field Count (whole register)
 */
defOp(BFCNT) {
    readRegPair();
    dstGPRUQuad() = __builtin_popcountll(srcGPRUQuad());
    status();
}

/**
 * Sign extend byte to long.
 * Register to Register optimised version.
 */
defOp(R2R_EXTB_L) {
    readRegPair();
    dstGPRLong() = (int32)srcGPRByte();
    next();
}

/**
 * Sign extend byte to quad.
 * Register to Register optimised version.
 */
defOp(R2R_EXTB_Q) {
    readRegPair();
    dstGPRQuad() = (int64)srcGPRByte();
    next();
}

/**
 * Sign extend word to long.
 * Register to Register optimised version.
 */
defOp(R2R_EXTW_L) {
    readRegPair();
    dstGPRLong() = (int32)srcGPRWord();
    next();
}

/**
 * Sign extend word to quad.
 * Register to Register optimised version.
 */
defOp(R2R_EXTW_Q) {
    readRegPair();
    dstGPRQuad() = (int64)srcGPRWord();
    next();
}

/**
 * Sign extend long to quad.
 * Register to Register optimised version.
 */
defOp(R2R_EXTL_Q) {
    readRegPair();
    dstGPRQuad() = (int64)srcGPRLong();
    next();
}

/**
 * Negate long.
 * Register to Register optimised version.
 */
defOp(R2R_NEG_L) {
    readRegPair();
    dstGPRLong() = -srcGPRLong();
    next();
}

/**
 * Negate quad.
 * Register to Register optimised version.
 */
defOp(R2R_NEG_Q) {
    readRegPair();
    dstGPRQuad() = -srcGPRQuad();
    next();
}

/**
 * Negate single.
 * Register to Register optimised version.
 */
defOp(R2R_FNEG_S) {
    readRegPair();
    dstFPRSingle() = -srcFPRSingle();
    next();
}

/**
 * Negate double.
 * Register to Register optimised version.
 */
defOp(R2R_FNEG_D) {
    readRegPair();
    dstFPRDouble() = -srcFPRDouble();
    next();
}

/**
 * Absolute single.
 * Register to Register optimised version.
 */
defOp(R2R_FABS_S) {
    readRegPair();
    dstFPRSingle() = std::fabs(srcFPRSingle());
    next();
}

/**
 * Absolute double.
 * Register to Register optimised version.
 */
defOp(R2R_FABS_D) {
    readRegPair();
    dstFPRDouble() = std::fabs(srcFPRDouble());
    next();
}

/**
 * Add long.
 * Register to Register optimised version.
 */
defOp(R2R_ADD_L) {
    readRegPair();
    dstGPRLong() += srcGPRLong();
    next();
}

/**
 * Add quad.
 * Register to Register optimised version.
 */
defOp(R2R_ADD_Q) {
    readRegPair();
    dstGPRQuad() += srcGPRQuad();
    next();
}

/**
 * Add single.
 * Register to Register optimised version.
 */
defOp(R2R_FADD_S) {
    readRegPair();
    dstFPRSingle() += srcFPRSingle();
    next();
}

/**
 * Add double.
 * Register to Register optimised version.
 */
defOp(R2R_FADD_D) {
    readRegPair();
    dstFPRDouble() += srcFPRDouble();
    next();
}

/**
 * Subtract long.
 * Register to Register optimised version.
 */
defOp(R2R_SUB_L) {
    readRegPair();
    dstGPRLong() -= srcGPRLong();
    next();
}

/**
 * Subtract quad.
 * Register to Register optimised version.
 */
defOp(R2R_SUB_Q) {
    readRegPair();
    dstGPRQuad() -= srcGPRQuad();
    next();
}

/**
 * Subtract single.
 * Register to Register optimised version.
 */
defOp(R2R_FSUB_S) {
    readRegPair();
    dstFPRSingle() -= srcFPRSingle();
    next();
}

/**
 * Subtract double.
 * Register to Register optimised version.
 */
defOp(R2R_FSUB_D) {
    readRegPair();
    dstFPRDouble() -= srcFPRDouble();
    next();
}

/**
 * Multiply signed long.
 * Register to Register optimised version.
 */
defOp(R2R_MULS_L) {
    readRegPair();
    dstGPRLong() *= srcGPRLong();
    next();
}

/**
 * Multiply signed quad.
 * Register to Register optimised version.
 */
defOp(R2R_MULS_Q) {
    readRegPair();
    dstGPRQuad() *= srcGPRQuad();
    next();
}

/**
 * Multiply unsigned long.
 * Register to Register optimised version.
 */
defOp(R2R_MULU_L) {
    readRegPair();
    dstGPRULong() *= srcGPRULong();
    next();
}

/**
 * Multiply unsigned quad.
 * Register to Register optimised version.
 */
defOp(R2R_MULU_Q) {
    readRegPair();
    dstGPRUQuad() *= srcGPRUQuad();
    next();
}

/**
 * Multiply single.
 * Register to Register optimised version.
 */
defOp(R2R_FMUL_S) {
    readRegPair();
    dstFPRSingle() *= srcFPRSingle();
    next();
}

/**
 * Multiply double.
 * Register to Register optimised version.
 */
defOp(R2R_FMUL_D) {
    readRegPair();
    dstFPRDouble() *= srcFPRDouble();
    next();
}

/**
 * Divide single.
 * Register to Register optimised version.
 */
defOp(R2R_FDIV_S) {
    readRegPair();
    dstFPRSingle() /= srcFPRSingle();
    next();
}

/**
 * Divide double.
 * Register to Register optimised version.
 */
defOp(R2R_FDIV_D) {
    readRegPair();
    dstFPRDouble() /= srcFPRDouble();
    next();
}

/**
 * Modulus single.
 * Register to Register optimised version.
 */
defOp(R2R_FMOD_S) {
    readRegPair();
    dstFPRSingle() = std::fmod(dstFPRSingle(), srcFPRSingle());
    next();
}

/**
 * Modulus double.
 * Register to Register optimised version.
 */
defOp(R2R_FMOD_D) {
    readRegPair();
    dstFPRDouble() = std::fmod(dstFPRDouble(), srcFPRDouble());
    next();
}

/**
 * Square Root single.
 * Register to Register optimised version.
 */
defOp(R2R_FSQRT_S) {
    readRegPair();
    dstFPRSingle() = std::sqrt(srcFPRSingle());
    next();
}

/**
 * Square Root double.
 * Register to Register optimised version.
 */
defOp(R2R_FSQRT_D) {
    readRegPair();
    dstFPRDouble() = std::sqrt(srcFPRDouble());
    next();
}

/**
 * Multiply accumulate single.
 * Register to Register optimised version.
 */
defOp(R2R_FMACC_S) {
    // dst += src1 * src2
    uint16 uRegs = *(uint16 const*)puProgramCounter;
    puProgramCounter += sizeof(uint16);
    aoFPR[uRegs & 0x000F].fSingle +=
        aoFPR[(uRegs >> 4) & 0x000F].fSingle *
        aoFPR[(uRegs >> 8) & 0x000F].fSingle;
    next();
}

/**
 * Multiply accumulate double.
 * Register to Register optimised version.
 */
defOp(R2R_FMACC_D) {
    // dst += src1 * src2
    uint16 uRegs = *(uint16 const*)puProgramCounter;
    puProgramCounter += sizeof(uint16);
    aoFPR[uRegs & 0x000F].fDouble +=
        aoFPR[(uRegs >> 4) & 0x000F].fDouble *
        aoFPR[(uRegs >> 8) & 0x000F].fDouble;
    next();
}

/**
 * Multiply add single.
 * Register to Register optimised version.
 */
defOp(R2R_FMADD_S) {
    // dst = src1 * src2 + src3
    uint16 uRegs = *(uint16 const*)puProgramCounter;
    puProgramCounter += sizeof(uint16);
    aoFPR[uRegs & 0x000F].fSingle =
        (aoFPR[(uRegs >> 4) & 0x000F].fSingle *
        aoFPR[(uRegs >> 8) & 0x000F].fSingle) +
        aoFPR[(uRegs >> 12) & 0x000F].fSingle;
    next();
}

/**
 * Multiply add double.
 * Register to Register optimised version.
 */
defOp(R2R_FMADD_D) {
    // dst = src1 * src2 + src3
    uint16 uRegs = *(uint16 const*)puProgramCounter;
    puProgramCounter += sizeof(uint16);
    aoFPR[uRegs & 0x000F].fDouble =
        (aoFPR[(uRegs >> 4) & 0x000F].fDouble *
        aoFPR[(uRegs >> 8) & 0x000F].fDouble) +
        aoFPR[(uRegs >> 12) & 0x000F].fDouble;
    next();
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Move Integer.
 */
defOp(MOVE_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) = asUByte(pSrcEA);
    status();
}

defOp(MOVE_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) = asUWord(pSrcEA);
    status();
}

defOp(MOVE_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) = asULong(pSrcEA);
    status();
}

defOp(MOVE_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) = asUQuad(pSrcEA);
    status();
}

/**
 * Save Multiple registers.
 */
defOp(SAVEM) {
    uint8 uEAMode = *puProgramCounter++;
    readMask();
    saveRegisters(uMask, uEAMode);
    status();
}

/**
 * Load Multiple Registers.
 */
defOp(LOADM) {
    readMask();
    uint8 uEAMode = *puProgramCounter++;
    restoreRegisters(uMask, uEAMode);
    status();
}

/**
 * Integer to float conversions.
 */
defOp(FMOVEB_S) {
    dyadic2(SIZE_LONG, SIZE_BYTE);
    asSingle(pDstEA) = (float32)asByte(pSrcEA);
    status();
}

defOp(FMOVEB_D) {
    dyadic2(SIZE_QUAD, SIZE_BYTE);
    asDouble(pDstEA) = (float64)asByte(pSrcEA);
    status();
}

defOp(FMOVEW_S) {
    dyadic2(SIZE_LONG, SIZE_WORD);
    asSingle(pDstEA) = (float32)asWord(pSrcEA);
    status();
}

defOp(FMOVEW_D) {
    dyadic2(SIZE_QUAD, SIZE_WORD);
    asDouble(pDstEA) = (float64)asWord(pSrcEA);
    status();
}

defOp(FMOVEL_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = (float32)asLong(pSrcEA);
    status();
}

defOp(FMOVEL_D) {
    dyadic2(SIZE_QUAD, SIZE_LONG);
    asDouble(pDstEA) = (float64)asLong(pSrcEA);
    status();
}

defOp(FMOVEQ_S) {
    dyadic2(SIZE_LONG, SIZE_QUAD);
    asSingle(pDstEA) = (float32)asQuad(pSrcEA);
    status();
}

defOp(FMOVEQ_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = (float64)asQuad(pSrcEA);
    status();
}

/**
 * Float Single to Integer conversions.
 */
defOp(FMOVES_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) = (int32)asSingle(pSrcEA);
    status();
}

defOp(FMOVES_Q) {
    dyadic2(SIZE_QUAD, SIZE_LONG);
    asQuad(pDstEA) = (int64)asSingle(pSrcEA);
    status();
}

/**
 * Float Single to Double conversion.
 */
defOp(FMOVES_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = (float64)asSingle(pSrcEA);
    status();
}

/**
 * Float Double to Integer conversions.
 */
defOp(FMOVED_L) {
    dyadic2(SIZE_LONG, SIZE_QUAD);
    asLong(pDstEA) = (int32)asDouble(pSrcEA);
    status();
}

defOp(FMOVED_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) = (int64)asDouble(pSrcEA);
    status();
}

/**
 * Float Double to Single conversion.
 */
defOp(FMOVED_S) {
    dyadic2(SIZE_LONG, SIZE_QUAD);
    asSingle(pDstEA) = (float32)asDouble(pSrcEA);
    status();
}

/**
 * Move Float.
 */
defOp(FMOVE_S) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) = asULong(pSrcEA);
    status();
}

defOp(FMOVE_D) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) = asQuad(pSrcEA);
    status();
}

/**
 * Classify Float.
 */
defOp(FINFO_S) {
    dyadic2(SIZE_BYTE, SIZE_LONG);
    asUByte(pDstEA) = classifyFloat(asSingle(pSrcEA));
    status();
}

defOp(FINFO_D) {
    dyadic2(SIZE_BYTE, SIZE_QUAD);
    asUByte(pDstEA) = classifyFloat(asDouble(pSrcEA));
    status();
}

/**
 * Integer clear.
 */
defOp(CLR_B) {
    monadic(SIZE_BYTE);
    asUByte(pDstEA) = 0;
    status();
}

defOp(CLR_W) {
    monadic(SIZE_WORD);
    asUWord(pDstEA) = 0;
    status();
}

defOp(CLR_L) {
    monadic(SIZE_LONG);
    asULong(pDstEA) = 0;
    status();
}

defOp(CLR_Q) {
    monadic(SIZE_QUAD);
    asUQuad(pDstEA) = 0;
    status();
}

/**
 * Link stack frame.
 */
defOp(LINK) {
    // link r<N>, #d
    readDisplacement();
    readRegPair();
    // sp - 8 -> sp
    aoGPR[GPRegister::SP].puByte -= sizeof(uint64);
    // r<N> -> (sp)
    *(aoGPR[GPRegister::SP].puQuad) = dstGPRUQuad();
    // sp -> r<N>
    dstGPRUQuad() = aoGPR[GPRegister::SP].uQuad;
    // sp + d -> sp
    aoGPR[GPRegister::SP].puByte += iDisplacement;
    next();
}

/**
 * Unlink stack frame.
 */
defOp(UNLK) {
    readRegPair();
    // r<N> -> sp
    aoGPR[GPRegister::SP].uQuad = dstGPRUQuad();
    // (sp) -> r<N>
    dstGPRUQuad() = *(aoGPR[GPRegister::SP].puQuad);
    // sp + 8 -> sp
    aoGPR[GPRegister::SP].puByte += sizeof(uint64);
    next();
}

/**
 * Load Effective Address.
 */
defOp(LEA) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) = (uint64)pSrcEA;
    status();
}

/**
 * Push Effective Address.
 */
defOp(PEA) {
    monadic(SIZE_QUAD);
    aoGPR[GPRegister::SP].puByte -= sizeof(uint64);
    *(aoGPR[GPRegister::SP].puQuad) = (uint64)pSrcEA;
    status();
}

/**
 * Compare operand to zero and set destination on condition.
 */
defOp(SCM) {
    handleSMC();
    status();
}

/**
 * Compare source operands and set destination on condition.
 */
defOp(SCD) {
    handleSDC();
    status();
}

/**
 * Bitwise AND
 */
defOp(AND_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) &= asUByte(pSrcEA);
    status();
}

defOp(AND_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) &= asUWord(pSrcEA);
    status();
}

defOp(AND_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) &= asULong(pSrcEA);
    status();
}

defOp(AND_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) &= asUQuad(pSrcEA);
    status();
}

/**
 * Bitwise OR
 */
defOp(OR_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) |= asUByte(pSrcEA);
    status();
}

defOp(OR_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) |= asUWord(pSrcEA);
    status();
}

defOp(OR_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) |= asULong(pSrcEA);
    status();
}

defOp(OR_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) |= asUQuad(pSrcEA);
    status();
}

/**
 * Bitwise Exclusive OR
 */
defOp(EOR_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) ^= asUByte(pSrcEA);
    status();
}

defOp(EOR_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) ^= asUWord(pSrcEA);
    status();
}

defOp(EOR_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) ^= asULong(pSrcEA);
    status();
}

defOp(EOR_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) ^= asUQuad(pSrcEA);
    status();
}

/**
 * Bitwise NOT
 */
defOp(NOT_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) = ~asUByte(pSrcEA);
    status();
}

defOp(NOT_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) = ~asUWord(pSrcEA);
    status();
}

defOp(NOT_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) = ~asULong(pSrcEA);
    status();
}

defOp(NOT_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) = ~asUQuad(pSrcEA);
    status();
}

/**
 * Logical Shift Left
 */
defOp(LSL_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) <<= (asUByte(pSrcEA) & 7);
    status();
}

defOp(LSL_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) <<= (asUByte(pSrcEA) & 15);
    status();
}

defOp(LSL_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) <<= (asUByte(pSrcEA) & 31);
    status();
}

defOp(LSL_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) <<= (asUByte(pSrcEA) & 63);
    status();
}

/**
 * Logical Shift Right
 */
defOp(LSR_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) >>= (asUByte(pSrcEA) & 7);
    status();
}

defOp(LSR_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) >>= (asUByte(pSrcEA) & 15);
    status();
}

defOp(LSR_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) >>= (asUByte(pSrcEA) & 31);
    status();
}

defOp(LSR_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) >>= (asUByte(pSrcEA) & 63);
    status();
}

/**
 * Logical Rotate Left
 */
defOp(ROL_B) {
    dyadic(SIZE_BYTE);
    rolByte((uint8*)pDstEA, asUByte(pSrcEA));
    status();
}
defOp(ROL_W) {
    dyadic(SIZE_WORD);
    rolWord((uint16*)pDstEA, asUByte(pSrcEA));
    status();
}

defOp(ROL_L) {
    dyadic(SIZE_LONG);
    rolLong((uint32*)pDstEA, asUByte(pSrcEA));
    status();
}

defOp(ROL_Q) {
    dyadic(SIZE_QUAD);
    rolQuad((uint64*)pDstEA, asUByte(pSrcEA));
    status();
}

/**
 * Logical Rotate Right
 */
defOp(ROR_B) {
    dyadic(SIZE_BYTE);
    rorByte((uint8*)pDstEA, asUByte(pSrcEA));
    status();
}

defOp(ROR_W) {
    dyadic(SIZE_WORD);
    rorWord((uint16*)pDstEA, asUByte(pSrcEA));
    status();
}

defOp(ROR_L) {
    dyadic(SIZE_LONG);
    rorLong((uint32*)pDstEA, asUByte(pSrcEA));
    status();
}

defOp(ROR_Q) {
    dyadic(SIZE_QUAD);
    rorQuad((uint64*)pDstEA, asUByte(pSrcEA));
    status();
}

/**
 * Clear bit position
 */
defOp(BCLR_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) &= (uint8) ~(1 << (asUByte(pSrcEA) & 7));
    status();
}

defOp(BCLR_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) &= (uint16) ~(1 << (asUByte(pSrcEA) & 15));
    status();
}

defOp(BCLR_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) &= (uint32) ~(1 << (asUByte(pSrcEA) & 31));
    status();
}

defOp(BCLR_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) &= (uint64) ~(1 << (asUByte(pSrcEA) & 63));
    status();
}

/**
 * Set bit position
 */
defOp(BSET_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) |= (uint8) (1 << (asUByte(pSrcEA) & 7));
    status();
}

defOp(BSET_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) |= (uint16) (1 << (asUByte(pSrcEA) & 15));
    status();
}

defOp(BSET_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) |= (uint32) (1 << (asUByte(pSrcEA) & 31));
    status();
}

defOp(BSET_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) |= (uint64) (1 << (asUByte(pSrcEA) & 63));
    status();
}

/**
 * Sign Extend
 */
defOp(EXTB_W) {
    dyadic(SIZE_WORD);
    asWord(pDstEA) = (int16)asByte(pSrcEA);
    status();
}

defOp(EXTB_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) = (int32)asByte(pSrcEA);
    status();
}

defOp(EXTB_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) = (int64)asByte(pSrcEA);
    status();
}

defOp(EXTW_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) = (int32)asWord(pSrcEA);
    status();
}

defOp(EXTW_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) = (int64)asWord(pSrcEA);
    status();
}

defOp(EXTL_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) = (int64)asLong(pSrcEA);
    status();
}

/**
 * Arithmetic Shift Left
 */
defOp(ASL_B) {
    dyadic(SIZE_BYTE);
    asByte(pDstEA) <<= (asUByte(pSrcEA) & 7);
    status();
}

defOp(ASL_W) {
    dyadic(SIZE_WORD);
    asWord(pDstEA) <<= (asUByte(pSrcEA) & 15);
    status();
}

defOp(ASL_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) <<= (asUByte(pSrcEA) & 31);
    status();
}

defOp(ASL_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) <<= (asUByte(pSrcEA) & 63);
    status();
}

/**
 * Arithmetic Shift Right
 */
defOp(ASR_B) {
    dyadic(SIZE_BYTE);
    asByte(pDstEA) >>= (asUByte(pSrcEA) & 7);
    status();
}

defOp(ASR_W) {
    dyadic(SIZE_WORD);
    asWord(pDstEA) >>= (asUByte(pSrcEA) & 15);
    status();
}

defOp(ASR_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) >>= (asUByte(pSrcEA) & 31);
    status();
}

defOp(ASR_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) >>= (asUByte(pSrcEA) & 63);
    status();
}

/**
 * Integer Addition
 */
defOp(ADD_B) {
    dyadic(SIZE_BYTE);
    asByte(pDstEA) += asByte(pSrcEA);
    status();
}
defOp(ADD_W) {
    dyadic(SIZE_WORD);
    asWord(pDstEA) += asWord(pSrcEA);
    status();
}

defOp(ADD_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) += asLong(pSrcEA);
    status();
}

defOp(ADD_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) += asQuad(pSrcEA);
    status();
}

/**
 * Float Addition
 */
defOp(FADD_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) += asSingle(pSrcEA);
    status();
}

defOp(FADD_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) += asDouble(pSrcEA);
    status();
}

/**
 * Integer Subtraction
 */
defOp(SUB_B) {
    dyadic(SIZE_BYTE);
    asByte(pDstEA) -= asByte(pSrcEA);
    status();
}

defOp(SUB_W) {
    dyadic(SIZE_WORD);
    asWord(pDstEA) -= asWord(pSrcEA);
    status();
}

defOp(SUB_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) -= asLong(pSrcEA);
    status();
}

defOp(SUB_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) -= asQuad(pSrcEA);
    status();
}

/**
 * Float Subtraction
 */
defOp(FSUB_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) -= asSingle(pSrcEA);
    status();
}

defOp(FSUB_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) -= asDouble(pSrcEA);
    status();
}

/**
 * Integer Negate
 */
defOp(NEG_B) {
    dyadic(SIZE_BYTE);
    asByte(pDstEA) = -asByte(pSrcEA);
    status();
}

defOp(NEG_W) {
    dyadic(SIZE_WORD);
    asWord(pDstEA) = -asWord(pSrcEA);
    status();
}

defOp(NEG_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) = -asLong(pSrcEA);
    status();
}

defOp(NEG_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) = -asQuad(pSrcEA);
    status();
}

/**
 * Float Negate
 */
defOp(FNEG_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = -asSingle(pSrcEA);
    status();
}

defOp(FNEG_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = -asDouble(pSrcEA);
    status();
}

/**
 * Integer Multiplication Signed
 */
defOp(MULS_B) {
    dyadic(SIZE_BYTE);
    asByte(pDstEA) *= asByte(pSrcEA);
    status();
}

defOp(MULS_W) {
    dyadic(SIZE_WORD);
    asWord(pDstEA) *= asWord(pSrcEA);
    status();
}

defOp(MULS_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) *= asLong(pSrcEA);
    status();
}

defOp(MULS_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) *= asQuad(pSrcEA);
    status();
}

/**
 * Integer Multiplication Unsigned
 */
defOp(MULU_B) {
    dyadic(SIZE_BYTE);
    asUByte(pDstEA) *= asUByte(pSrcEA);
    status();
}

defOp(MULU_W) {
    dyadic(SIZE_WORD);
    asUWord(pDstEA) *= asUWord(pSrcEA);
    status();
}

defOp(MULU_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) *= asULong(pSrcEA);
    status();
}

defOp(MULU_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) *= asUQuad(pSrcEA);
    status();
}

/**
 * Float Multiplication
 */
defOp(FMUL_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) *= asSingle(pSrcEA);
    status();
}

defOp(FMUL_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) *= asDouble(pSrcEA);
    status();
}

/**
 * Integer Division Signed
 */
defOp(DIVS_L) {
    dyadic(SIZE_BYTE);
    asLong(pDstEA) /= asLong(pSrcEA);
    status();
}

defOp(DIVS_Q) {
    dyadic(SIZE_WORD);
    asQuad(pDstEA) /= asQuad(pSrcEA);
    status();
}

/**
 * Integer Moduluo Signed
 */
defOp(MODS_L) {
    dyadic(SIZE_LONG);
    asLong(pDstEA) %= asLong(pSrcEA);
    status();
}

defOp(MODS_Q) {
    dyadic(SIZE_QUAD);
    asQuad(pDstEA) %= asQuad(pSrcEA);
    status();
}

/**
 * Integer Division Unsigned
 */
defOp(DIVU_L) {
    dyadic(SIZE_BYTE);
    asULong(pDstEA) /= asULong(pSrcEA);
    status();
}

defOp(DIVU_Q) {
    dyadic(SIZE_WORD);
    asUQuad(pDstEA) /= asUQuad(pSrcEA);
    status();
}

/**
 * Integer Moduluo Unsigned
 */
defOp(MODU_L) {
    dyadic(SIZE_LONG);
    asULong(pDstEA) %= asULong(pSrcEA);
    status();
}

defOp(MODU_Q) {
    dyadic(SIZE_QUAD);
    asUQuad(pDstEA) %= asUQuad(pSrcEA);
    status();
}

/**
 * Float Division
 */
defOp(FDIV_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) /= asSingle(pSrcEA);
    status();
}

defOp(FDIV_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) /= asDouble(pSrcEA);
    status();
}

/**
 * Float Moduluo
 */
defOp(FMOD_S) {
    dyadic(SIZE_LONG);
    float32 f = asSingle(pDstEA);
    asSingle(pDstEA) = std::fmod(f, asSingle(pSrcEA));
    status();
}

defOp(FMOD_D) {
    dyadic(SIZE_QUAD);
    float64 f = asDouble(pDstEA);
    asDouble(pDstEA) = std::fmod(f, asDouble(pSrcEA));
    status();
}

/**
 * Float Absolute
 */
defOp(FABS_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::fabs(asSingle(pSrcEA));
    status();
}

defOp(FABS_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::fabs(asDouble(pSrcEA));
    status();
}

/**
 * Float Square Root
 */
defOp(FSQRT_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::sqrt(asSingle(pSrcEA));
    status();
}

defOp(FSQRT_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::sqrt(asDouble(pSrcEA));
    status();
}

/**
 * Float Arc Cosine
 */
defOp(FACOS_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::acos(asSingle(pSrcEA));
    status();
}

defOp(FACOS_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::acos(asDouble(pSrcEA));
    status();
}

/**
 * Float Arc Sine
 */
defOp(FASIN_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::asin(asSingle(pSrcEA));
    status();
}

defOp(FASIN_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::asin(asDouble(pSrcEA));
    status();
}

/**
 * Float Arc Tangent
 */
defOp(FATAN_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::atan(asSingle(pSrcEA));
    status();
}

defOp(FATAN_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::atan(asDouble(pSrcEA));
    status();
}

/**
 * Float Cosine
 */
defOp(FCOS_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::cos(asSingle(pSrcEA));
    status();
}

defOp(FCOS_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::cos(asDouble(pSrcEA));
    status();
}

/**
 * Float Sine
 */
defOp(FSIN_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::sin(asSingle(pSrcEA));
    status();
}

defOp(FSIN_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::sin(asDouble(pSrcEA));
    status();
}

/**
 * Float Sine with Cosine
 */
defOp(FSINCOS_S) {
    dyadic(SIZE_LONG);
    float32 f = asSingle(pSrcEA);
    asSingle(pDstEA) = std::sin(f);
    asSingle(pSrcEA) = std::cos(f);
    status();
}

defOp(FSINCOS_D) {
    dyadic(SIZE_QUAD);
    float64 f = asDouble(pSrcEA);
    asDouble(pDstEA) = std::sin(f);
    asDouble(pSrcEA) = std::cos(f);
    status();
}

/**
 * Float Tangent
 */
defOp(FTAN_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::tan(asSingle(pSrcEA));
    status();
}

defOp(FTAN_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::tan(asDouble(pSrcEA));
    status();
}

/**
 * Float Exponential
 */
defOp(FETOX_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::exp(asSingle(pSrcEA));
    status();
}

defOp(FETOX_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::exp(asDouble(pSrcEA));
    status();
}

/**
 * Float Log Natural
 */
defOp(FLOGN_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::log(asSingle(pSrcEA));
    status();
}

defOp(FLOGN_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::log(asDouble(pSrcEA));
    status();
}

/**
 * Float Log Base 2
 */
defOp(FLOG2_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::log2(asSingle(pSrcEA));
    status();
}

defOp(FLOG2_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::log2(asDouble(pSrcEA));
    status();
}

/**
 * Float Two to Power X
 */
defOp(FTWOTOX_S) {
    dyadic(SIZE_LONG);
    asSingle(pDstEA) = std::exp2(asSingle(pSrcEA));
    status();
}

defOp(FTWOTOX_D) {
    dyadic(SIZE_QUAD);
    asDouble(pDstEA) = std::exp2(asDouble(pSrcEA));
    status();
}

#endif
