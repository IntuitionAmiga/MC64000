#ifndef MC64K_MACHINE_OPCODE_HANDLERS_DATA_MOVE_HPP
    #define MC64K_MACHINE_OPCODE_HANDLERS_DATA_MOVE_HPP
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

#endif
