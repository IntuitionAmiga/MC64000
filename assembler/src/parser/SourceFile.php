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

namespace ABadCafe\MC64K\Parser;

/**
 * SourceFile
 *
 * Quick and dirty parser for a source file. This is considered a prototype version.
 */
class SourceFile {

    private array $aLineParsers = [];

    public function __construct() {
        $this->addLineParser(new SourceLine\Label\Local());
        $this->addLineParser(new SourceLine\Instruction\Statement());
    }

    public function process(string $sFile) {
        $rHandle = $this->openFile($sFile);

    }

    private function addLineParser(SourceLine\IParser $oParser) {
        $sCapture = $oParser->getCaptureExpression();
        if (isset($this->aLineParsers[$sCapture])) {
            throw new \Exception('Duplicate');
        }
        $this->aLineParsers[$sCapture] = $oParser;
    }

    private function openFile(string $sFile) {
        $rFile = null;
        if (
            !file_exists($sFile) ||
            !is_readable($sFile) ||
            !($rFile = fopen($sFile))
        ) {
            throw new \Exception('Could not read file ' . $sFile);
        }
        return $rFile;
    }

}
