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
use ABadCafe\MC64K\Parser\SourceLine\Instruction;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;

use function \array_filter, \count;

/**
 * None
 *
 * For those operations that have no operands to parse.
 */
class None implements Instruction\IOperandSetParser {

    /**
     * @inheritDoc
     */
    public function getOpcodes(): array {
        return [
            IControl::STOP,
            IControl::RTS,
        ];
    }

    /**
     * @inheritDoc
     */
    public function parse(int $iOpcode, array $aOperands, array $aSizes = []): string {
        $aOperands = array_filter($aOperands);
        if (!empty($aOperands)) {
            throw new \LengthException(__CLASS__ . ' expects no operands, got ' . count($aOperands));
        }
        return '';
    }
}
