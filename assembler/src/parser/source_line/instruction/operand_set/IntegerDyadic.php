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

/**
 * IntegerDyadic
 *
 * For all vanilla integer destination @ source -> destination operations
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
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IDataMove::MOVE_B   => self::CLEAR_8,
        IDataMove::MOVE_W   => self::CLEAR_16,
        IDataMove::MOVE_L   => self::CLEAR_32,
        IDataMove::MOVE_Q   => self::CLEAR_64,
        ILogical::AND_B     => self::CLEAR_8,
        ILogical::AND_W     => self::CLEAR_16,
        ILogical::AND_L     => self::CLEAR_32,
        ILogical::AND_Q     => self::CLEAR_64,
        ILogical::OR_B      => self::FOLD,
        ILogical::OR_W      => self::FOLD,
        ILogical::OR_L      => self::FOLD,
        ILogical::OR_Q      => self::FOLD,
        ILogical::EOR_B     => null,
        ILogical::EOR_W     => null,
        ILogical::EOR_L     => null,
        ILogical::EOR_Q     => null,
        ILogical::NOT_B     => null,
        ILogical::NOT_W     => null,
        ILogical::NOT_L     => null,
        ILogical::NOT_Q     => null,
        ILogical::LSL_B     => self::FOLD,
        ILogical::LSL_W     => self::FOLD,
        ILogical::LSL_L     => self::FOLD,
        ILogical::LSL_Q     => self::FOLD,
        ILogical::LSR_B     => self::FOLD,
        ILogical::LSR_W     => self::FOLD,
        ILogical::LSR_L     => self::FOLD,
        ILogical::LSR_Q     => self::FOLD,
        ILogical::ROL_B     => self::FOLD,
        ILogical::ROL_W     => self::FOLD,
        ILogical::ROL_L     => self::FOLD,
        ILogical::ROL_Q     => self::FOLD,
        ILogical::ROR_B     => self::FOLD,
        ILogical::ROR_W     => self::FOLD,
        ILogical::ROR_L     => self::FOLD,
        ILogical::ROR_Q     => self::FOLD,
        ILogical::BCLR_B    => null,
        ILogical::BCLR_W    => null,
        ILogical::BCLR_L    => null,
        ILogical::BCLR_Q    => null,
        ILogical::BSET_B    => null,
        ILogical::BSET_W    => null,
        ILogical::BSET_L    => null,
        ILogical::BSET_Q    => null,
        IArithmetic::EXTB_W => self::CLEAR_16,
        IArithmetic::EXTB_L => self::CLEAR_32,
        IArithmetic::EXTB_Q => self::CLEAR_64,
        IArithmetic::EXTW_L => self::CLEAR_32,
        IArithmetic::EXTW_Q => self::CLEAR_64,
        IArithmetic::EXTL_Q => self::CLEAR_64,
        IArithmetic::ASL_B  => self::FOLD,
        IArithmetic::ASL_W  => self::FOLD,
        IArithmetic::ASL_L  => self::FOLD,
        IArithmetic::ASL_Q  => self::FOLD,
        IArithmetic::ASR_B  => self::FOLD,
        IArithmetic::ASR_W  => self::FOLD,
        IArithmetic::ASR_L  => self::FOLD,
        IArithmetic::ASR_Q  => self::FOLD,
        IArithmetic::ADD_B  => self::FOLD,
        IArithmetic::ADD_W  => self::FOLD,
        IArithmetic::ADD_L  => self::FOLD,
        IArithmetic::ADD_Q  => self::FOLD,
        IArithmetic::SUB_B  => self::FOLD,
        IArithmetic::SUB_W  => self::FOLD,
        IArithmetic::SUB_L  => self::FOLD,
        IArithmetic::SUB_Q  => self::FOLD,
        IArithmetic::NEG_B  => null,
        IArithmetic::NEG_W  => null,
        IArithmetic::NEG_L  => null,
        IArithmetic::NEG_Q  => null,
        IArithmetic::MULS_B => self::CLEAR_8,
        IArithmetic::MULS_W => self::CLEAR_16,
        IArithmetic::MULS_L => self::CLEAR_32,
        IArithmetic::MULS_Q => self::CLEAR_64,
        IArithmetic::MULU_B => self::CLEAR_8,
        IArithmetic::MULU_W => self::CLEAR_16,
        IArithmetic::MULU_L => self::CLEAR_32,
        IArithmetic::MULU_Q => self::CLEAR_64,
        IArithmetic::DIVS_B => self::THROW,
        IArithmetic::DIVS_W => self::THROW,
        IArithmetic::DIVS_L => self::THROW,
        IArithmetic::DIVS_Q => self::THROW,
        IArithmetic::DIVU_B => self::THROW,
        IArithmetic::DIVU_W => self::THROW,
        IArithmetic::DIVU_L => self::THROW,
        IArithmetic::DIVU_Q => self::THROW,
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
    public function getOpcodes() : array {
        return array_keys(self::OPCODES);
    }

    public function parse(int $iOpcode, array $aOperands, array $aSizes = []) : string {
        $sFullByteCode = parent::parse($iOpcode, $aOperands, $aSizes);
        if ($this->oSrcParser->wasImmediate() && 0 == $this->oSrcParser->getImmediate()) {
            if (isset(self::OPCODES[$iOpcode])) {
                $sAlternativeBytecode = '';
                switch(self::OPCODES[$iOpcode]) {
                    case self::CLEAR_8:
                        $sAlternativeBytecode = chr(IDataMove::CLR_B) . $this->sDstBytecode;
                        break;
                    case self::CLEAR_16:
                        $sAlternativeBytecode = chr(IDataMove::CLR_W) . $this->sDstBytecode;
                        break;
                    case self::CLEAR_32:
                        $sAlternativeBytecode = chr(IDataMove::CLR_L) . $this->sDstBytecode;
                        break;
                    case self::CLEAR_64:
                        $sAlternativeBytecode = chr(IDataMove::CLR_Q) . $this->sDstBytecode;
                        break;
                    case self::THROW:
                        throw new \UnexpectedValueException('Immediate zero is an illegal source operand for this operation');
                        break;
                    case self::FOLD:
                        break;
                    default:
                        throw new \UnexpectedValueException('Immediate zero resulted in an unhandled case');
                        break;
                }
                throw new CodeFoldException(
                    'Immediate Zero Source Operand',
                    $sAlternativeBytecode
                );
            }
        }
        return $sFullByteCode;
    }

}
