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
use ABadCafe\MC64K\Defs\Mnemonic\ILogical;
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;

use function \array_keys;

/**
 * IntegerDyadic
 *
 * For all vanilla integer destination @ source -> destination operations.
 */
class IntegerDyadic extends Dyadic {

    const
        NO_OP    = 0,
        CLEAR_8  = 1,
        CLEAR_16 = 2,
        CLEAR_32 = 3,
        CLEAR_64 = 4,
        FOLD     = 5,
        THROW    = 6
    ;

    /**
     * The set of specific opcodes that this Operand Parser applies to, mapped to folding
     * cases for given immediate source operands
     */
    const OPCODES = [
        IDataMove::MOVE_B   => [0 => 'foldClearDestinationByte'],
        IDataMove::MOVE_W   => [0 => 'foldClearDestinationWord'],
        IDataMove::MOVE_L   => [0 => 'foldClearDestinationLong'],
        IDataMove::MOVE_Q   => [0 => 'foldClearDestinationQuad'],
        ILogical::AND_B     => [0 => 'foldClearDestinationByte'],
        ILogical::AND_W     => [0 => 'foldClearDestinationWord'],
        ILogical::AND_L     => [0 => 'foldClearDestinationLong'],
        ILogical::AND_Q     => [0 => 'foldClearDestinationQuad'],
        ILogical::OR_B      => [0 => 'foldEmpty'],
        ILogical::OR_W      => [0 => 'foldEmpty'],
        ILogical::OR_L      => [0 => 'foldEmpty'],
        ILogical::OR_Q      => [0 => 'foldEmpty'],
        ILogical::EOR_B     => [],
        ILogical::EOR_W     => [],
        ILogical::EOR_L     => [],
        ILogical::EOR_Q     => [],
        ILogical::NOT_B     => [],
        ILogical::NOT_W     => [],
        ILogical::NOT_L     => [],
        ILogical::NOT_Q     => [],
        ILogical::LSL_B     => [0 => 'foldEmpty'],
        ILogical::LSL_W     => [0 => 'foldEmpty'],
        ILogical::LSL_L     => [0 => 'foldEmpty'],
        ILogical::LSL_Q     => [0 => 'foldEmpty'],
        ILogical::LSR_B     => [0 => 'foldEmpty'],
        ILogical::LSR_W     => [0 => 'foldEmpty'],
        ILogical::LSR_L     => [0 => 'foldEmpty'],
        ILogical::LSR_Q     => [0 => 'foldEmpty'],
        ILogical::ROL_B     => [0 => 'foldEmpty'],
        ILogical::ROL_W     => [0 => 'foldEmpty'],
        ILogical::ROL_L     => [0 => 'foldEmpty'],
        ILogical::ROL_Q     => [0 => 'foldEmpty'],
        ILogical::ROR_B     => [0 => 'foldEmpty'],
        ILogical::ROR_W     => [0 => 'foldEmpty'],
        ILogical::ROR_L     => [0 => 'foldEmpty'],
        ILogical::ROR_Q     => [0 => 'foldEmpty'],
        ILogical::BCLR_B    => [],
        ILogical::BCLR_W    => [],
        ILogical::BCLR_L    => [],
        ILogical::BCLR_Q    => [],
        ILogical::BSET_B    => [],
        ILogical::BSET_W    => [],
        ILogical::BSET_L    => [],
        ILogical::BSET_Q    => [],
        IArithmetic::EXTB_W => [0 => 'foldClearDestinationWord'],
        IArithmetic::EXTB_L => [0 => 'foldClearDestinationLong'],
        IArithmetic::EXTB_Q => [0 => 'foldClearDestinationQuad'],
        IArithmetic::EXTW_L => [0 => 'foldClearDestinationLong'],
        IArithmetic::EXTW_Q => [0 => 'foldClearDestinationQuad'],
        IArithmetic::EXTL_Q => [0 => 'foldClearDestinationQuad'],
        IArithmetic::ASL_B  => [0 => 'foldEmpty'],
        IArithmetic::ASL_W  => [0 => 'foldEmpty'],
        IArithmetic::ASL_L  => [0 => 'foldEmpty'],
        IArithmetic::ASL_Q  => [0 => 'foldEmpty'],
        IArithmetic::ASR_B  => [0 => 'foldEmpty'],
        IArithmetic::ASR_W  => [0 => 'foldEmpty'],
        IArithmetic::ASR_L  => [0 => 'foldEmpty'],
        IArithmetic::ASR_Q  => [0 => 'foldEmpty'],
        IArithmetic::ADD_B  => [0 => 'foldEmpty'],
        IArithmetic::ADD_W  => [0 => 'foldEmpty'],
        IArithmetic::ADD_L  => [0 => 'foldEmpty'],
        IArithmetic::ADD_Q  => [0 => 'foldEmpty'],
        IArithmetic::SUB_B  => [0 => 'foldEmpty'],
        IArithmetic::SUB_W  => [0 => 'foldEmpty'],
        IArithmetic::SUB_L  => [0 => 'foldEmpty'],
        IArithmetic::SUB_Q  => [0 => 'foldEmpty'],
        IArithmetic::NEG_B  => [0 => 'foldEmpty'],
        IArithmetic::NEG_W  => [0 => 'foldEmpty'],
        IArithmetic::NEG_L  => [0 => 'foldEmpty'],
        IArithmetic::NEG_Q  => [0 => 'foldEmpty'],
        IArithmetic::MULS_B => [0 => 'foldClearDestinationByte', 1 => 'foldEmpty'],
        IArithmetic::MULS_W => [0 => 'foldClearDestinationWord', 1 => 'foldEmpty'],
        IArithmetic::MULS_L => [0 => 'foldClearDestinationLong', 1 => 'foldEmpty'],
        IArithmetic::MULS_Q => [0 => 'foldClearDestinationQuad', 1 => 'foldEmpty'],
        IArithmetic::MULU_B => [0 => 'foldClearDestinationByte', 1 => 'foldEmpty'],
        IArithmetic::MULU_W => [0 => 'foldClearDestinationWord', 1 => 'foldEmpty'],
        IArithmetic::MULU_L => [0 => 'foldClearDestinationLong', 1 => 'foldEmpty'],
        IArithmetic::MULU_Q => [0 => 'foldClearDestinationQuad', 1 => 'foldEmpty'],
        IArithmetic::DIVS_B => [0 => 'trapIllegal', 1 => 'foldEmpty'],
        IArithmetic::DIVS_W => [0 => 'trapIllegal', 1 => 'foldEmpty'],
        IArithmetic::DIVS_L => [0 => 'trapIllegal', 1 => 'foldEmpty'],
        IArithmetic::DIVS_Q => [0 => 'trapIllegal', 1 => 'foldEmpty'],
        IArithmetic::DIVU_B => [0 => 'trapIllegal', 1 => 'foldEmpty'],
        IArithmetic::DIVU_W => [0 => 'trapIllegal', 1 => 'foldEmpty'],
        IArithmetic::DIVU_L => [0 => 'trapIllegal', 1 => 'foldEmpty'],
        IArithmetic::DIVU_Q => [0 => 'trapIllegal', 1 => 'foldEmpty'],
    ];

