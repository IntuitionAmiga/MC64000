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

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction;

/**
 * CodeFoldException
 *
 * Exception raised where code can be folded. The intention of a code folding exception is to be thrown deep in a parsing layer
 * where an alternative set of instructions to those being assembled has been identified as a more efficient code path. This is
 * usually the elimination of code that has no effect. The exception then bubbles up through the parser heirarchy and is caught
 * at the bytecode generation phase which may decide to use it instead of the instruction under evaluation.
 */
class CodeFoldException extends \Exception {

    protected string $sAlternative;

    public function __construct(string $sMessage, string $sAlternative = '') {
        parent::__construct($sMessage);
        $this->sAlternative = $sAlternative;
    }

    /**
     * Get the alternative bytecode
     *
     * @return string
     */
    public final function getAlternativeBytecode(): string {
        return $this->sAlternative;
    }
}
