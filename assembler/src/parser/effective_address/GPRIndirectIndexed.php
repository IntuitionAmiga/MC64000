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
 * GPRIndirectIndexed
 *
 * Basic parser for GPR indirect modes that have an index register.
 */
class GPRIndirectIndexed implements IParser, EffectiveAddress\IRegisterIndirectIndexed {

    use TOperationSizeAware;

    /**
     * Required match
     */
    const MATCHES = [
        // Register indirect with signed 8-bit index (rN, rN.b)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\)$/' => self::REG_IND_IDXB,

        // Register indirect with signed 16-bit index (rN, rN.w)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\)$/' => self::REG_IND_IDXW,

        // Register indirect with signed 32-bit index (rN, rN.l)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\)$/' => self::REG_IND_IDXL,

        // Register indirect with signed 64-bit index (rN, rN.q)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\)$/' => self::REG_IND_IDXQ,

        // Register indirect with 16-bit scaled signed 8-bit index (rN, rN.b*2)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*2\s*\)$/' => self::REG_IND_IDXB_2,

        // Register indirect with 16-bit scaled signed 16-bit index (rN, rN.w*2)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*2\s*\)$/' => self::REG_IND_IDXW_2,

        // Register indirect with 16-bit scaled signed 32-bit index (rN, rN.l*2)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*2\s*\)$/' => self::REG_IND_IDXL_2,

        // Register indirect with 16-bit scaled signed 64-bit index (rN, rN.q*2)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*2\s*\)$/' => self::REG_IND_IDXQ_2,

        // Register indirect with 32-bit scaled signed 8-bit index and signed displacement d32(rN, rN.b*4)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*4\s*\)$/' => self::REG_IND_IDXB_4,

        // Register indirect with 32-bit scaled signed 16-bit index and signed displacement d32(rN, rN.w*4)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*4\s*\)$/' => self::REG_IND_IDXW_4,

        // Register indirect with 32-bit scaled signed 32-bit index and signed displacement d32(rN, rN.l*4)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*4\s*\)$/' => self::REG_IND_IDXL_4,

        // Register indirect with 32-bit scaled signed 64-bit index and signed displacement d32(rN, rN.q*4)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*4\s*\)$/' => self::REG_IND_IDXQ_4,

        // Register indirect with 64-bit scaled signed 8-bit index (rN, rN.b*8)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*8\s*\)$/' => self::REG_IND_IDXB_8,

        // Register indirect with 64-bit scaled signed 16-bit index (rN, rN.w*8)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*8\s*\)$/' => self::REG_IND_IDXW_8,

        // Register indirect with 64-bit scaled signed 32-bit index (rN, rN.l*8)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*8\s*\)$/' => self::REG_IND_IDXL_8,

        // Register indirect with 64-bit scaled signed 64-bit index (rN, rN.q*8)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*8\s*\)$/' => self::REG_IND_IDXQ_8,
    ];

    const
        MATCHED_BASE_NAME  = 1,
        MATCHED_INDEX_NAME = 2
    ;

    /**
     * @inheritDoc
     */
    public function hasSideEffects() : bool {
        return false;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource) : ?string {
        foreach (self::MATCHES as $sMatch => $iOffset) {
            if (preg_match($sMatch, $sSource, $aMatches)) {
                $iData = $this->getRegisterNumber($aMatches[self::MATCHED_BASE_NAME]) << 4 |
                         $this->getRegisterNumber($aMatches[self::MATCHED_INDEX_NAME]);
                return chr($iOffset) . chr($iData);
            }
        }
        return null;
    }

    /**
     * @param  string
     * @return int
     */
    private function getRegisterNumber(string $sRegister) : int {
        if (!isset(Register\INames::GPR_MAP[$sRegister])) {
            throw new \OutOfBoundsException($sRegister . ' is not a recognised GPR name');
        }
        return Register\INames::GPR_MAP[$sRegister];
    }
}
