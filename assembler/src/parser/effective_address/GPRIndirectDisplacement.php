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

/**
 * GPRIndirect
 *
 * Basic parser for GPR indirect modes, including pre/post increment/decrement
 */
class GPRIndirectDisplacement implements IParser, EffectiveAddress\IRegisterIndirect {

    use TOperationSizeAware;
    use TSignedDisplacementAware;

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
    public function parse(string $sSource) : ?string {
        foreach (self::MATCHES as $sMatch => $iOffset) {
            if (preg_match($sMatch, $sSource, $aMatches)) {
                $sRegister = $aMatches[self::MATCHED_NAME];
                if (!isset(Register\INames::GPR_MAP[$sRegister])) {
                    throw new \OutOfBoundsException($sRegister . ' is not a recognised GPR name');
                }

                $iDisplacement = $this->parseDisplacement(
                    $aMatches[self::MATCHED_DISP],
                    !empty($aMatches[self::MATCHED_HEX])
                );

                return chr($iOffset + Register\INames::GPR_MAP[$sRegister]) . pack('V', $iDisplacement);
            }
        }
        return null;
    }

    private function parseHexDisplacement(string $sDisplacement) : int {
        $sDisplacement = substr($sDisplacement, 2);
        if (strlen($sDisplacement) <= 8) {
            return Parser\Utils\Hex::stringToInt32($sDisplacement);
        }
        throw new \RangeError('Displacement ' . $sDisplacement . ' is too large');
    }

    private function parseDecimalDisplacement(string $sDisplacement) : int {
        $iDisplacement = (int)$sDisplacement;
        if ($iDisplacement >= -2147483648 && $iDisplacement <= 2147483647) {
            return $iDisplacement;
        }
        throw new \RangeError('Displacement ' . $sDisplacement . ' is too large');
    }
}
