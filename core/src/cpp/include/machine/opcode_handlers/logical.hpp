#ifndef MC64K_MACHINE_OPCODE_HANDLERS_LOGIC_HPP
    #define MC64K_MACHINE_OPCODE_HANDLERS_LOGIC_HPP
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

#endif
