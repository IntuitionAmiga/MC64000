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

namespace ABadCafe\MC64K\IO;

/**
 * String based implementation of ISourceFile
 */
class SourceString implements ISourceFile {

    private string $sFilename;
    private array  $aLines      = [];
    private int    $iLineNumber = 0;
    private int    $iCount      = 0;

    /**
     * @param  string $sFilename
     * @throws \Exception
     */
    public function __construct(string $sSource, string $sFilename = 'string') {
        $this->sFilename = $sFilename;
        $this->aLines    = explode("\n", $sSource);
        $this->iCount    = count($this->aLines);
    }

    /**
     * @return string|null
     */
    public function readLine() : ?string {
        if ($this->iLineNumber < $this->iCount) {
            return $this->aLines[++$this->iLineNumber];
        }
        return null;
    }

    /**
     * Get the name of the file
     *
     * @return string
     */
    public function getFilename() : string {
        return $this->sFilename;
    }

    /**
     * Get the current line number
     *
     * @return int
     */
    public function getLineNumber() : int {
        return $this->iLineNumber + 1;
    }

    /**
     * Get the current line content
     *
     * @return string
     */
    public function getLine() : string {
        return $this->aLines[$this->iLineNumber];
    }
}
