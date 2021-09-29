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

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction\Operand;
use ABadCafe\MC64K;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Parser\SourceLine\Instruction\CodeFoldException;

use function \preg_match, \substr, \pack, \strlen, \str_pad;

/**
 * FixedInteger
 *
 * Operand parser for fixed integer values.
 */
class FixedInteger implements MC64K\IParser, Defs\IIntLimits {

    const PREFIX = '#';

    private int $iSize;

    /**
     * Constructor
     *
     * @param int $iSize
     */
    public function __construct(int $iSize) {
        if (!isset(self::WORD_SIZES[$iSize])) {
            throw new \UnexpectedValueException();
        }
        $this->iSize = $iSize;
    }

    /**
     * @inheritDoc
     *
     * If the resolved displacement value is zero (branch to next instruction)
     * the code will be folded out.
     *
     */
    public function parse(string $sSource): ?string {
        $aMatches = Parser\Utils\Integer::match($sSource, self::PREFIX);
        if (!empty($aMatches)) {
            $iValue = Parser\Utils\Integer::parseMatch($aMatches, $this->iSize);
            return pack(self::WORD_SIZES[$this->iSize][self::BIN_FORMAT], $iValue);
        }
        return null;
    }

}
