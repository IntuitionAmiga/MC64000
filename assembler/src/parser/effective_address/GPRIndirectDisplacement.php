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

namespace ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs\EffectiveAddress;
use ABadCafe\MC64K\Defs\Register;
use ABadCafe\MC64K\Defs\IIntLimits;
use ABadCafe\MC64K\Parser;

use function \preg_match, \chr, \pack;

/**
 * GPRIndirectDisplacement
 *
 * Basic parser for GPR indirect with signed displacement.
 */
class GPRIndirectDisplacement implements IParser, EffectiveAddress\IRegisterIndirect {

    use TOperationSizeAware;
    use TDisplacementSizeAware;

    private const BASE_EA = [
        self::OFS_GPR_IND_DSP8, // Base EA mode for displacements fitting into 8 bits
        self::OFS_GPR_IND_DSP   // Base EA mode for displacements fitting into 32 bits
    ];

    private const BIN_FMT = [
        IIntLimits::BYTE_BIN_FORMAT,
        IIntLimits::LONG_BIN_FORMAT
    ];

    /**
     * Required match
     */
    const MATCHES = [
        // Register indirect with signed displacement d(rN)
        '/^' . self::D32 . '\(\s*' . self::RA . '\s*\)$/',

        // Register indirect with signed displacement (d, rN)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*\)$/',
    ];

    const
        MATCHED_DISP = 1,
        MATCHED_NAME = 2
    ;

    /**
     * @inheritDoc
     */
    public function hasSideEffects(): bool {
        return false;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource): ?string {
        foreach (self::MATCHES as $sMatch) {
            if (preg_match($sMatch, $sSource, $aMatches)) {
                $iRegister     = Register\Enumerator::getGPRNumber($aMatches[self::MATCHED_NAME]);
                $iDisplacement = Parser\Utils\Integer::parseLiteral($aMatches[self::MATCHED_DISP], IIntLimits::LONG);
                $iOffset       = $this->fitsByte($iDisplacement) ? 0 : 1;
                return
                    chr(self::BASE_EA[$iOffset] + $iRegister) .
                    pack(self::BIN_FMT[$iOffset], $iDisplacement);
            }
        }
        return null;
    }

}
