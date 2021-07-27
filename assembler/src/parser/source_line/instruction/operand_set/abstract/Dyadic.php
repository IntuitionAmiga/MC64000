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
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;

/**
 * Dyadic
 *
 * Base for all vanilla destination @ source -> destination operations.
 */
abstract class Dyadic extends Monadic {

    const
        DEF_OPERAND_DST   = 1,
        MIN_OPERAND_COUNT = 2
    ;

    protected EffectiveAddress\IParser $oDstParser;

    /**
     * @var int[] $aSameAsDestination
     */
    protected static array $aSameAsDestination = [];

    protected ?string $sDstBytecode;
    protected ?string $sSrcBytecode;

    /**
     * Base constructor
     */
    public function __construct() {
        if (empty(self::$aSameAsDestination)) {
            self::$aSameAsDestination = array_combine(
                Defs\EffectiveAddress\ISameAsDestination::LEGAL,
                Defs\EffectiveAddress\ISameAsDestination::LEGAL
            );
        }
    }

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []) : string {
        $this->sSrcBytecode = null;
        $this->assertMinimumOperandCount($aOperands, self::MIN_OPERAND_COUNT);
        $oState = State\Coordinator::get()
            ->setCurrentStatementLength(Defs\IOpcodeLimits::SIZE);
        $iDstIndex          = $this->getDestinationOperandIndex();
        $this->sDstBytecode = $this->oDstParser
            ->setOperationSize($aSizes[$iDstIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iDstIndex]);
        if (null === $this->sDstBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iDstIndex] . ' not a valid destination operand'
            );
        }

        $oState->setCurrentStatementLength(Defs\IOpcodeLimits::SIZE + strlen($this->sDstBytecode));

        $iSrcIndex    = $this->getSourceOperandIndex();
        $sSrcBytecode = $this->oSrcParser
            ->setOperationSize($aSizes[$iSrcIndex] ?? self::DEFAULT_SIZE)
            ->parse($aOperands[$iSrcIndex]);
        if (null === $sSrcBytecode) {
            throw new \UnexpectedValueException(
                $aOperands[$iSrcIndex] . ' not a valid source operand'
            );
        }

        $this->sSrcBytecode = $this->optimiseSourceOperandBytecode($sSrcBytecode, $this->sDstBytecode);

        $oState->setCurrentStatementLength(Defs\IOpcodeLimits::SIZE + strlen($this->sDstBytecode) + strlen($this->sSrcBytecode));

        return $this->sDstBytecode . $this->sSrcBytecode;
    }

    /**
     * Returns the expected index in the operands array for the destination operand
     *
     * @return int
     */
    protected function getDestinationOperandIndex() : int {
        return self::DEF_OPERAND_DST;
    }

    /**
     * Checks to see if the source operand can be optimised to "same as destination"
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return bool
     */
    protected function canOptimiseSourceOperand(string $sSrcBytecode, string $sDstBytecode) : bool {
        // If the source operand bytecode is the same as the destination and the destination mode
        // is in the set defined by ISameAsDestination, we can use the special "same as destination" EA mode
        return $sSrcBytecode === $sDstBytecode && isset(self::$aSameAsDestination[ord($sDstBytecode[0])]);
    }

    /**
     * Checks to see if the source operand bytecode can be replaced with the "same as destination" EA mode.
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return string bytecode
     */
    protected function optimiseSourceOperandBytecode(string $sSrcBytecode, string $sDstBytecode) : string {
        if ($this->canOptimiseSourceOperand($sSrcBytecode, $sDstBytecode)) {
            $sSrcBytecode = chr(Defs\EffectiveAddress\IOther::SAME_AS_DEST);
        }
        return $sSrcBytecode;
    }

    protected function sourceOperandWasOptimised() : bool {
        return $this->sSrcBytecode === chr(Defs\EffectiveAddress\IOther::SAME_AS_DEST);
    }

    /**
     * Common dyadic code folder:
     *
     * Emit a clr.b <destination> operation, used for folding out code where immediate zero is a source.
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return string
     */
    protected function foldClearDestinationByte(string $sSrcBytecode, string $sDstBytecode) : string {
        return chr(IDataMove::CLR_B) . $sDstBytecode;
    }

    /**
     * Common dyadic code folder:
     *
     * Emit a clr.w <destination> operation, used for folding out code where immediate zero is a source.
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return string
     */
    protected function foldClearDestinationWord(string $sSrcBytecode, string $sDstBytecode) : string {
        return chr(IDataMove::CLR_W) . $sDstBytecode;
    }

    /**
     * Common dyadic code folder:
     *
     * Emit a clr.l <destination> operation, used for folding out code where immediate zero is a source.
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return string
     */
    protected function foldClearDestinationLong(string $sSrcBytecode, string $sDstBytecode) : string {
        return chr(IDataMove::CLR_L) . $sDstBytecode;
    }

    /**
     * Emit a clr.q <destination> operation, used for folding out code where immediate zero is a source.
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return string
     */
    protected function foldClearDestinationQuad(string $sSrcBytecode, string $sDstBytecode) : string {
        return chr(IDataMove::CLR_Q) . $sDstBytecode;
    }

    /**
     * Common dyadic code folder:
     *
     * Emit nothing
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return string
     */
    protected function foldEmpty(string $sSrcBytecode, string $sDstBytecode) : string {
        return '';
    }
    /**
     * Throw an exception where immediate zero is an illegal source operand
     *
     * @param  string $sSrcBytecode
     * @param  string $sDstBytecode
     * @return string
     */
    protected function trapIllegal(string $sSrcBytecode, string $sDstBytecode) : string {
        throw new \UnexpectedValueException('Immediate zero is an illegal source operand for this operation');
    }
}
