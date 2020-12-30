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

namespace ABadCafe\MC64K\Parser\Instruction;
use ABadCafe\MC64K;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Tokeniser;

/**
 * Line
 *
 */
class Line implements MC64K\IParser, Defs\Mnemonic\IMatches {

    private Tokeniser\Instruction $oTokeniser;

    /**
     * @var IOperandSetParser[] $aOperandParsers
     */
    private array $aOperandParsers = [];

    public function __construct() {
        $this->oTokeniser = new Tokeniser\Instruction;
        $this->addOperandParser(new OperandSet\None());
        $this->addOperandParser(new OperandSet\IntegerDyadicBranch());
        $this->addOperandParser(new OperandSet\IntegerDyadic());
        $this->addOperandParser(new OperandSet\FloatDyadic());

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

            return chr($iOpcode) . $this->aOperandParsers[$iOpcode]->parse(
                $oToken->aOperands,
                $aSizes
            );
        }

        return null;
    }

    private function addOperandParser(IOperandSetParser $oOperandParser) : void {
        foreach ($oOperandParser->getOpcodes() as $iOpcode) {
            $this->aOperandParsers[(string)$iOpcode] = $oOperandParser;
        }
    }
}