    const SKIP_IF_OPERANDS_SAME = [
        IDataMove::MOVE_B   => 1,
        IDataMove::MOVE_W   => 1,
        IDataMove::MOVE_L   => 1,
        IDataMove::MOVE_Q   => 1,
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new EffectiveAddress\AllIntegerReadable();
        $this->oDstParser = new EffectiveAddress\AllIntegerWriteable();
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

        if ($this->oSrcParser->wasImmediate()) {
            $iImmediate = $this->oSrcParser->getImmediate();
            if (isset(self::OPCODES[$iOpcode][$iImmediate])) {
                $sFoldFunc = self::OPCODES[$iOpcode][$iImmediate];
                $cCallback = [$this, $sFoldFunc];
                $sAlternativeBytecode = $cCallback($this->sSrcBytecode, $this->sDstBytecode);
                if (empty($sAlternativeBytecode)) {
                    // If we don't lose side any important effects, empty is fine
                    if (false === $this->oDstParser->hasSideEffects()) {
                        throw new CodeFoldException(
                            'SrcEA #' . $iImmediate . ' using ' . $sFoldFunc,
                            $sAlternativeBytecode
                        );
                    }
                } else {
                    throw new CodeFoldException(
                        'SrcEA #' . $iImmediate . ' using ' . $sFoldFunc,
                        $sAlternativeBytecode
                    );
                }
            }
        }
        return $sFullByteCode;
    }

}
