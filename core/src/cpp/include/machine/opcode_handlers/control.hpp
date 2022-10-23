#ifndef MC64K_MACHINE_OPCODE_HANDLERS_CONTROL_HPP
    #define MC64K_MACHINE_OPCODE_HANDLERS_CONTROL_HPP
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

#endif
