#ifndef MC64K_MACHINE_OPCODE_HANDLERS_FAST_HPP
    #define MC64K_MACHINE_OPCODE_HANDLERS_FAST_HPP
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

#endif
