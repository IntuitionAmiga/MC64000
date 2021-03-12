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
use ABadCafe\MC64K\Defs;

/**
 * Output
 *
 * Bytecode output
 */
class Output {

    private string $sBytecode = '';

    private int
        $iCurrentStatementPosition = 0,
        $iCurrentStatementLength   = 0
    ;

    public function getBytecode() : string {
        return $this->sBytecode;
    }

    /**
     * Add a complete bytecode statement to the buffer.
     * Resets the current statement position and length properties
     *
     * @param  string $sBytecode
     * @return self
     */
    public function appendStatement(string $sBytecode) : self {
        $this->iCurrentStatementPosition += strlen($sBytecode);
        $this->iCurrentStatementLength   = 0;
        $this->sBytecode .= $sBytecode;
        return $this;
    }

    /**
     * Returns the current internal statement position.
     *
     * @return int
     */
    public function getCurrentStatementPosition() : int {
        return $this->iCurrentStatementPosition;
    }

    /**
     * Returns the current internal statement length.
     *
     * @return int
     */
    public function getCurrentStatementLength() : int {
        return $this->iCurrentStatementLength;
    }

    /**
     * Sets the current internal statement length, used to track the size of the bytecode of the operation so far.
     *
     * @param  int $iStatementLength
     * @return self
     */
    public function setCurrentStatementLength(int $iStatementLength) : self {
        $this->iCurrentStatementLength = $iStatementLength;
        return $this;
    }

    /**
     * Returns the displacement for a given position.
     *
     * @param  int $iPosition
     * @return int
     */
    public function getDisplacmentForPosition(int $iPosition) : int {
        return $iPosition - $this->iCurrentStatementPosition - $this->iCurrentStatementLength;
    }

    /**
     * Returns the current offset into the bytecode.
     *
     * @return int
     */
    public function getCurrentOffset() : int {
        return $this->iCurrentStatementPosition + $this->iCurrentStatementLength;
    }
}
