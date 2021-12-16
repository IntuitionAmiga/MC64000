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
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Utils\Log;
use ABadCafe\MC64K\IO;

use function \sprintf, \strlen, \array_flip, \str_split, \implode, \array_keys;

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
        I_OFST = 0,

        IE_READ  = 1,
        IE_WRITE = 2,
        IE_CALL  = 4
    ;

    /** @var (string|int)[][] $aGlobalLabels */
    private array $aGlobalLabels         = [];

    /** @var (string|int)[][][] $aLocalLabels */
    private array $aLocalLabels          = [];

    /** @var (string|int)[][][] $aUnresolvedLabels */
    private array $aUnresolvedLabels     = [];

    /** @var string[] $aExportedLabels */
    private array $aExportedLabels       = [];

    /** @var \stdClass[][] $aImportedLabels */
    private array $aImportedLabels       = [];

    /** @var string[] $aEnumeratedImports */
    private array $aEnumeratedImports    = [];

    /** @var int[] $aLabelIEQualification */
    private array $aLabelIEQualification = [];

    /** @const string[] IE_MODES */
    const IE_MODES = [
        '---', // Not really valid.
        'r--',
        '-w-',
        'rw-',
        '--x',
        'r-x',
        '-wx', // Danger!
        'rwx'  // Danger!
    ];

    /**
     * Register a label for export. This will be sanity checked in the second pass.
     *
     * Silently accepts multiple references for the same export but will throw an exception if
     * trying to mark for export any label already marked for import.
     *
     * @param  string $sLabel
     * @param  string $sIEQualification - Import / Export qualification: any of r|w|x
     * @return self
     * @throws \Exception
     */
    public function registerExport(string $sLabel, string $sIEQualification): self {
        $this->assertLabel($sLabel);
        if (isset($this->aImportedLabels[$sLabel])) {
            throw new \Exception("Label '" . $sLabel . "' is already registered as an import");
        }
        $this->aExportedLabels[$sLabel] = $sLabel;
        $this->addIEQualification($sLabel, $this->parseIEQualification($sIEQualification));
        return $this;
    }

    /**
     * Register a label for import. This will be sanity checked in the second pass.
     *
     * Silently accepts multiple references for the same import but will throw an exception if
     * trying to mark for import any label already marked for export.
     *
     * @param  string $sLabel
     * @param  string $sIEQualification - Import / Export qualification: any of r|w|x
     * @return self
     * @throws \Exception
     */
    public function registerImport(string $sLabel, string $sIEQualification): self {
        $this->assertLabel($sLabel);
        if (isset($this->aExportedLabels[$sLabel])) {
            throw new \Exception("Label '" . $sLabel . "' is already registered as an export");
        }
        if (!isset($this->aImportedLabels[$sLabel])) {
            $this->aImportedLabels[$sLabel] = [];
            if (Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_LABEL_IMPORT)) {
                Log::printf(
                    "Added imported label '%s'",
                    $sLabel
                );
            }
        }
        $this->addIEQualification($sLabel, $this->parseIEQualification($sIEQualification));
        return $this;
    }


    /**
     * Add a global label to the registry. A global label can be declared only once.
     *
     * @param  IO\ISourceFile $oFile
     * @param  string         $sLabel
     * @param  int            $iOffset
     * @return self
     * @throws \RuntimeException
     */
    public function addGlobal(IO\ISourceFile $oFile, string $sLabel, int $iOffset): self {
        $this->assertLabel($sLabel);
        if (isset($this->aGlobalLabels[$sLabel])) {
            throw new \RuntimeException(
                "Duplicate global: '"    . $sLabel .
                "' already declared in " . IO\SourceFile::shortenFilename((string)$this->aGlobalLabels[$sLabel][self::I_FILE]) .
                ' on line ' . $this->aGlobalLabels[$sLabel][self::I_LINE]
            );
        }
        $iLine = $oFile->getLineNumber();
        $this->aGlobalLabels[$sLabel] = [
            self::I_FILE => $oFile->getFilename(),
            self::I_LINE => $iLine,
            self::I_OFST => $iOffset
        ];
        if (Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_LABEL_ADD)) {
            Log::printf(
                "Added global label '%s' on line %d of %s, BCP #%d",
                $sLabel,
                $iLine,
                $oFile->getShortFilename(),
                $iOffset
            );
        }
        return $this;
    }

    /**
     * Add a local label to the registry. A local label can be declared only once in given file.
     *
     * @param  IO\ISourceFile $oFile
     * @param  string         $sLabel
     * @param  int            $iOffset
     * @return self
     * @throws \RuntimeException
     */
    public function addLocal(IO\ISourceFile $oFile, string $sLabel, int $iOffset): self {
        $this->assertLabel($sLabel);
        $sCurrentFile = $oFile->getFilename();
        if (isset($this->aLocalLabels[$sCurrentFile][$sLabel])) {
            throw new \RuntimeException(
                "Duplicate local: '"     . $sLabel .
                "' already declared in " . IO\SourceFile::shortenFilename($sCurrentFile) .
                ' on line '              . $this->aLocalLabels[$sCurrentFile][$sLabel][self::I_LINE]
            );
        }
        $iLine = $oFile->getLineNumber();
        $this->aLocalLabels[$sCurrentFile][$sLabel] = [
            self::I_LINE => $iLine,
            self::I_OFST => $iOffset
        ];
        if (Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_LABEL_ADD)) {
            Log::printf(
                "Added local label '%s' on line %d, BCP #%d",
                $sLabel,
                $iLine,
                $iOffset
            );
        }
        return $this;
    }

    /**
     * Returns the global symbol table.
     *
     * @return mixed[][] [string => [string, string, int]]
     */
    public function getGlobals(): array {
        return $this->aGlobalLabels;
    }

    /**
     * Returns whether or not a given global label name is currently resolved.
     *
     * @param  string $sLabel
     * @return bool
     */
    public function isGlobalResolved(string $sLabel): bool {
        return isset($this->aGlobalLabels[$sLabel]);
    }

    /**
     * Obtains the offset for a locally declared label (if known) in the specified file.
     *
     * @param  IO\ISourceFile $oFile
     * @param  string         $sLabel
     * @return int|null
     */
    public function getPositionForLocal(IO\ISourceFile $oFile, string $sLabel): ?int {
        $sFile = $oFile->getFilename();
        if (isset($this->aLocalLabels[$sFile][$sLabel])) {
            if (Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_LABEL_RESOLVE)) {
                Log::printf(
                    "Resolved local label '%s' to BCP #%d",
                    $sLabel,
                    $this->aLocalLabels[$sFile][$sLabel][self::I_OFST]
                );
            }
            return (int)$this->aLocalLabels[$sFile][$sLabel][self::I_OFST];
        }
        return null;
    }

    /**
     * Obtains the offset for a globally declared label (if known).
     *
     * @param  string $sLabel
     * @return int|null
     */
    public function getPositionForGlobal(string $sLabel): ?int {
        if (isset($this->aGlobalLabels[$sLabel])) {
            if (Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_LABEL_RESOLVE)) {
                Log::printf(
                    "Resolved label '%s' to BCP #%d",
                    $sLabel,
                    $this->aGlobalLabels[$sLabel][self::I_OFST]
                );
            }
            return (int)$this->aGlobalLabels[$sLabel][self::I_OFST];
        }
        return null;
    }

    /**
     * Records an instance of an unresolved label reference.
     *
     * @param  IO\ISourceFile $oFile
     * @param  string         $sLabel
     * @param  int            $iLocation
     * @return self
     */
    public function addUnresolved(IO\ISourceFile $oFile, string $sLabel, int $iLocation): self {
        $this->assertLabel($sLabel);
        $sCurrentFilename   = $oFile->getFilename();
        $iCurrentLineNumber = $oFile->getLineNumber();
        if (isset($this->aUnresolvedLabels[$sCurrentFilename][$sLabel][$iCurrentLineNumber])) {
            throw new \Exception("Duplicate unresolved label reference to same line in same file");
        }
        if (Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_LABEL_ADD)) {
            Log::printf(
                "Recorded reference to unresolved label '%s' at BCP #%d",
                $sLabel,
                $iLocation
            );
        }
        $this->aUnresolvedLabels[$sCurrentFilename][$sLabel][$iCurrentLineNumber] = $iLocation;
        return $this;
    }

    /**
     * Add a reference to a declared import. This will throw an exception if the supplied label is not
     * a declared import.
     *
     * @param  IO\ISourceFile $oFile
     * @param  string         $sLabel
     * @param  int            $iLocation
     * @return self
     * @throws \Exception
     */
    public function addImportReference(IO\ISourceFile $oFile, string $sLabel, int $iLocation): self {
        $this->assertLabel($sLabel);

        if (!isset($this->aImportedLabels[$sLabel])) {
            throw new \Exception("Reference to undeclared imported label");
        }

        $sCurrentFilename   = $oFile->getFilename();
        $iCurrentLineNumber = $oFile->getLineNumber();
        if (Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_LABEL_ADD)) {
            Log::printf(
                "Recorded reference to imported label '%s' at BCP #%d",
                $sLabel,
                $iLocation
            );
        }
        $this->aImportedLabels[$sLabel][] = (object)[
            'sFilename'   => $sCurrentFilename,
            'iLineNumber' => $iCurrentLineNumber,
            'iLocation'   => $iLocation
        ];
        return $this;
    }

    /**
     * Resolves any branch targets that were unresolved at the time of generation during the first pass.
     *
     * Throws if an unresolved refrence cannot be resolved.
     *
     * @return \stdClass[] {int, string, string, int}[]
     * @throws \Exception
     */
    public function resolveBranchTargetList(): array {
        $aResult = [];
        foreach ($this->aUnresolvedLabels as $sCurrentFilename => $aUnresolvedMap) {
            foreach ($aUnresolvedMap as $sLabel => $aUnresolvedLocation) {
                if (Defs\ILabel::LOCAL_PREFIX_CHAR === $sLabel[0]) {
                    if (!isset($this->aLocalLabels[$sCurrentFilename][$sLabel])) {
                        throw new \Exception(
                            sprintf("Reference to undeclared local label %s in %s", $sLabel, $sCurrentFilename)
                        );
                    }
                    $oTarget = (object)[
                        'iLabelPosition' => $this->aLocalLabels[$sCurrentFilename][$sLabel][self::I_OFST],
                        'sLabel'         => $sLabel,
                        'sFilename'      => $sCurrentFilename,
                        'iLineNumber'    => $this->aLocalLabels[$sCurrentFilename][$sLabel][self::I_LINE]
                    ];
                } else {
                    if (!isset($this->aGlobalLabels[$sLabel])) {
                        throw new \Exception(
                            sprintf(
                                "Reference to undeclared global label %s in %s, line(s): %s",
                                $sLabel,
                                $sCurrentFilename,
                                implode(', ', array_keys($aUnresolvedLocation))
                            )
                        );
                    }
                    $oTarget = (object)[
                        'iLabelPosition' => $this->aGlobalLabels[$sLabel][self::I_OFST],
                        'sLabel'         => $sLabel,
                        'sFilename'      => $sCurrentFilename,
                        'iLineNumber'    => $this->aGlobalLabels[$sLabel][self::I_LINE]
                    ];
                }

                foreach ($aUnresolvedLocation as $iLineNumber => $iReferencePosition) {
                    $aResult[$iReferencePosition] = $oTarget;
                }
            }
        }
        return $aResult;
    }

    /**
     * Returns the set of exported labels.
     *
     * @return \stdClass[] {string, int, string}[]
     */
    public function resolveExports(): array {
        $aResult = [];
        foreach ($this->aExportedLabels as $sLabel) {
            if (!isset($this->aGlobalLabels[$sLabel])) {
                throw new \Exception(
                    sprintf("Exported global label %s has not been defined", $sLabel)
                );
            }
            $aResult[] = (object)[
                'sLabel'         => $sLabel,
                'iLabelPosition' => $this->aGlobalLabels[$sLabel][self::I_OFST],
                'sFilename'      => $this->aGlobalLabels[$sLabel][self::I_FILE]
            ];
        }
        return $aResult;
    }

    /**
     * Set the array of imports that were enumerated in the second pass.
     *
     * @param  string[] $aImports
     * @return self
     */
    public function setEnumeratedImports(array $aImports): self {
        $this->aEnumeratedImports = $aImports;
        return $this;
    }

    /**
     * Get the array of imports that were enumerated in the second pass.
     *
     * @return string[]
     */
    public function getEnumeratedImports(): array {
        return $this->aEnumeratedImports;
    }

    /**
     * Returns whether or not a given label is declared as an import.
     *
     * @param  string $sLabel
     * @return bool
     */
    public function isDefinedImport(string $sLabel): bool {
        return isset($this->aImportedLabels[$sLabel]);
    }

    /**
     * Returns the (pre-enumerated) set of imports.
     *
     * @return \stdClass[][]
     */
    public function getImports(): array {
        return $this->aImportedLabels;
    }

    /**
     * Returns the set of Import/Export qualification flags for all the encountered global labels.
     * This does not differentiate between import and export as the same identifier name cannot be used
     * for both simultaneously.
     *
     * The return format is a label-keyed array of integers containing a bit set for each of the r/w/x behaviours.
     *
     * @return int[] - keyed by label.
     */
    public function getImportExportQualifications(): array {
        return $this->aLabelIEQualification;
    }

    /**
     * Asserts if a label is valid. For now this is just a length check.
     *
     * @param   string $sLabel
     * @@throws \LengthException|\InvalidArgumentException
     */
    private function assertLabel(string $sLabel): void {
        if (strlen($sLabel) > Defs\ILabel::MAX_LENGTH) {
            throw new \LengthException("Label identifer '" . $sLabel . "' too long");
        }
        if (
            isset(Defs\Register\INames::GPR_MAP[$sLabel]) ||
            isset(Defs\Register\INames::FPR_MAP[$sLabel])
        ) {
            throw new \InvalidArgumentException(
                "Register name '" . $sLabel .
                "' used in a context that expects a label"
            );
        }
    }

    /**
     * Return the integer bitmask representation of the import/export qualification string.
     *
     * @param  string $sIEQualification
     * @return int
     */
    private function parseIEQualification(string $sIEQualification): int {
        $aFlags = array_flip(str_split($sIEQualification, 1));
        return
            (isset($aFlags[Defs\ILabel::IE_READ])  ? self::IE_READ  : 0) |
            (isset($aFlags[Defs\ILabel::IE_WRITE]) ? self::IE_WRITE : 0) |
            (isset($aFlags[Defs\ILabel::IE_CALL])  ? self::IE_CALL  : 0)
        ;
    }

    /**
     * @param string $sLabel
     * @param int    $iIEQualification
     */
    private function addIEQualification(string $sLabel, int $iIEQualification): void {
        if (isset($this->aLabelIEQualification[$sLabel])) {
            $iOldIEQualification = $this->aLabelIEQualification[$sLabel];
            $iNewIEQualification = $iOldIEQualification | $iIEQualification;
            if ($iNewIEQualification !== $iOldIEQualification) {

                if (Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_LABEL_IEMODE)) {
                    Log::printf(
                        "Change of access scope on label '%s' from %s to %s",
                        $sLabel,
                        self::IE_MODES[$iOldIEQualification],
                        self::IE_MODES[$iNewIEQualification]
                    );
                }

                $this->aLabelIEQualification[$sLabel] = $iNewIEQualification;
            }
        } else {
            $this->aLabelIEQualification[$sLabel] = $iIEQualification;
        }
    }
}
