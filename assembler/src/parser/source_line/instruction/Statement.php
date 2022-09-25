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
use ABadCafe\MC64K\Parser\SourceLine;
use ABadCafe\MC64K;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;
use ABadCafe\MC64K\Defs\Mnemonic\ILogical;
use ABadCafe\MC64K\Defs\Mnemonic\IArithmetic;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Tokeniser;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Utils\Log;
use ABadCafe\MC64K\Utils\Binary;
use ABadCafe\MC64K\State;

use function \chr, \array_flip, \explode, \get_class, \end, \ksort, \trim;

/**
 * Statement
 *
 * Parses a complete assembler instruction statement into bytecode.
 */
class Statement implements SourceLine\IParser, Defs\Mnemonic\IMatches {

    use SourceLine\TParser;

    const BASIC_LINE_MATCH = '/^\s+[a-z2]+/';

    private Tokeniser\Instruction $oTokeniser;

    /** @var IOperandSetParser[] $aOperandParsers */
    private array $aOperandParsers = [];

    /** @var string[][] $aCoverage */
    private array $aCoverage = [];

    private FastPathOptimiser $oOptimiser;

    /**
     * Constructor
     */
    public function __construct() {
        $this->oTokeniser = new Tokeniser\Instruction();
        $this->oOptimiser = new FastPathOptimiser();
        $this->addOperandSetParser(new OperandSet\None());
        $this->addOperandSetParser(new OperandSet\BranchDisplacementOnly());
        $this->addOperandSetParser(new OperandSet\IntegerMonadicAddress());
        $this->addOperandSetParser(new OperandSet\IntegerMonadic());
        $this->addOperandSetParser(new OperandSet\IntegerMonadicBranch());
        $this->addOperandSetParser(new OperandSet\FloatMonadicBranch());
        $this->addOperandSetParser(new OperandSet\IntegerDyadic());
        $this->addOperandSetParser(new OperandSet\IntegerDyadicBranch());
        $this->addOperandSetParser(new OperandSet\FloatDyadic());
        $this->addOperandSetParser(new OperandSet\FloatDyadicBranch());
        $this->addOperandSetParser(new OperandSet\FloatToIntegerDyadic());
        $this->addOperandSetParser(new OperandSet\IntegerToFloatDyadic());
        $this->addOperandSetParser(new OperandSet\PackedGPRPair());
        $this->addOperandSetParser(new OperandSet\PackedFPRPair());

        // Now for the awkward gits...
        $this->addOperandSetParser(new OperandSet\CustomDyadic(
            [IDataMove::LEA],
            new EffectiveAddress\AllControlAddressing(),
            new EffectiveAddress\AllIntegerWriteable()
        ));

        $this->addOperandSetParser(new OperandSet\CustomDyadic(
            [
                IDataMove::SAVEM,
            ],
            new EffectiveAddress\Custom(new Operand\FixedInteger(Defs\IIntLimits::LONG)),
            new EffectiveAddress\GPRIndirectUpdating()
        ));

        $this->addOperandSetParser(new OperandSet\CustomDyadic(
            [
                IDataMove::LOADM,
            ],
            new EffectiveAddress\GPRIndirectUpdating(),
            new EffectiveAddress\Custom(new Operand\FixedInteger(Defs\IIntLimits::LONG))
        ));

        $this->addOperandSetParser(new OperandSet\CustomDyadic(
            [IDataMove::LINK],
            new EffectiveAddress\ARDirect(),
            new EffectiveAddress\Custom(new Operand\FixedInteger(Defs\IIntLimits::LONG))
        ));

        $this->addOperandSetParser(new OperandSet\CustomMonadic(
            [IDataMove::UNLK],
            new EffectiveAddress\ARDirect()
        ));

        $this->addOperandSetParser(new OperandSet\CustomMonadicBranch(
            [IControl::DBNZ],
            new EffectiveAddress\AllIntegerWriteable(),
            true
        ));

        $this->addOperandSetParser(new OperandSet\CustomDyadic(
            [IControl::HOST],
            new EffectiveAddress\Custom(new Operand\FixedInteger(Defs\IIntLimits::BYTE)),
            new EffectiveAddress\Custom(new Operand\FixedInteger(Defs\IIntLimits::BYTE))
        ));
    }


