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
use ABadCafe\MC64K\Parser;

use function \preg_match, \chr;

/**
 * GPRIndirect
 *
 * Basic parser for GPR indirect modes
 */
class GPRIndirect implements IParser, EffectiveAddress\IRegisterIndirect {

    use TOperationSizeAware;

    /**
     * Required match
     */
    const MATCH        = '/^\(\s*' . self::RA . '\s*\)$/';
    const MATCHED_NAME = 1;

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
        if (preg_match(self::MATCH, $sSource, $aMatches)) {
            $sRegister = $aMatches[self::MATCHED_NAME];
            if (!isset(Register\INames::GPR_MAP[$sRegister])) {
                throw new \OutOfBoundsException($sRegister . ' is not a recognised GPR name');
            }
            return chr(self::OFS_GPR_IND + Register\INames::GPR_MAP[$sRegister]);
        }
        return null;
    }
}
