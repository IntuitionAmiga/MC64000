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
use ABadCafe\MC64K\Project;
use ABadCafe\MC64K\Utils\Log;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\IO;

/**
 * Coordinator
 *
 * DSG Antipattern: Dirty Great Singleton
 *
 * It may not be the right tool for most things, but here, it makes some sense.
 */
class Coordinator {

    private static ?self $oInstance = null; // Singleton

    private Options        $oGlobalOptions, $oOptions;
    private DefinitionSet  $oGlobalDefinitionSet, $oDefinitionSet;
    private IO\ISourceFile $oCurrentFile;
    private LabelLocation  $oLabelLocation;
    private Output         $oOutput;

    /**
     * Constructor
     */
    private function __construct() {
        $this->oGlobalOptions       =
        $this->oOptions             = new Options();
        $this->oGlobalDefinitionSet =
        $this->oDefinitionSet       = new DefinitionSet();
        $this->oCurrentFile         = new IO\SourceString('', 'none');
        $this->oLabelLocation       = new LabelLocation();
        $this->oOutput              = new Output();
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
     * @return Options
     */
    public function getOptions() : Options {
        return $this->oOptions;
    }

    /**
     * Returns the current DefinitionSet. This has scope for the current file only and can
     * undefine and redefine anything in the global set.
     *
     * @return DefinitionSet
     */
    public function getDefinitionSet() : DefinitionSet {
        return $this->oDefinitionSet;
    }

    /**
     * Set the global definition set. Used during Project import.
     *
     * @param  DefinitionSet $oDefinitionSet
     * @return self (fluent)
     */
    public function setGlobalDefinitionSet(DefinitionSet $oDefinitionSet) : self {
        $this->oGlobalDefinitionSet = $oDefinitionSet;
        return $this;
    }

    /**
     * Set the global options. Used during Project import.
     *
     * @param  Options $oOptions
     * @return self
     */
    public function setGlobalOptions(Options $oOptions) : self {
        $this->oGlobalOptions = $oOptions;
        return $this;
    }

    /**
     * Set the file to work on.
     *
     * @param  IO\ISourceFile $oFile
     * @return self
     */
    public function setCurrentFile(IO\ISourceFile $oFile) : self {
        if ($oFile !== $this->oCurrentFile) {
            $this->oCurrentFile   = $oFile;
            $this->oOptions       = clone $this->oGlobalOptions;
            $this->oDefinitionSet = clone $this->oGlobalDefinitionSet;
        }
        return $this;
    }

    /**
     * @return IO\ISourceFile
     */
    public function getCurrentFile() : IO\ISourceFile {
        return $this->oCurrentFile;
    }

    /**
     * @return Output
     */
    public function getOutput() : Output {
        return $this->oOutput;
    }

    /**
     * @return LabelLocation
     */
    public function getLabelLocation() : LabelLocation {
        return $this->oLabelLocation;
    }

    /**
     * Simple trampoline
     */
    public function setCurrentStatementLength(int $iLength) : self {
        $this->oOutput->setCurrentStatementLength($iLength);
        return $this;
    }

    /**
     * Simple trampoline
     */
    public function adjustCurrentStatementLength(int $iDelta) : self {
        $this->oOutput->adjustCurrentStatementLength($iDelta);
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
        $this->oLabelLocation->addGlobal(
            $this->oCurrentFile,
            $sLabel,
            $this->oOutput->getCurrentStatementPosition()
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
        $this->oLabelLocation->addLocal(
            $this->oCurrentFile,
            $sLabel,
            $this->oOutput->getCurrentStatementPosition()
        );
        return $this;
    }

    /**
     * @param  string $sLabel
     * @return int
     */
    public function getBranchDisplacementForLabel(string $sLabel) : int {
        $iPosition = $this->getPositionForLabel($sLabel);
        if (null !== $iPosition) {
            $iDisplacement = $this->oOutput->getDisplacmentForPosition($iPosition);
            if ($this->oOptions->isEnabled(Defs\Project\IOptions::LOG_LABEL_RESOLVE)) {
                Log::printf(
                    "Resolved %s to displacement %d [%d - %d - %d]",
                    $sLabel,
                    $iDisplacement,
                    $iPosition,
                    $this->oOutput->getCurrentStatementPosition(),
                    $this->oOutput->getCurrentStatementLength()
                );
            }
            return $iDisplacement;
        }
        return Defs\IBranchLimits::UNRESOLVED_DISPLACEMENT;
    }

    /**
     * @param  string $sLabel
     * @return int|null
     */
    public function getPositionForLabel(string $sLabel) : ?int {
        return (Defs\ILabel::LOCAL_PREFIX_CHAR === $sLabel[0]) ?
            $this->oLabelLocation->getPositionForLocal($this->oCurrentFile, $sLabel) :
            $this->oLabelLocation->getPositionForGlobal($sLabel);
    }

    /**
     * @param  string $sLabel
     * @return self
     */
    public function addUnresolvedLabel(string $sLabel) : self {
        $iLocation = $this->oOutput->getCurrentOffset() - Defs\IBranchLimits::DISPLACEMENT_SIZE;
        $this->oLabelLocation->addUnresolved(
            $this->oCurrentFile,
            $sLabel,
            $iLocation
        );
        return $this;
    }

}
