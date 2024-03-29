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
 * PCIndirectIndexedDisplacement
 *
 * PC Relative mode with index register and displacement.
 */
class PCIndirectIndexedDisplacement implements IParser, EffectiveAddress\IRegisterIndirectIndexed {

    use TOperationSizeAware;

    /**
     * Required match
     */
    const MATCHES = [
        // Program counter with signed 8-bit index and signed displacement d32(pc, rN.b)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\)$/' => self::PC_IND_IDXB_DSP,

        // Program counter with signed 8-bit index and signed displacement (d32, pc, rN.b)
        '/^\(\s*' . self::D32 . '\s*\,\s*pc\s*,\s*' . self::RDA . '\.b\s*\)$/' => self::PC_IND_IDXB_DSP,

        // Program counter with signed 16-bit index and signed displacement d32(pc, rN.w)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\)$/' => self::PC_IND_IDXW_DSP,

        // Program counter with signed 16-bit index and signed displacement (d32, pc, rN.w)
        '/^\(\s*' . self::D32 . '\s*\,\s*pc\s*,\s*' . self::RDA . '\.w\s*\)$/' => self::PC_IND_IDXW_DSP,

        // Program counter with signed 32-bit index and signed displacement d32(pc, rN.l)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\)$/' => self::PC_IND_IDXL_DSP,

        // Program counter with signed 32-bit index and signed displacement (d32, pc, rN.l)
        '/^\(\s*' . self::D32 . '\s*\,\s*pc\s*,\s*' . self::RDA . '\.l\s*\)$/' => self::PC_IND_IDXL_DSP,

        // Program counter with signed 64-bit index and signed displacement d32(pc, rN.q)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\)$/' => self::PC_IND_IDXQ_DSP,

        // Program counter with signed 64-bit index and signed displacement (d32, pc, rN.q)
        '/^\(\s*' . self::D32 . '\s*\,\s*pc\s*,\s*' . self::RDA . '\.q\s*\)$/' => self::PC_IND_IDXQ_DSP,

        // Program counter with 16-bit scaled signed 8-bit index and signed displacement d32(pc, rN.b*2)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*2\s*\)$/' => self::PC_IND_IDXB_2_DSP,

        // Program counter with 16-bit scaled signed 8-bit index and signed displacement (d32, pc, rN.b*2)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*2\s*\)$/' => self::PC_IND_IDXB_2_DSP,

        // Program counter with 16-bit scaled signed 16-bit index and signed displacement d32(pc, rN.w*2)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*2\s*\)$/' => self::PC_IND_IDXW_2_DSP,

        // Program counter with 16-bit scaled signed 16-bit index and signed displacement (d32, pc, rN.w*2)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*2\s*\)$/' => self::PC_IND_IDXW_2_DSP,

        // Program counter with 16-bit scaled signed 32-bit index and signed displacement d32(pc, rN.l*2)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*2\s*\)$/' => self::PC_IND_IDXL_2_DSP,

        // Program counter with 16-bit scaled signed 32-bit index and signed displacement (d32, pc, rN.l*2)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*2\s*\)$/' => self::PC_IND_IDXL_2_DSP,

        // Program counter with 16-bit scaled signed 64-bit index and signed displacement d32(pc, rN.q*2)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*2\s*\)$/' => self::PC_IND_IDXQ_2_DSP,

        // Program counter with 16-bit scaled signed 64-bit index and signed displacement d32(pc, rN.q*2)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*2\s*\)$/' => self::PC_IND_IDXQ_2_DSP,

        // Program counter with 32-bit scaled signed 8-bit index and signed displacement d32(pc, rN.b*2)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*4\s*\)$/' => self::PC_IND_IDXB_4_DSP,

        // Program counter with 32-bit scaled signed 8-bit index and signed displacement (d32, pc, rN.b*2)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*4\s*\)$/' => self::PC_IND_IDXB_4_DSP,

        // Program counter with 32-bit scaled signed 16-bit index and signed displacement d32(pc, rN.w*2)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*4\s*\)$/' => self::PC_IND_IDXW_4_DSP,

        // Program counter with 32-bit scaled signed 16-bit index and signed displacement (d32, pc, rN.w*2)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*4\s*\)$/' => self::PC_IND_IDXW_4_DSP,

        // Program counter with 32-bit scaled signed 32-bit index and signed displacement d32(pc, rN.l*2)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*4\s*\)$/' => self::PC_IND_IDXL_4_DSP,

        // Program counter with 32-bit scaled signed 32-bit index and signed displacement (d32, pc, rN.l*2)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*4\s*\)$/' => self::PC_IND_IDXL_4_DSP,

        // Program counter with 32-bit scaled signed 64-bit index and signed displacement d32(pc, rN.q*2)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*4\s*\)$/' => self::PC_IND_IDXQ_4_DSP,

        // Program counter with 32-bit scaled signed 64-bit index and signed displacement d32(pc, rN.q*2)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*4\s*\)$/' => self::PC_IND_IDXQ_4_DSP,

        // Program counter with 64-bit scaled signed 8-bit index and signed displacement d32(pc, rN.b*8)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*8\s*\)$/' => self::PC_IND_IDXB_8_DSP,

        // Program counter with 64-bit scaled signed 8-bit index and signed displacement (d32, pc, rN.b*8)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*8\s*\)$/' => self::PC_IND_IDXB_8_DSP,

        // Program counter with 64-bit scaled signed 16-bit index and signed displacement d32(pc, rN.w*8)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*8\s*\)$/' => self::PC_IND_IDXW_8_DSP,

        // Program counter with 64-bit scaled signed 16-bit index and signed displacement (d32, pc, rN.w*8)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*8\s*\)$/' => self::PC_IND_IDXW_8_DSP,

        // Program counter with 64-bit scaled signed 32-bit index and signed displacement d32(pc, rN.l*8)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*8\s*\)$/' => self::PC_IND_IDXL_8_DSP,

        // Program counter with 64-bit scaled signed 32-bit index and signed displacement (d32, pc, rN.l*8)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*8\s*\)$/' => self::PC_IND_IDXL_8_DSP,

        // Program counter with 64-bit scaled signed 64-bit index and signed displacement d32(pc, rN.q*8)
        '/^' . self::D32 . '\s*\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*8\s*\)$/' => self::PC_IND_IDXQ_8_DSP,

        // Program counter with 64-bit scaled signed 64-bit index and signed displacement d32(pc, rN.q*8)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*8\s*\)$/' => self::PC_IND_IDXQ_8_DSP
    ];

    const
        MATCHED_INDEX_NAME = 2,
        MATCHED_DISP       = 1
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
                $iIndex = Register\Enumerator::getGPRNumber($aMatches[self::MATCHED_INDEX_NAME]);
                $iDisplacement = Parser\Utils\Integer::parseLiteral($aMatches[self::MATCHED_DISP], IIntLimits::LONG);
                return chr($iOffset) . chr($iIndex) . pack(IIntLimits::LONG_BIN_FORMAT, $iDisplacement);
            }
        }
        return null;
    }
}