    /**
     * @inheritDoc
     */
    public function parse(string $sSource): ?string {

        $oToken = $this->oTokeniser->tokenise($sSource);
        if ($oToken) {
            if (!isset(self::MATCHES[$oToken->sMnemonic])) {
                throw new \OutOfBoundsException($oToken->sMnemonic . ' not a recognised mnemonic');
            }
            $iOpcode = self::MATCHES[$oToken->sMnemonic];
            if (!isset($this->aOperandParsers[$iOpcode])) {
                throw new \OutOfBoundsException($oToken->sMnemonic . ' does not have an operand parser configured');
            }

            $aSizes = Defs\Mnemonic\IOperandSizes::MAP[$iOpcode] ?? [];
            try {
                // Note that code optimisation exceptions can be triggered by both the optimiser and the original
                // statement under evaluation.
                return $this->oOptimiser->attempt(
                    $iOpcode,
                    $this->aOperandParsers[$iOpcode]->parse(
                        $iOpcode,
                        $oToken->aOperands,
                        $aSizes
                    )
                );
            } catch (FastPathFoldedException $oFold) {
                // We found a fast path alternative, typically this is a variation that does not require any runtime
                // effective address calculations due to having a register direct form available.
                return $this->handleFastPath($oFold, $sSource);
            } catch (CodeFoldException $oFold) {
                // A deeper code fold was found, e.g. a total replacement for an operation. This could be anything
                // from a no-op to an alternative instruction for the same task. However, the generated instruction
                // may also have a fast path alternative we can evaluate here. For example, a muls.q #0, d0 should
                // result in a substitution to clr.q d0, however this will be using the generic effective address
                // variant of that operation. The fast path optimiser can replace it with a register direct variant.
                try {
                    if (State\Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_CODE_FOLD)) {
                        Log::printf(
                            'Substitute \'%s\' to \'%s\' (%s)',
                            trim($sSource),
                            Binary::format($oFold->getAlternativeBytecode()),
                            $oFold->getMessage()
                        );
                    }
                    return $this->oOptimiser->attemptOnFolded($oFold);
                } catch (FastPathFoldedException $oFold) {
                    return $this->handleFastPath($oFold, $sSource);
                }
            }
        }
        return null;
    }

    private function handleFastPath(FastPathFoldedException $oFold, string $sSource): string {
        if (State\Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_CODE_FOLD)) {
            Log::printf(
                'Fast Path \'%s\' to \'%s\' (%s)',
                trim($sSource),
                Binary::format($oFold->getAlternativeBytecode()),
                $oFold->getMessage()
            );
        }
        return $oFold->getAlternativeBytecode();
    }

    /**
     * Returns the set of covered mnemonics
     *
     * @return string[][]
     */
    public function getCoverage(): array {
        if (empty($this->aCoverage)) {
            $aMnemonics = array_flip(Defs\Mnemonic\IMatches::MATCHES);
            foreach ($this->aOperandParsers as $iOpcode => $oOperandParser) {
                $aName = explode('\\',  get_class($oOperandParser));
                $this->aCoverage[$iOpcode] = [$aMnemonics[$iOpcode], end($aName)];
            }
        }
        ksort($this->aCoverage);
        return $this->aCoverage;
    }

    /**
     * Add an OperandSetParser
     *
     * @param IOperandSetParser $oOperandParser
     */
    private function addOperandSetParser(IOperandSetParser $oOperandParser): void {
        foreach ($oOperandParser->getOpcodes() as $iOpcode) {
            if (isset($this->aOperandParsers[(string)$iOpcode])) {
                throw new \Exception(
                    "Duplicate Opcode Handler entry for Opcode #" . $iOpcode
                );
            }
            $this->aOperandParsers[(string)$iOpcode] = $oOperandParser;
        }
        $this->aCoverage = [];
    }
}
