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
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;
use ABadCafe\MC64K\Defs\Mnemonic\ILogical;
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Tokeniser;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Utils\Binary;
use ABadCafe\MC64K\Utils\Log;
use ABadCafe\MC64K\Defs\EffectiveAddress\IRegisterDirect;
use ABadCafe\MC64K\Defs\EffectiveAddress\IOther;

use function \strlen, \ord, \chr, \substr, \reset, \unpack, \pack;

/**
 * FastPathOptimiser
 *
 * Identifies fast path replacements for selected bytecode sequences. Fast path replacements rearrange the opcode
 * for selected instructions that have an alternative representation in the runtime interpreter, typically skipping
 * unnecessary steps, e.g. effective address decoding, etc. A limited number of intructions support such fast path
 * optimisation, generally for dyadic register to register oeprations.
 */
class FastPathOptimiser {

    const MONADIC_FAST_PATH = [
        IControl::DBNZ     => IControl::DBNZ_R,
        IDataMove::CLR_L   => IDataMove::R2R_CLR_L,
        IDataMove::CLR_Q   => IDataMove::R2R_CLR_Q,
    ];

    const DYADIC_FAST_PATH = [
        IDataMove::MOVE_L  => IDataMove::R2R_MOVE_L,
        IDataMove::MOVE_Q  => IDataMove::R2R_MOVE_Q,
        IDataMove::FMOVE_S => IDataMove::R2R_FMOVE_S,
        IDataMove::FMOVE_D => IDataMove::R2R_FMOVE_D,

        ILogical::AND_L => ILogical::R2R_AND_L,
        ILogical::AND_Q => ILogical::R2R_AND_Q,
        ILogical::OR_L  => ILogical::R2R_OR_L,
        ILogical::OR_Q  => ILogical::R2R_OR_Q,
        ILogical::EOR_L => ILogical::R2R_EOR_L,
        ILogical::EOR_Q => ILogical::R2R_EOR_Q,
        ILogical::NOT_L => ILogical::R2R_NOT_L,
        ILogical::NOT_Q => ILogical::R2R_NOT_Q,
        ILogical::LSL_L => ILogical::R2R_LSL_L,
        ILogical::LSL_Q => ILogical::R2R_LSL_Q,
        ILogical::LSR_L => ILogical::R2R_LSR_L,
        ILogical::LSR_Q => ILogical::R2R_LSR_Q,

        IArithmetic::EXTB_L  => IArithmetic::R2R_EXTB_L,
        IArithmetic::EXTB_Q  => IArithmetic::R2R_EXTB_Q,
        IArithmetic::EXTW_L  => IArithmetic::R2R_EXTW_L,
        IArithmetic::EXTW_Q  => IArithmetic::R2R_EXTW_Q,
        IArithmetic::EXTL_Q  => IArithmetic::R2R_EXTL_Q,
        IArithmetic::NEG_L   => IArithmetic::R2R_NEG_L,
        IArithmetic::NEG_Q   => IArithmetic::R2R_NEG_Q,
        IArithmetic::FNEG_S  => IArithmetic::R2R_FNEG_S,
        IArithmetic::FNEG_D  => IArithmetic::R2R_FNEG_D,
        IArithmetic::FABS_S  => IArithmetic::R2R_FABS_S,
        IArithmetic::FABS_D  => IArithmetic::R2R_FABS_D,
        IArithmetic::ADD_L   => IArithmetic::R2R_ADD_L,
        IArithmetic::ADD_Q   => IArithmetic::R2R_ADD_Q,
        IArithmetic::FADD_S  => IArithmetic::R2R_FADD_S,
        IArithmetic::FADD_D  => IArithmetic::R2R_FADD_D,
        IArithmetic::SUB_L   => IArithmetic::R2R_SUB_L,
        IArithmetic::SUB_Q   => IArithmetic::R2R_SUB_Q,
        IArithmetic::FSUB_S  => IArithmetic::R2R_FSUB_S,
        IArithmetic::FSUB_D  => IArithmetic::R2R_FSUB_D,
        IArithmetic::MULS_L  => IArithmetic::R2R_MULS_L,
        IArithmetic::MULS_Q  => IArithmetic::R2R_MULS_Q,
        IArithmetic::MULU_L  => IArithmetic::R2R_MULU_L,
        IArithmetic::MULU_Q  => IArithmetic::R2R_MULU_Q,
        IArithmetic::FMUL_S  => IArithmetic::R2R_FMUL_S,
        IArithmetic::FMUL_D  => IArithmetic::R2R_FMUL_D,
        IArithmetic::FDIV_S  => IArithmetic::R2R_FDIV_S,
        IArithmetic::FDIV_D  => IArithmetic::R2R_FDIV_D,
        IArithmetic::FMOD_S  => IArithmetic::R2R_FMOD_S,
        IArithmetic::FMOD_D  => IArithmetic::R2R_FMOD_D,
        IArithmetic::FSQRT_S => IArithmetic::R2R_FSQRT_S,
        IArithmetic::FSQRT_D => IArithmetic::R2R_FSQRT_D,

    ];

