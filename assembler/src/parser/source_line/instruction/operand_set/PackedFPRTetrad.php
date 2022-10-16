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
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\State;

/**
 * PackedFPRTetrad
 *
 * For register to register operations where both registers are packed into a single byte.
 */
class PackedFPRTetrad extends Tetradic {

    const OPCODES = [
        IArithmetic::R2R_FMADD_S,
        IArithmetic::R2R_FMADD_D,
    ];

    /**
     * Constructor
     */
    public function __construct() {
        parent::__construct();
        $this->oSrcParser  =
        $this->oDstParser  =
        $this->oSrc2Parser =
        $this->oSrc3Parser = new EffectiveAddress\FPRDirect();
    }

    public function getOpcodes(): array {
        return self::OPCODES;
    }

    public function parse(int $iOpcode, array $aOperands, array $aSizes = []): string {
        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);

        $oState = State\Coordinator::get()
            ->setCurrentStatementLength(Defs\IOpcodeLimits::SIZE);
        $iDstIndex    = $this->getDestinationOperandIndex();
        $sDstBytecode = $this->oDstParser
            ->setOperationSize($aSizes[$iDstIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iDstIndex]);
        if (null === $sDstBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iDstIndex] . ' not a valid destination operand'
            );
        }

        $iSrc1Index    = $this->getSource1OperandIndex();
        $sSrc1Bytecode = $this->oSrcParser
            ->setOperationSize($aSizes[$iSrc1Index] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrc1Index]);
        if (null === $sSrc1Bytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrc1Index] . ' not a valid source 1 operand'
            );
        }

        $iSrc2Index    = $this->getSource2OperandIndex();
        $sSrc2Bytecode = $this->oSrc2Parser
            ->setOperationSize($aSizes[$iSrc2Index] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrc2Index]);
        if (null === $sSrc2Bytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrc2Index] . ' not a valid source 2 operand'
            );
        }

        $iSrc3Index    = $this->getSource3OperandIndex();
        $sSrc3Bytecode = $this->oSrc3Parser
            ->setOperationSize($aSizes[$iSrc3Index] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrc3Index]);
        if (null === $sSrc2Bytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrc3Index] . ' not a valid source 3 operand'
            );
        }

        $iDstReg  = ord($sDstBytecode)  & 0x0F;
        $iSrc1Reg = ord($sSrc1Bytecode) & 0x0F;
        $iSrc2Reg = ord($sSrc2Bytecode) & 0x0F;
        $iSrc3Reg = ord($sSrc3Bytecode) & 0x0F;

        return chr($iSrc1Reg << 4 | $iDstReg) . chr($iSrc3Reg << 4 | $iSrc2Reg);
    }

}
