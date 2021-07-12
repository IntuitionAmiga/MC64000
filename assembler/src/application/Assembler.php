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

namespace ABadCafe\MC64K\Application;
use ABadCafe\MC64K\Project;
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Process;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\IO;
use ABadCafe\MC64K\Utils\Log;

/**
 * Assembler
 *
 * Main application
 */
class Assembler {

    private Project\Definition $oProject;

    private Parser\SourceLine\Processor $oParser;

    /**
     * Constructor
     *
     * @throws \Exception
     */
    public function __construct() {
        $this->oParser  = new Parser\SourceLine\Processor();
    }

    /**
     * Load up the project file
     *
     * @param  string $sProjectFile
     * @throws \Exception
     */
    public function loadProject(string $sProjectFile) : self {
        $this->oProject = new Project\Definition($sProjectFile);
        State\Coordinator::get()
            ->setGlobalOptions($this->oProject->getOptions())
            ->setGlobalDefinitionSet($this->oProject->getDefinitionSet());
        return $this;
    }

    /**
     * Execute the first pass.
     *
     * @return self (fluent)
     * @throws \Exception
     */
    public function firstPass() : self {
        foreach ($this->oProject->getSourceList() as $sSourceFile) {
            $this->processSourceFile(new IO\SourceFile($sSourceFile));
        }
        return $this;
    }

    /**
     * Execute the second pass.
     *
     * @return self (fluent)
     * @throws \Exception
     */
    public function secondPass() : self {
        $oState = State\Coordinator::get();
        $oSecondPass = new Process\SecondPass();
        $oSecondPass->resolveForwardsBranchReferences(
            $oState->getLabelLocation(),
            $oState->getOutput()
        );
        return $this;
    }

    /**
     * Write the compiled output
     *
     * @return self (fluent)
     * @throws \Exception
     */
    public function writeBinary() : self {
        $oWriter = new IO\Output\Binary(
            $this->oProject->getBaseDirectoryPath() . $this->oProject->getOutputBinaryPath()
        );
        $oState = State\Coordinator::get();
        $oWriter->writeChunk($oState->getOutput());
        $oWriter->writeChunk(new IO\Output\ExportList($oState->getLabelLocation()));
        return $this;
    }

    /**
     * Load and parse a single source file
     *
     * @param  IO\SourceFile $oSource
     * @throws \Exception
     */
    private function processSourceFile(IO\SourceFile $oSource) : void {
        $oOutput = State\Coordinator::get()
            ->setCurrentFile($oSource)
            ->getOutput();
        while ( ($sSourceLine = $oSource->readLine()) ) {
            $oOutput->appendStatement($this->oParser->parse($sSourceLine));
        }
    }
}
