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

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction\OperandSet;
use ABadCafe\MC64K\Parser\SourceLine\Instruction\CodeFoldException;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;
use ABadCafe\MC64K\Defs\EffectiveAddress\IOther;

use function \chr, \pack, \array_keys, \is_nan;
use function \abs, \sqrt, \exp, \log, \sin, \cos, \tan, \asin, \acos, \atan;

/**
 * FloatDyadic
 *
 * For all vanilla float destination @ source -> destination operations.
 */
class FloatDyadic extends Dyadic {



    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IDataMove::FMOVES_D    => false,
        IDataMove::FMOVED_S    => false,
        IDataMove::FMOVE_S     => false,
        IDataMove::FMOVE_D     => false,
        IArithmetic::FADD_S    => false,
        IArithmetic::FADD_D    => false,
        IArithmetic::FSUB_S    => false,
        IArithmetic::FSUB_D    => false,
        IArithmetic::FNEG_S    => false,
        IArithmetic::FNEG_D    => false,
        IArithmetic::FMUL_S    => false,
        IArithmetic::FMUL_D    => false,
        IArithmetic::FDIV_S    => false,
        IArithmetic::FDIV_D    => false,
        IArithmetic::FMOD_S    => false,
        IArithmetic::FMOD_D    => false,
        IArithmetic::FSINCOS_S => false,
        IArithmetic::FSINCOS_D => false,
        IArithmetic::FABS_S    => [0, 'abs'],
        IArithmetic::FABS_D    => [1, 'abs'],
        IArithmetic::FSQRT_S   => [0, 'sqrt'],
        IArithmetic::FSQRT_D   => [1, 'sqrt'],
        IArithmetic::FACOS_S   => [0, 'acos'],
        IArithmetic::FACOS_D   => [1, 'acos'],
        IArithmetic::FASIN_S   => [0, 'asin'],
        IArithmetic::FASIN_D   => [1, 'asin'],
        IArithmetic::FATAN_S   => [0, 'atan'],
        IArithmetic::FATAN_D   => [1, 'atan'],
        IArithmetic::FCOS_S    => [0, 'cos'],
        IArithmetic::FCOS_D    => [1, 'cos'],
        IArithmetic::FSIN_S    => [0, 'sin'],
        IArithmetic::FSIN_D    => [1, 'sin'],
        IArithmetic::FTAN_S    => [0, 'tan'],
        IArithmetic::FTAN_D    => [1, 'tan'],
        IArithmetic::FETOX_S   => [0, 'exp'],
        IArithmetic::FETOX_D   => [1, 'exp'],
        IArithmetic::FLOGN_S   => [0, 'log'],
        IArithmetic::FLOGN_D   => [1, 'log'],
        IArithmetic::FLOG2_S   => [0, self::class . '::immediateLog2'],
        IArithmetic::FLOG2_D   => [1, self::class . '::immediateLog2'],
        IArithmetic::FTWOTOX_S => [0, self::class . '::immediate2ToX'],
        IArithmetic::FTWOTOX_D => [1, self::class . '::immediate2ToX'],
    ];


    const SKIP_IF_OPERANDS_SAME = [
        IDataMove::FMOVE_S   => 1,
        IDataMove::FMOVE_D   => 1,
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new EffectiveAddress\AllFloatReadable();
        $this->oDstParser = new EffectiveAddress\AllFloatWriteable();
        parent::__construct();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes(): array {
        return array_keys(self::OPCODES);
    }

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []): string {
        $sFullByteCode = parent::parse($iOpcode, $aOperands, $aSizes);

        if (
            isset(self::SKIP_IF_OPERANDS_SAME[$iOpcode]) &&
            $this->sourceOperandWasOptimised()
        ) {
            throw new CodeFoldException(
                'Operation has no effect'
            );
        }

        if ( ( $aConversion = self::OPCODES[$iOpcode]) && $this->oSrcParser->wasImmediate()) {
            $fImmediate = (float)$this->oSrcParser->getImmediate();
            $cConstExp = $aConversion[1];
            $fConstant = $cConstExp($fImmediate);

            if (is_nan($fConstant)) {
                throw new \Exception(
                    'Constant evaluation of ' . $cConstExp .
                    '(' . $fImmediate . ') results in NaN'
                );
            }

            throw new CodeFoldException(
                'Const expression ' . $cConstExp .
                '(' . $fImmediate . ') replaced by ' . $fConstant,
                $aConversion[0] ?
                    $this->emitMoveConstDouble($fConstant) :
                    $this->emitMoveConstSingle($fConstant)
            );
        }

        return $sFullByteCode;
    }

    private static function immediateLog2(float $fValue): float {
        return log($fValue, 2.0);
    }

    private static function immediate2ToX(float $fValue): float {
        return 2.0 ** $fValue;
    }

    /**
     * Emit the bytecode to load an immediate double precision float to the destination.
     *
     * @param  float $fValue
     * @return string
     */
    private function emitMoveConstDouble(float $fValue): string {
        return
            chr(IDataMove::FMOVE_D)     . $this->sDstBytecode .
            chr(IOther::FLT_IMM_DOUBLE) . pack('e', $fValue);
    }

    /**
     * Emit the bytecode to load an immediate single precision float to the destination.
     *
     * @param  float $fValue
     * @return string
     */
    private function emitMoveConstSingle(float $fValue): string {
        return
            chr(IDataMove::FMOVE_S)     . $this->sDstBytecode .
            chr(IOther::FLT_IMM_SINGLE) . pack('g', $fValue);
    }
}