    const SPECIAL_CASES = [
        //IControl::DBNZ => 'handleDBNZ',
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
     * Attempt to find a fast path code fold option for the current opcode / encoded operand string. If a code fold is
     * found, a CodeFoldException for the complete statement is raised. Otherwise the vanilla bytecode is returned.
     *
     * @param  int    $iOpcode
     * @param  string $sOperandByteCode
     * @return string
     */
    public function attempt(int $iOpcode, string $sOperandByteCode): string {

        $sOpcode = $this->encodeOpcode($iOpcode);

        if (State\Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::OPT_USE_FAST_PATH)) {
            if (
                isset(self::MONADIC_FAST_PATH[$iOpcode]) &&
                1 <= strlen($sOperandByteCode)
            ) {
                $iOperand = ord($sOperandByteCode);
                if (isset(self::OPERANDS[$iOperand])) {
                    // We could just return the code, but throwing as a code fold allows us to log it
                    throw new FastPathFoldedException(
                        "Register monadic fast path",
                        chr(self::MONADIC_FAST_PATH[$iOpcode]) .
                        chr(($iOperand & 0x0F)) .
                        substr($sOperandByteCode, 1)
                    );
                }
            }
            else if (
                isset(self::DYADIC_FAST_PATH[$iOpcode]) &&
                2 <= strlen($sOperandByteCode)
            ) {
                // Extract the operands to check if they are full on register to reister
                $iDstOperand = ord($sOperandByteCode[0]);
                $iSrcOperand = ord($sOperandByteCode[1]);
                if ($iSrcOperand == IOther::SAME_AS_DEST) {
                    $iSrcOperand = $iDstOperand;
                }

                if (
                    isset(self::OPERANDS[$iDstOperand]) &&
                    isset(self::OPERANDS[$iSrcOperand])
                ) {
                    // We could just return the code, but throwing as a code fold allows us to log it
                    throw new FastPathFoldedException(
                        "Register to register dyadic fast path",
                        chr(self::DYADIC_FAST_PATH[$iOpcode]) .
                        chr(
                            ($iDstOperand & 0x0F) |
                            (($iSrcOperand & 0x0F) << 4)
                        ) .
                        substr($sOperandByteCode, 2)
                    );
                }
            } else if (isset(self::SPECIAL_CASES[$iOpcode])) {
                $cHandler = [$this, self::SPECIAL_CASES[$iOpcode]];
                $cHandler($iOpcode, $sOperandByteCode);
            }
        }

        return $sOpcode . $sOperandByteCode;
    }

    /**
     * Trampoline method that attempts a fast path optimsiation for the output of an earlier code fold. Examples are
     * where an instruction has been replaced by a general clear or set operation, which then can be optimised as a
     * clear or register type operation.
     */
    public function attemptOnFolded(CodeFoldException $oFolded): string {
        $sBytecode = $oFolded->getAlternativeBytecode();
        if (strlen($sBytecode) > 1) {
            $this->attempt(ord($sBytecode[0]), substr($sBytecode, 1));
        }
        return $sBytecode;
    }


    private function encodeOpcode(int $iOpcode): string {
        $sOpcode = '';
        do {
            $sOpcode = chr($iOpcode & 0xFF) . $sOpcode;
            $iOpcode >>= 8;
        } while ($iOpcode);
        return $sOpcode;
    }
}
