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

namespace ABadCafe\MC64K\Project;

use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Utils\Log;

/**
 * Definition
 *
 * Loads and parses a basic project definition file.
 */
class Definition {

    private string
        $sBaseDirectory,
        $sName,
        $sDescription,
        $sOutputBinary
    ;

    /**
     * @var string[] $aSourceFiles
     */
    private array $aSourceFiles;

    private State\DefinitionSet $oDefinitionSet;

    /**
     * Constructor - must be given a valid project file definition.
     *
     * @param string $sProjectFile
     */
    public function __construct(string $sProjectFile) {
        $this->oDefinitionSet = new State\DefinitionSet();
        $this->load($sProjectFile);
    }

    /**
     * @return string
     */
    public function getBaseDirectoryPath() : string {
        return $this->sBaseDirectory;
    }

    /**
     * @return string
     */
    public function getOutputBinaryPath() : string {
        return $this->sOutputBinary;
    }

    /**
     * @return string[]
     */
    public function getSourceList() : array {
        return $this->aSourceFiles;
    }

    /**
     * Return the definition set (could be empty)
     */
    public function getDefinitionSet() : State\DefinitionSet {
        return $this->oDefinitionSet;
    }

    /**
     * Loads up a given project file, or throws an exception if this is not possible.
     * The source file json document that must contain at least:
     *
     * Project name
     * Output binary name
     * List of Sources
     *
     * @param string $sProjectFile
     *
     */
    public function load(string $sProjectFile) : self {
        Log::printf("Attempting to open project file %s...", $sProjectFile);
        if (
            empty($sProjectFile) ||
            !file_exists($sProjectFile) ||
            !is_readable($sProjectFile)
        ) {
            throw new \Exception("Unable to open project file '" . $sProjectFile . "' for reading.\n");
        }
        $oProjectData = json_decode(file_get_contents($sProjectFile));
        if (
            !($oProjectData instanceof \stdClass) ||
            empty($oProjectData->name) ||
            empty($oProjectData->output) ||
            empty($oProjectData->sources) ||
            !is_countable($oProjectData->sources)
        ) {
            throw new \Exception("Invalid project file '" . $sProjectFile . "'");
        }

        $sBaseDirectory       = realpath(dirname($sProjectFile)) . '/';
        $this->aSourceFiles   = array_map(
            function(string $sSourcePath) use ($sBaseDirectory) {
                $sSourcePath = $sBaseDirectory . $sSourcePath;
                if (
                    !file_exists($sSourcePath) ||
                    !is_readable($sSourcePath)
                ) {
                    throw new \Exception("Invalid source path '" . $sSourcePath . "'");
                }
                return $sSourcePath;
            },
            $oProjectData->sources
        );
        $this->sBaseDirectory = $sBaseDirectory;
        $this->sName          = (string)$oProjectData->name;
        $this->sDescription   = (string)($oProjectData->description ?? '');
        $this->sOutputBinary  = (string)$oProjectData->output;

        if (!empty($oProjectData->options) && (
            is_object($oProjectData->options) ||
            is_array($oProjectData->options)
        )) {
            $oState = State\Coordinator::get();
            foreach ((array)$oProjectData->options as $sOption => $mValue) {
                $oState->setOption($sOption, $mValue);
            }
        }

        if (!empty($oProjectData->define) && (
            is_object($oProjectData->define) ||
            is_array($oProjectData->define)
        )) {
            foreach ((array)$oProjectData->define as $sDefine => $sValue) {
                $this->oDefinitionSet->add($sDefine, (string)$sValue);
            }
        }

        Log::printf(
            "Project file %s loaded successfully:\n\tName:   %s\n\tInfo:   %s\n\tOutput: %s\n\tFiles:  %d",
            $sProjectFile,
            $this->sName,
            $this->sDescription,
            $this->sOutputBinary,
            count($this->aSourceFiles)
        );

        return $this;
    }
}
