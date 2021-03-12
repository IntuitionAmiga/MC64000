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
use ABadCafe\MC64K\IO;

/**
 * LabelLocation
 *
 * Responsible for keeping track of where labels are declared and referenced.
 */
class LabelLocation {

    const
        I_NAME = 3,
        I_FILE = 2,
        I_LINE = 1,
        I_OFST = 0
    ;

    private array  $aGlobalLabels     = [];
    private array  $aLocalLabels      = [];
    private array  $aUnresolvedLabels = [];

    /**
     * Add a global label to the registry. A global label can be declared only once.
     *
     * @param  IO\ISourceFile $oFile
     * @param  string         $sLabel
     * @param  int            $iOffset
     * @return self
     * @throws \Exception
     */
    public function addGlobal(IO\ISourceFile $oFile, string $sLabel, int $iOffset) : self {
        if (isset($this->aGlobalLabels[$sLabel])) {
            throw new \Exception(
                'Duplicate global: '    . $sLabel .
                ' already declared in ' . $this->aGlobalLabels[$sLabel][self::I_FILE] .
                ' on line ' . $this->aGlobalLabels[$sLabel][self::I_LINE]
            );
        }
        $iLine = $oFile->getLineNumber();
        $this->aGlobalLabels[$sLabel] = [
            self::I_FILE => $oFile->getFilename(),
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
     * Add a local label to the registry. A local label can be declared only once in given file.
     *
     * @param  IO\File $oFile
     * @param  string  $sLabel
     * @param  int     $iOffset
     * @return self
     * @throws \Exception
     */
    public function addLocal(IO\ISourceFile $oFile, string $sLabel, int $iOffset) : self {
        $sCurrentFile = $oFile->getFilename();
        if (isset($this->aLocalLabels[$sCurrentFile][$sLabel])) {
            throw new \Exception(
                'Duplicate local: '     . $sLabel .
                ' already declared in ' . $sCurrentFile .
                ' on line '             . $this->aLocalLabels[$CurrentFile][$sLabel][self::I_LINE]
            );
        }
        $iLine = $oFile->getLineNumber();
        $this->aLocalLabels[$sCurrentFile][$sLabel] = [
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

    /**
     * Obtains the offset for a locally declared label (if known) in the specified file.
     *
     * @param  IO\ISourceFile $oFile
     * @param  string         $sLabel
     * @return int|null
     */
    public function getPositionForLocal(IO\ISourceFile $oFile, string $sLabel) : ?int {
        $sFile = $oFile->getFilename();
        if (isset($this->aLocalLabels[$sFile][$sLabel])) {
            Log::printf(
                "Resolved local label '%s' to bytecode position %d",
                $sLabel,
                $this->aLocalLabels[$sFile][$sLabel][self::I_OFST]
            );
            return $this->aLocalLabels[$sFile][$sLabel][self::I_OFST];
        }
        return null;
    }

    /**
     * Obtains the offset for a globally declared label (if known)
     *
     * @param  string $sLabel
     * @return int|null
     */
    public function getPositionForGlobal(string $sLabel) : ?int {
        if (isset($this->aGlobalLabels[$sLabel])) {
            Log::printf(
                "Resolved label '%s' to bytecode position %d",
                $sLabel,
                $this->aGlobalLabels[$sLabel][self::I_POSN]
            );
            return $this->aGlobalLabels[$sLabel][self::I_OFST];
        }
        return null;
    }

    /**
     * Records an instance of an unresolved label reference.
     *
     * @param  IO\ISourceFile $oFile
     * @param  string         $sLabel
     * @parma  int            $iLocation
     * @return self
     */
    public function addUnresolved(IO\ISourceFile $oFile, string $sLabel, int $iLocation) : self {
        $sCurrentFilename   = $oFile->getFilename();
        $iCurrentLineNumber = $oFile->getLineNumber();
        if (isset($this->aUnresolvedLabels[$sCurrentFilename][$sLabel][$iCurrentLineNumber])) {
            throw new \Exception("Duplicate unresolved label reference to same line in same file");
        }
        Log::printf(
            "Recorded reference to unresolved label '%s' at bytecode position %d",
            $sLabel,
            $iLocation
        );
        $this->aUnresolvedLabels[$sCurrentFilename][$sLabel][$iCurrentLineNumber] = $iLocation;
        return $this;
    }
}
