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

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction\Operand;
use ABadCafe\MC64K\Parser\SourceLine\Instruction\UnhandledCodeFoldException;
use ABadCafe\MC64K;
use ABadCafe\MC64K\Parser;

/**
 * BranchDisplacement
 */
class BranchDisplacement implements MC64K\IParser {

    use Parser\Utils\TSignedDisplacementAware;

    const UNRESOLVED = 0;

    const
        MATCH_NUMERIC = '/^#' . Parser\EffectiveAddress\IParser::D32 . '$/',
        MATCH_LABEL   = '/^(\.){0,1}[a-zA-Z_]+$/',
        MATCHED_LABEL = 0,
        MATCHED_LOCAL = 1,
        MATCHED_DISP  = 1,
        MATCHED_HEX   = 2
    ;

    private int $iLastDisplacement = self::UNRESOLVED;

    /**
     * @inheritDoc
     *
     * If the resolved displacement value is zero (branch to next instruction)
     * the code will be folded out.
     *
     * @throws CodeFoldException
     */
    public function parse(string $sSource) : ?string {
        $this->iLastDisplacement = self::UNRESOLVED;
        if (preg_match(self::MATCH_NUMERIC, $sSource, $aMatches)) {
            $this->iLastDisplacement = $this->parseDisplacement(
                $aMatches[self::MATCHED_DISP],
                isset($aMatches[self::MATCHED_HEX])
            );

            if (0 === $this->iLastDisplacement) {
                throw new UnhandledCodeFoldException('Branch has no effect');
            }

            return pack('V', $this->iLastDisplacement);
        }
        // TODO - handle labels
        if (preg_match(self::MATCH_LABEL, $sSource, $aMatches)) {
            return pack('V', self::UNRESOLVED);
        }

        throw new \UnexpectedValueException($sSource . ' could not be parsed');
    }

    /**
     * Returns the integer value of the most recently processed branch displacement.
     *
     * @return int
     */
    public function getLastDisplacement() : int {
        return $this->iLastDisplacement;
    }
}
