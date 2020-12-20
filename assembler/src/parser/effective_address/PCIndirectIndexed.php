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
 * PCIndirectIndexed
 *
 * PC Relative modes with index register.
 */
class PCIndirectIndexed implements IParser, EffectiveAddress\IRegisterIndirectIndexed {

    use TOperationSizeAware;

    /**
     * Required match
     */
    const MATCHES = [
        // Program counter with signed 8-bit index (pc, rN.b)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\)$/' => self::PC_IND_IDXB,

        // Program counter with signed 16-bit index (pc, rN.w)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\)$/' => self::PC_IND_IDXW,

        // Program counter with signed 32-bit index (pc, rN.l)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\)$/' => self::PC_IND_IDXL,

        // Program counter with signed 64-bit index (pc, rN.q)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\)$/' => self::PC_IND_IDXQ,

        // Program counter with 16-bit scaled signed 8-bit index (pc, rN.b*2)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*2\s*\)$/' => self::PC_IND_IDXB_2,

        // Program counter with 16-bit scaled signed 16-bit index (pc, rN.w*2)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*2\s*\)$/' => self::PC_IND_IDXW_2,

        // Program counter with 16-bit scaled signed 32-bit index (pc, rN.l*2)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*2\s*\)$/' => self::PC_IND_IDXL_2,

        // Program counter with 16-bit scaled signed 64-bit index (pc, rN.q*2)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*2\s*\)$/' => self::PC_IND_IDXQ_2,

        // Program counter with 32-bit scaled signed 8-bit index (pc, rN.b*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*4\s*\)$/' => self::PC_IND_IDXB_4,

        // Program counter with 32-bit scaled signed 16-bit index (pc, rN.w*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*4\s*\)$/' => self::PC_IND_IDXW_4,

        // Program counter with 32-bit scaled signed 32-bit index (pc, rN.l*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*4\s*\)$/' => self::PC_IND_IDXL_4,

        // Program counter with 32-bit scaled signed 64-bit index (pc, rN.q*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*4\s*\)$/' => self::PC_IND_IDXQ_4,

        // Program counter with 32-bit scaled signed 8-bit index (pc, rN.b*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*8\s*\)$/' => self::PC_IND_IDXB_8,

        // Program counter with 32-bit scaled signed 16-bit index (pc, rN.w*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*8\s*\)$/' => self::PC_IND_IDXW_8,

        // Program counter with 32-bit scaled signed 32-bit index (pc, rN.l*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*8\s*\)$/' => self::PC_IND_IDXL_8,

        // Program counter with 32-bit scaled signed 64-bit index (pc, rN.q*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*8\s*\)$/' => self::PC_IND_IDXQ_8,
    ];

    const
        MATCHED_INDEX_NAME = 1
    ;

    /**
     * @inheritDoc
     */
    public function parse(string $sSource) : ?string {
        foreach (self::MATCHES as $sMatch => $iOffset) {
            if (preg_match($sMatch, $sSource, $aMatches)) {
                $iIndex = Register\Enumerator::getGPRNumber($aMatches[self::MATCHED_INDEX_NAME]);
                return chr($iOffset) . chr($iIndex);
            }
        }
        return null;
    }
}
