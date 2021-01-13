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
 *
 */
trait TParser {

    protected int $iCurrentLineNumber = 0;

    protected int $iCurrentOutputPosition = 0;

    /**
     * @implements IParser::setCurrentLine()
     */
    public function setCurrentLine(int $iLineNumber) : self {
        $this->iCurrentLineNumber = $iLineNumber;
        return $this;
    }

    /**
     * @implements IParser::setCurrentPosition()
     */
    public function setCurrentPosition(int $iPosition) : self {
        $this->iCurrentOutputPosition = $iPosition;
        return $this;
    }

    /**
     * @implements IParser::checkLine()
     */
    public function checkLine(string $sLine) : bool {
        return (bool)preg_match(self::BASIC_LINE_MATCH, $sLine);
    }

}
