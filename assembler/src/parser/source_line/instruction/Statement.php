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
use ABadCafe\MC64K\Defs\Mnemonic\IDataMove;
use ABadCafe\MC64K\Defs\Mnemonic\ILogical;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Tokeniser;
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Utils\Log;
use ABadCafe\MC64K\Utils\Binary;


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
    private array $aCoverage = [];

    /**
     * Constructor
     */
    public function __construct() {
        $this->oTokeniser = new Tokeniser\Instruction();
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
                IDataMove::FSAVEM,
            ],
            new EffectiveAddress\Custom(new Operand\FixedInteger(Defs\IIntLimits::WORD)),
            new EffectiveAddress\GPRIndirectUpdating()
        ));

        $this->addOperandSetParser(new OperandSet\CustomDyadic(
            [
                IDataMove::LOADM,
                IDataMove::FLOADM,
            ],
            new EffectiveAddress\GPRIndirectUpdating(),
            new EffectiveAddress\Custom(new Operand\FixedInteger(Defs\IIntLimits::WORD))
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
            new EffectiveAddress\AllIntegerWriteable()
        ));

        $this->addOperandSetParser(new OperandSet\CustomMonadic(
            [IControl::HCF],
            new EffectiveAddress\Custom(new Operand\FixedInteger(Defs\IIntLimits::LONG)),
            [IControl::HCF => chr(0xFF)]
        ));

        // Todo - this probably isn't ideal.
        $this->addOperandSetParser(new OperandSet\CustomDyadic(
            [
                ILogical::BFCLR,
                ILogical::BFSET,
                ILogical::BFINS,
                ILogical::BFEXTS,
                ILogical::BFEXTU,
                ILogical::BFFFO,
                ILogical::BFCNT,
                ILogical::BFXXX,
            ],
            new EffectiveAddress\GPRDirect(),
            new EffectiveAddress\GPRDirect()
        ));
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
                Log::printf(
                    'Folding %s to \'%s\' (%s)',
                    $sSource,
                    Binary::format($oFold->getAlternativeBytecode()),
                    $oFold->getMessage()
                );
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

            if (isset($this->aOperandParsers[(string)$iOpcode])) {
                throw new \Exception(
                    "Duplicate Opcode Handler entry for Opcode #" . $iOpcode
                );
            }

            $this->aOperandParsers[(string)$iOpcode] = $oOperandParser;
        }
        $this->aCoverage = [];
    }

    public function getCoverage() : array {
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
}
