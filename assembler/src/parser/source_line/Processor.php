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

namespace ABadCafe\MC64K\Parser\SourceLine;

/**
 * Processor
 *
 * This is basically a composite implementation of parsers that handle different types of line,
 * e.g. instruction statements, local labels, global labels etc.
 */
class Processor implements IParser {

    const COMMENT_MATCH = '/(?:;|\/\/).*$/';

    use TParser;

    private array $aParsers = [];

    public function __construct() {
        $this->aParsers = [
            new Instruction\Statement(),
            new Label\Local(),
            new Label\Exported()
        ];
    }

    /**
     * Stub - always true
     */
    public function checkLine(string $sLine) : bool {
        return true;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSourceLine) : ?string {
        $sSourceLine = $this->cleanSourceLine($sSourceLine);
        if (empty($sSourceLine)) {
            return '';
        }
        foreach ($this->aParsers as $oParser) {
            if ($oParser->checkLine($sSourceLine)) {
                return $oParser
                    ->setCurrentLine($this->iCurrentLineNumber)
                    ->setCurrentPosition($this->iCurrentOutputPosition)
                    ->parse($sSourceLine);
            }
        }
        return '';
    }

    public function dump() {

    }

    /**
     * Removes any comment guff
     */
    private function cleanSourceLine(string $sSourceLine) : string {
        return rtrim(preg_replace(self::COMMENT_MATCH, '', $sSourceLine));
    }
}

