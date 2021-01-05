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
use ABadCafe\MC64K\Tokeniser;

/**
 * Statement
 *
 * Parses a complete assembler instruction statement into bytecode.
 */
class Statement implements MC64K\IParser, Defs\Mnemonic\IMatches {

    private Tokeniser\Instruction $oTokeniser;

    /**
     * @var IOperandSetParser[] $aOperandParsers
     */
    private array $aOperandParsers = [];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oTokeniser = new Tokeniser\Instruction();
        $this->addOperandSetParser(new OperandSet\None());
        $this->addOperandSetParser(new OperandSet\IntegerMonadic());
        $this->addOperandSetParser(new OperandSet\IntegerMonadicBranch());
        $this->addOperandSetParser(new OperandSet\FloatMonadicBranch());
        $this->addOperandSetParser(new OperandSet\IntegerDyadic());
        $this->addOperandSetParser(new OperandSet\IntegerDyadicBranch());
        $this->addOperandSetParser(new OperandSet\FloatDyadic());
        $this->addOperandSetParser(new OperandSet\FloatDyadicBranch());
        $this->addOperandSetParser(new OperandSet\FloatToIntegerDyadic());
        $this->addOperandSetParser(new OperandSet\IntegerToFloatDyadic());

        echo "Coverage: ", count($this->aOperandParsers), " opcodes.\n";
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource) : ?string {
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
                return chr($iOpcode) . $this->aOperandParsers[$iOpcode]->parse(
                    $iOpcode,
                    $oToken->aOperands,
                    $aSizes
                );
            } catch (CodeFoldException $oFold) {
                return $oFold->getAlternativeBytecode();
            }
        }
        return null;
    }

    /**
     * Add an OperandSetParser
     */
    private function addOperandSetParser(IOperandSetParser $oOperandParser) : void {
        foreach ($oOperandParser->getOpcodes() as $iOpcode) {
            $this->aOperandParsers[(string)$iOpcode] = $oOperandParser;
        }
    }
}
