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
use ABadCafe\MC64K\IO;

use function \sprintf;

/**
 * SourceError
 */
class SourceError extends \Exception {

    /**
     * Repackage parsing exceptions
     *
     * @param IO\SourceFile $oSource
     * @param \Throwable    $oError
     */
    public function __construct(IO\SourceFile $oSource, \Throwable $oError) {
        $iLine = $oSource->getLineNumber();
        if ($iLine < 2) {
            parent::__construct(
                sprintf(
                    "Error in file '%s':\n\n\tLine %4d : %s\n\tProblem   : %s",
                    $oSource->getFilename(),
                    $iLine,
                    $oSource->getLine(),
                    $oError->getMessage()
                )
            );
        } else {
            parent::__construct(
                sprintf(
                    "Error in file '%s':\n\n\tLine %4d : %s\tLine %4d : %s\n\tProblem   : %s",
                    $oSource->getFilename(),
                    $iLine - 1,
                    $oSource->getPrecedingLine(),
                    $iLine,
                    $oSource->getLine(),
                    $oError->getMessage()
                )
            );
        }
    }
}

