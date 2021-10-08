<?php

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

declare(strict_types = 1);

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction;

use ABadCafe\MC64K;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;
use ABadCafe\MC64K\Defs\Mnemonic\ILogical;
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Tokeniser;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Utils\Log;
use ABadCafe\MC64K\Defs\EffectiveAddress\IRegisterDirect;
use function \strlen, \ord, \chr;

/**
 */
class FastPathOptimiser {

    const
        FAST_INT_PREFIX   = 254,
        FAST_FLOAT_PREFIX = 255
    ;

    const FAST_PATH = [
        //IDataMove::MOVE_B   => self::FAST_INT_PREFIX,
        //IDataMove::MOVE_W   => self::FAST_INT_PREFIX,
        IDataMove::MOVE_L   => self::FAST_INT_PREFIX,
        IDataMove::MOVE_Q   => self::FAST_INT_PREFIX,

        IDataMove::FMOVE_S  => self::FAST_FLOAT_PREFIX,
        IDataMove::FMOVE_D  => self::FAST_FLOAT_PREFIX,
        IArithmetic::FADD_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FADD_D => self::FAST_FLOAT_PREFIX,
        IArithmetic::FSUB_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FSUB_D => self::FAST_FLOAT_PREFIX,
        IArithmetic::FNEG_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FNEG_D => self::FAST_FLOAT_PREFIX,
        IArithmetic::FMUL_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FMUL_D => self::FAST_FLOAT_PREFIX,
        IArithmetic::FDIV_S => self::FAST_FLOAT_PREFIX,
        IArithmetic::FDIV_D => self::FAST_FLOAT_PREFIX,
    ];

    const OPERANDS = [
        IRegisterDirect::R0_DIR   => 1,
        IRegisterDirect::R1_DIR   => 1,
        IRegisterDirect::R2_DIR   => 1,
        IRegisterDirect::R3_DIR   => 1,
        IRegisterDirect::R4_DIR   => 1,
        IRegisterDirect::R5_DIR   => 1,
        IRegisterDirect::R6_DIR   => 1,
        IRegisterDirect::R7_DIR   => 1,
        IRegisterDirect::R8_DIR   => 1,
        IRegisterDirect::R9_DIR   => 1,
        IRegisterDirect::R10_DIR  => 1,
        IRegisterDirect::R11_DIR  => 1,
        IRegisterDirect::R12_DIR  => 1,
        IRegisterDirect::R13_DIR  => 1,
        IRegisterDirect::R14_DIR  => 1,
        IRegisterDirect::R15_DIR  => 1,
        IRegisterDirect::FP0_DIR  => 1,
        IRegisterDirect::FP1_DIR  => 1,
        IRegisterDirect::FP2_DIR  => 1,
        IRegisterDirect::FP3_DIR  => 1,
        IRegisterDirect::FP4_DIR  => 1,
        IRegisterDirect::FP5_DIR  => 1,
        IRegisterDirect::FP6_DIR  => 1,
        IRegisterDirect::FP7_DIR  => 1,
        IRegisterDirect::FP8_DIR  => 1,
        IRegisterDirect::FP9_DIR  => 1,
        IRegisterDirect::FP10_DIR => 1,
        IRegisterDirect::FP11_DIR => 1,
        IRegisterDirect::FP12_DIR => 1,
        IRegisterDirect::FP13_DIR => 1,
        IRegisterDirect::FP14_DIR => 1,
        IRegisterDirect::FP15_DIR => 1,
    ];

    /**
     * @param  int    $iOpcode
     * @param  string $sOperandByteCode
     * @return string
     */
    public function attempt(int $iOpcode, string $sOperandByteCode): string {
        if (
            isset(self::FAST_PATH[$iOpcode])
            && 2 == strlen($sOperandByteCode)
            && isset(self::OPERANDS[ord($sOperandByteCode[0])])
            && isset(self::OPERANDS[ord($sOperandByteCode[1])])
        ) {
            // We could just return the code, but throwing as a code fold allows us to log it
            throw new CodeFoldException(
                "Fast path",
                chr(self::FAST_PATH[$iOpcode]) .
                chr((ord($sOperandByteCode[0]) & 0x0F) | ((ord($sOperandByteCode[1]) & 0x0F) << 4)) .
                chr($iOpcode)
            );
        }

        return chr($iOpcode) . $sOperandByteCode;
    }
}
