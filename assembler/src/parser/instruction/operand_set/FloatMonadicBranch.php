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

namespace ABadCafe\MC64K\Parser\Instruction\OperandSet;
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Parser\Instruction\CodeFoldException;

/**
 * FloatMonadicBranch
 *
 * For all vanilla float compare and branch
 */
class FloatMonadicBranch extends Monadic {

    use TBranching;

    const
        OPERAND_TARGET    = 1,
        MIN_OPERAND_COUNT = 2,
        FIXED_LENGTH      = 4
    ;

    /**
     * The set of specific opcodes that this Operand Parser applies to
     */
    const OPCODES = [
        IControl::FBIZ_S => 'isZero',
        IControl::FBIZ_D => 'isZero',
        IControl::FBNZ_S => 'isNotZero',
        IControl::FBNZ_D => 'isNotZero',
        IControl::FBMI_S => 'isMinus',
        IControl::FBMI_D => 'isMinus',
        IControl::FBPL_S => 'isPlus',
        IControl::FBPL_D => 'isPlus',
    ];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oSrcParser = new Parser\EffectiveAddress\AllFloatReadable();
        $this->oTgtParser = new Parser\Instruction\Operand\BranchDisplacement();
    }

    /**
     * @inheritDoc
     */
    public function getOpcodes() : array {
        return array_keys(self::OPCODES);
    }

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []) : string {
        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);

        $sDisplacement = $this->oTgtParser->parse($aOperands[self::OPERAND_TARGET]);
        $iSrcIndex     = $this->getSourceOperandIndex();
        $sSrcBytecode  = $this->oSrcParser
            ->setOperationSize($aSizes[$iSrcIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrcIndex]);
        if (null === $sSrcBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrcIndex] . ' not a valid comparison operand'
            );
        }

        $sBytecode = $sSrcBytecode . $sDisplacement;
        $this->checkBranchDisplacement($sBytecode);

        if ($this->oSrcParser->wasImmediate()) {
            $cCallback = [$this, self::OPCODES[$iOpcode]];
            throw new CodeFoldException(
                'Compile time constant comparison ',
                $cCallback(
                    $this->oSrcParser->getImmediate(),
                    $this->oTgtParser->getLastDisplacement(),
                    strlen($sBytecode)
                )
            );
        }
        return $sBytecode;
    }

    private function isZero(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        if (0.0 !== $fImmediate) {
            // Not zero? Do nothing.
            return '';
        }
        return $this->encodeFixedBranch($iDisplacement, $iOriginalSize);
    }

    private function isNotZero(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        if (0.0 === $fImmediate) {
            // Is zero? Do nothing.
            return '';
        }
        return $this->encodeFixedBranch($iDisplacement, $iOriginalSize);
    }

    private function isMinus(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        if ($fImmediate >= 0.0) {
            // Not negative? Do nothing.
            return '';
        }
        return $this->encodeFixedBranch($iDisplacement, $iOriginalSize);
    }

    private function isPlus(float $fImmediate, int $iDisplacement, int $iOriginalSize) : string {
        if ($fImmediate < 0.0) {
            // Negative? Do nothing.
            return '';
        }
        return $this->encodeFixedBranch($iDisplacement, $iOriginalSize);
    }

    private function encodeFixedBranch(int $iDisplacement, int $iOriginalSize) : string {
        if ($iDisplacement < 0) {
            $iDisplacement = $iDisplacement + $iOriginalSize - self::FIXED_LENGTH;
        }
        return chr(IControl::BRA) . pack('V', $iDisplacement);
    }

}
