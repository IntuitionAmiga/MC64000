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
    use Parser\Utils\TSignedDisplacementAware;

    /**
     * Required match
     */
    const MATCHES = [
        // Register indirect with signed displacement d32(rN)
        '/^' . self::D32 . '\(\s*' . self::RA . '\s*\)$/' => self::OFS_GPR_IND_DSP,

        // Register indirect with signed displacement (d32, rN)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*\)$/' => self::OFS_GPR_IND_DSP,
    ];

    const
        MATCHED_DISP = 1,
        MATCHED_HEX  = 2,
        MATCHED_NAME = 3
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
        foreach (self::MATCHES as $sMatch => $iOffset) {
            if (preg_match($sMatch, $sSource, $aMatches)) {
                $iRegister = Register\Enumerator::getGPRNumber($aMatches[self::MATCHED_NAME]);
                $iDisplacement = $this->parseDisplacement(
                    $aMatches[self::MATCHED_DISP],
                    !empty($aMatches[self::MATCHED_HEX])
                );
                return chr($iOffset + $iRegister) . pack(IIntLimits::LONG_BIN_FORMAT, $iDisplacement);
            }
        }
        return null;
    }
}
