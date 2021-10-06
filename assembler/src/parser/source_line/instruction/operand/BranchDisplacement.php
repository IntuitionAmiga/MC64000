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
use ABadCafe\MC64K\Parser\SourceLine\Instruction\CodeFoldException;
use ABadCafe\MC64K\Parser\SourceLine\Instruction\UnhandledCodeFoldException;
use ABadCafe\MC64K;
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\State;

use function \preg_match, \pack;

/**
 * BranchDisplacement
 *
 * Operand parser for branch targets.
 */
class BranchDisplacement implements MC64K\IParser {

    private const
        MATCH_LABEL   = '/^(\.){0,1}[a-zA-Z_]{1}[0-9a-zA-Z_]{0,}$/',
        MATCHED_LABEL = 0,
        MATCHED_LOCAL = 1
    ;

    private int $iLastDisplacement = Defs\IBranchLimits::UNRESOLVED_DISPLACEMENT;

    /**
     * @inheritDoc
     *
     * If the resolved displacement value is zero (branch to next instruction)
     * the code will be folded out.
     *
     * @throws CodeFoldException
     */
    public function parse(string $sSource): ?string {
        $this->iLastDisplacement = Defs\IBranchLimits::UNRESOLVED_DISPLACEMENT;

        $int_displacement = $this->parseIntegerDisplacement($sSource);
        if (null !== $int_displacement) {
            $this->iLastDisplacement = $int_displacement;
            if (0 === $this->iLastDisplacement) {
                throw new CodeFoldException('Branch to next instruction has no useful effect');
            }
            return pack(Defs\IIntLimits::LONG_BIN_FORMAT, $this->iLastDisplacement);
        }

        // TODO - handle labels
        if (preg_match(self::MATCH_LABEL, $sSource, $aMatches)) {
            $this->iLastDisplacement = State\Coordinator::get()
               ->getBranchDisplacementForLabel($aMatches[self::MATCHED_LABEL]);
            return pack(Defs\IIntLimits::LONG_BIN_FORMAT, $this->iLastDisplacement);
        }
        throw new \UnexpectedValueException($sSource . ' could not be parsed');
    }

    /**
     * @return bool
     */
    public function wasUnresolved(): bool {
        return Defs\IBranchLimits::UNRESOLVED_DISPLACEMENT === $this->iLastDisplacement;
    }

    /**
     * Returns the integer value of the most recently processed branch displacement.
     *
     * @return int
     */
    public function getLastDisplacement(): int {
        return $this->iLastDisplacement;
    }

   /**
    * @param  string $sDisplacement
    * @return int|null
    */
   private function parseIntegerDisplacement(string $sDisplacement): ?int {
       $aMatches = Parser\Utils\Integer::match($sDisplacement, Defs\IInteger::LITERAL_PREFIX);
       if (empty($aMatches)) {
           return null;
       }
       return Parser\Utils\Integer::parseMatch($aMatches, Defs\IIntLimits::LONG);
    }

}
