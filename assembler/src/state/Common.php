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

namespace ABadCafe\MC64K\State;

use ABadCafe\MC64K\Utils\Log;

/**
 * DSG Antipattern: Dirty Great Singleton
 *
 * It may not be the right tool for most things, but here, it makes some sense.
 */
class Common {

    const
        I_FILE = 2,
        I_LINE = 1,
        I_POSN = 0
    ;

    private static ?self $oInstance = null; // Singleton

    private array
        $aGlobalLabels = [],
        $aLocalLabels  = []
    ;

    private string $sCurrentFilename = '';

    private int
        $iCurrentLineNumber        = 0,
        $iCurrentStatementPosition = 0,
        $iCurrentStatementOffset   = 0
    ;

    private function __construct() {

    }

    /**
     * Singleton accessor
     *
     * @return self
     */
    public static function get() : self {
        if (null === self::$oInstance) {
            self::$oInstance = new self;
        }
        return self::$oInstance;
    }

    /**
     * @return string
     */
    public function getCurrentFilename() : string {
        return $this->sCurrentFilename;
    }

    /**
     * @param  string $sFile
     * @return self
     */
    public function setCurrentFilename(string $sFilename) : self {
        if ($this->sCurrentFilename !== $sFilename) {
            $this->sCurrentFilename        = $sFilename;
            $this->iCurrentLineNumber      = 0;
            $this->iCurrentStatementOffset = 0;

        }
        return $this;
    }

    /**
     * @return int
     */
    public function getCurrentLineNumber() : int {
        return $this->iCurrentLineNumber;
    }

    /**
     * @param  int $iLineNumber
     * @return self
     */
    public function setCurrentLineNumber(int $iLineNumber) : self {
        $this->iCurrentLineNumber = $iLineNumber;
        return $this;
    }

    /**
     * @param  int $iLineCount
     * @return self
     */
    public function incrementCurrentLineNumber(int $iLineCount = 1) : self {
        $this->iCurrentLineNumber += $iLineCount;
        return $this;
    }

    /**
     * @return int
     */
    public function getCurrentStatementPosition() : int {
        return $this->iCurrentStatementPosition;
    }

    /**
     * @param  int $iStatementPosition
     * @return self
     */
    public function setCurrentStatementPosition(int $iStatementPosition) : self {
        $this->iCurrentStatementPosition = $iStatementPosition;
        return $this;
    }

    /**
     * @param  int $iCodeLength
     * @return self
     */
    public function incrementStatementPosition(int $iCodeLength) : self {
        $this->iCurrentStatementPosition += $iCodeLength;
        return $this;
    }


    /**
     * @return int
     */
    public function getCurrentStatementOffset() : int {
        return $this->iCurrentStatementOffset;
    }

    /**
     * @param  int $iStatementOffset
     * @return self
     */
    public function setCurrentStatementOffet(int $iStatementOffset) : self {
        $this->iCurrentStatementOffset = $iStatementOffset;
        return $this;
    }

    /**
     * Add a global label to the registry. A global label can be declared only once.
     *
     * @param  string $sLabel
     * @return self
     * @throws Exception
     */
    public function addGlobalLabel(string $sLabel) : self {
        if (isset($this->aGlobalLabels[$sLabel])) {
            throw new \Exception(
                'Duplicate global: '    . $sLabel .
                ' already declared in ' . $this->aGlobalLabels[$sLabel][self::I_FILE] .
                ' on line ' . $this->aGlobalLabels[$sLabel][self::I_LINE]
            );
        }
        $this->aGlobalLabels[$sLabel] = [
            self::I_FILE => $this->sCurrentFilename,
            self::I_LINE => $this->iCurrentLineNumber,
            self::I_POSN => $this->iCurrentStatementPosition
        ];
        Log::printf(
            "Added global label '%s' on line %d, code position %d",
            $sLabel,
            $this->iCurrentLineNumber,
            $this->iCurrentStatementPosition
        );
        return $this;
    }

    /**
     * Add a local label to the registry. A local label can be declared only once in the current file.
     *
     * @param  string $sLabel
     * @return self
     * @throws Exception
     */
    public function addLocalLabel(string $sLabel) : self {
        if (isset($this->aLocalLabels[$this->sCurrentFilename][$sLabel])) {
            throw new \Exception(
                'Duplicate local: '     . $sLabel .
                ' already declared in ' . $this->sCurrentFilename .
                ' on line '             . $this->aLocalLabels[$this->sCurrentFilename][$sLabel][self::I_LINE]
            );
        }
        $this->aLocalLabels[$this->sCurrentFilename][$sLabel] = [
            self::I_LINE => $this->iCurrentLineNumber,
            self::I_POSN => $this->iCurrentStatementPosition
        ];
        Log::printf(
            "Added local label '%s' on line %d, code position %d",
            $sLabel,
            $this->sCurrentFilename,
            $this->iCurrentStatementPosition
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

    /**
     * @return int|null
     */
    public function getPositionForLocal(string $sLabel) : ?int {
        if (isset($this->aLocalLabels[$this->sCurrentFilename][$sLabel])) {
            return $this->aLocalLabels[$this->sCurrentFilename][$sLabel][self::I_POSN];
        }
        return null;
    }
}
