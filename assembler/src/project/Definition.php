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

use function \file_exists, \is_readable, \count, \array_map, \is_object, \is_array;
use function \json_decode, \file_get_contents, \is_countable, \realpath, \dirname;

/**
 * Definition
 *
 * Loads and parses a basic project definition file.
 */
class Definition {

    private string
        $sLibraryPath,
        $sBaseDirectory,
        $sName,
        $sDescription,
        $sOutputBinary
    ;

    /**
     * @var string[] $aSourceFiles
     */
    private array $aSourceFiles;

    private State\Options       $oOptions;
    private State\DefinitionSet $oDefinitionSet;
    private State\Target        $oTarget;

    /**
     * Constructor - must be given a valid project file definition.
     *
     * @param string $sProjectFile
     */
    public function __construct(string $sProjectFile, string $sLibraryPath) {
        $this->sLibraryPath   = $sLibraryPath;
        $this->oOptions       = new State\Options();
        $this->oDefinitionSet = new State\DefinitionSet();
        $this->load($sProjectFile);
    }

    /**
     * @return string
     */
    public function getBaseDirectoryPath(): string {
        return $this->sBaseDirectory;
    }

    /**
     * @return string
     */
    public function getOutputBinaryPath(): string {
        return $this->sOutputBinary;
    }

    /**
     * @return string[]
     */
    public function getSourceList(): array {
        return $this->aSourceFiles;
    }

    /**
     * Return the target definition
     *
     * @return State\Target
     */
    public function getTarget(): State\Target {
        return $this->oTarget;
    }

    /**
     * Return the definition set (could be empty)
     *
     * @return State\DefinitionSet
     */
    public function getDefinitionSet(): State\DefinitionSet {
        return $this->oDefinitionSet;
    }

    /**
     * Return the options (could be empty)#*
     *
     * @return State\Options
     */
    public function getOptions(): State\Options {
        return $this->oOptions;
    }

    /**
     * Loads up a given project file, or throws an exception if this is not possible.
     * The source file json document that must contain at least:
     *
     * Project name
     * Output binary name
     * List of Sources
     *
     * @param  string $sProjectFile
     * @return self (fluent)
     */
    public function load(string $sProjectFile): self {
        Log::printf("Attempting to open project file %s...", $sProjectFile);

        $oProjectData = $this->loadDefinition($sProjectFile);

        $this->processTarget($oProjectData);
        $this->processSources($oProjectData);
        $this->processOptions($oProjectData);
        $this->processDefines($oProjectData);

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

    /**
     * @param object $oProjectData
     */
    private function processTarget(object $oProjectData): void {
        $this->oTarget = new State\Target(
            (string)$oProjectData->target->name,
            (string)$oProjectData->target->version
        );
        if (isset($oProjectData->target->host)) {
            if (
                empty($oProjectData->target->host->name) ||
                empty($oProjectData->target->host->version)
            ) {
                throw new \Exception('Host section must not be empty');
            }
            $this->oTarget
                ->setFlags(State\Target::F_EXECUTABLE)
                ->getDependencySet()
                ->add(
                    (string)$oProjectData->target->host->name,
                    (string)$oProjectData->target->host->version
                );
        }
    }

    /**
     * @param object $oProjectData
     */
    private function processSources(object $oProjectData): void {
        $this->aSourceFiles = array_map(
            function(string $sSourcePath) {

                if (preg_match('/^lib\:/', $sSourcePath)) {
                    $sSourcePath = $this->sLibraryPath . substr($sSourcePath, 4);
                } else {
                    $sSourcePath = $this->sBaseDirectory . $sSourcePath;
                }
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
    }

    /**
     * @param object $oProjectData
     */
    private function processOptions(object $oProjectData): void {
        if (!empty($oProjectData->options) && (
            is_object($oProjectData->options) ||
            is_array($oProjectData->options)
        )) {
            $this->oOptions->import((array)$oProjectData->options);
        }
    }

    /**
     * @param object $oProjectData
     */
    private function processDefines(object $oProjectData): void {
        if (!empty($oProjectData->defines) && (
            is_object($oProjectData->defines) ||
            is_array($oProjectData->defines)
        )) {
            foreach ((array)$oProjectData->defines as $sDefine => $sValue) {
                $this->oDefinitionSet->add($sDefine, (string)$sValue);
            }
        }
    }

    /**
     * @param  string $sProjectFile
     * @return object
     */
    private function loadDefinition(string $sProjectFile): object {
        if (
            empty($sProjectFile) ||
            !file_exists($sProjectFile) ||
            !is_readable($sProjectFile)
        ) {
            throw new \Exception("Unable to open project file '" . $sProjectFile . "' for reading.\n");
        }
        $oProjectData = json_decode((string)file_get_contents($sProjectFile));
        if (
            !($oProjectData instanceof \stdClass) ||
            empty($oProjectData->target->name) ||
            empty($oProjectData->target->version) ||
            empty($oProjectData->target->output) ||
            empty($oProjectData->sources) ||
            !is_countable($oProjectData->sources)
        ) {
            throw new \Exception("Invalid structure in project file '" . $sProjectFile . "'");
        }
        $this->sBaseDirectory = realpath(dirname($sProjectFile)) . '/';
        $this->sName          = (string)$oProjectData->target->name;
        $this->sDescription   = (string)($oProjectData->target->description ?? '');
        $this->sOutputBinary  = (string)$oProjectData->target->output;
        return $oProjectData;
    }
}
