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

namespace ABadCafe\MC64K\Parser\SourceLine\Label;

use ABadCafe\MC64K\Utils\Log;


class Registry {

    const
        I_FILE = 2,
        I_LINE = 1,
        I_OFST = 0
    ;

    private array  $aGlobalLabels = [];
    private array  $aLocalLabels  = [];
    private string $sCurrentFile  = '';

    /**
     * @param  string $sFile
     * @return self
     */
    public function setCurrentFile(string $sFile) : self {
        if ($this->sCurrentFile !== $sFile) {
            $this->sCurrentFile = $sFile;
        }
        return $this;
    }

    /**
     * Add a global label to the registry. A global label can be declared only once.
     *
     * @param  string $sLabel
     * @param  int    $iLine
     * @param  int    $iOffset
     * @return self
     * @throws Exception
     */
    public function addGlobal(string $sLabel, int $iLine, int $iOffset) : self {
        if (isset($this->aGlobalLabels[$sLabel])) {
            throw new \Exception(
                'Duplicate global: '    . $sLabel .
                ' already declared in ' . $this->aGlobalLabels[$sLabel][self::I_FILE] .
                ' on line ' . $this->aGlobalLabels[$sLabel][self::I_LINE]
            );
        }
        $this->aGlobalLabels[$sLabel] = [
            self::I_FILE => $this->sCurrentFile,
            self::I_LINE => $iLine,
            self::I_OFST => $iOffset
        ];
        Log::printf(
            "Added global label '%s' on line %d, code position %d",
            $sLabel,
            $iLine,
            $iOffset
        );
        return $this;
    }

    /**
     * Add a local label to the registry. A local label can be declared only once in the current file.
     *
     * @param  string $sLabel
     * @param  int    $iOffset
     * @return self
     * @throws Exception
     */
    public function addLocal(string $sLabel, int $iLine, int $iOffset) : self {
        if (isset($this->aLocalLabels[$this->sCurrentFile][$sLabel])) {
            throw new \Exception(
                'Duplicate local: '     . $sLabel .
                ' already declared in ' . $this->sCurrentFile .
                ' on line '             . $this->aLocalLabels[$this->sCurrentFile][$sLabel][self::I_LINE]
            );
        }
        $this->aLocalLabels[$this->sCurrentFile][$sLabel] = [
            self::I_LINE => $iLine,
            self::I_OFST => $iOffset
        ];
        Log::printf(
            "Added local label '%s' on line %d, code position %d",
            $sLabel,
            $iLine,
            $iOffset
        );
        return $this;
    }

    /**
     * Returns the global symbol table.
     *
     * @return array
     */
    public function getGlobals() : array {
        return $this->aGlobalLabels;
    }

    public function getOffsetForLocal(string $sLabel) : ?int {
        if (isset($this->aLocalLabels[$this->sCurrentFile][$sLabel])) {
            return $this->aLocalLabels[$this->sCurrentFile][$sLabel][self::I_OFST];
        }
        return null;
    }
}
