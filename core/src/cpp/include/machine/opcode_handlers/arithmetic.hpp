#ifndef MC64K_MACHINE_OPCODE_HANDLERS_ARITHMETIC_HPP
    #define MC64K_MACHINE_OPCODE_HANDLERS_ARITHMETIC_HPP
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
