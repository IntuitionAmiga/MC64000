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
 * GPRIndirectIndexedDisplacement
 *
 * Basic parser for GPR indirect modes that have an index register and signed displacement.
 */
class GPRIndirectIndexedDisplacement implements IParser, EffectiveAddress\IRegisterIndirectIndexed {

    use TOperationSizeAware;
    use TDisplacementSizeAware;

    private const BASE_EA = [
        self::OFS_GPR_IDX_DSP8 - self::OFS_GPR_IDX_DSP, // Base EA offset for displacements fitting into 8 bits
        0                                               // Base EA offset for displacements fitting into 32 bits
    ];

    private const BIN_FMT = [
        IIntLimits::BYTE_BIN_FORMAT,
        IIntLimits::LONG_BIN_FORMAT
    ];

    /**
     * Required match
     */
    const MATCHES = [
        // Register indirect with signed 8-bit index and signed displacement d32(rN, rN.b)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\)$/' => self::REG_IND_IDXB_DSP,

        // Register indirect with signed 8-bit index and signed displacement (d32, rN, rN.b)
        '/^\(\s*' . self::D32 . '\s*\,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\)$/' => self::REG_IND_IDXB_DSP,

        // Register indirect with signed 16-bit index and signed displacement d32(rN, rN.w)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\)$/' => self::REG_IND_IDXW_DSP,

        // Register indirect with signed 16-bit index and signed displacement (d32, rN, rN.w)
        '/^\(\s*' . self::D32 . '\s*\,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\)$/' => self::REG_IND_IDXW_DSP,

        // Register indirect with signed 32-bit index and signed displacement d32(rN, rN.l)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\)$/' => self::REG_IND_IDXL_DSP,

        // Register indirect with signed 32-bit index and signed displacement (d32, rN, rN.l)
        '/^\(\s*' . self::D32 . '\s*\,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\)$/' => self::REG_IND_IDXL_DSP,

        // Register indirect with signed 64-bit index and signed displacement d32(rN, rN.q)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\)$/' => self::REG_IND_IDXQ_DSP,

        // Register indirect with signed 64-bit index and signed displacement (d32, rN, rN.q)
        '/^\(\s*' . self::D32 . '\s*\,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\)$/' => self::REG_IND_IDXQ_DSP,

        // Register indirect with 16-bit scaled signed 8-bit index and signed displacement d32(rN, rN.b*2)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*2\s*\)$/' => self::REG_IND_IDXB_2_DSP,

        // Register indirect with 16-bit scaled signed 8-bit index and signed displacement (d32, rN, rN.b*2)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*2\s*\)$/' => self::REG_IND_IDXB_2_DSP,

        // Register indirect with 16-bit scaled signed 16-bit index and signed displacement d32(rN, rN.w*2)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*2\s*\)$/' => self::REG_IND_IDXW_2_DSP,

        // Register indirect with 16-bit scaled signed 16-bit index and signed displacement (d32, rN, rN.w*2)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*2\s*\)$/' => self::REG_IND_IDXW_2_DSP,

        // Register indirect with 16-bit scaled signed 32-bit index and signed displacement d32(rN, rN.l*2)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*2\s*\)$/' => self::REG_IND_IDXL_2_DSP,

        // Register indirect with 16-bit scaled signed 32-bit index and signed displacement (d32, rN, rN.l*2)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*2\s*\)$/' => self::REG_IND_IDXL_2_DSP,

        // Register indirect with 16-bit scaled signed 64-bit index and signed displacement d32(rN, rN.q*2)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*2\s*\)$/' => self::REG_IND_IDXQ_2_DSP,

        // Register indirect with 16-bit scaled signed 64-bit index and signed displacement d32(rN, rN.q*2)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*2\s*\)$/' => self::REG_IND_IDXQ_2_DSP,

        // Register indirect with 32-bit scaled signed 8-bit index and signed displacement d32(rN, rN.b*4)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*4\s*\)$/' => self::REG_IND_IDXB_4_DSP,

        // Register indirect with 32-bit scaled signed 8-bit index and signed displacement (d32, rN, rN.b*4)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*4\s*\)$/' => self::REG_IND_IDXB_4_DSP,

        // Register indirect with 32-bit scaled signed 16-bit index and signed displacement d32(rN, rN.w*4)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*4\s*\)$/' => self::REG_IND_IDXW_4_DSP,

        // Register indirect with 32-bit scaled signed 16-bit index and signed displacement d32(rN, rN.w*4)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*4\s*\)$/' => self::REG_IND_IDXW_4_DSP,

        // Register indirect with 32-bit scaled signed 32-bit index and signed displacement d32(rN, rN.l*4)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*4\s*\)$/' => self::REG_IND_IDXL_4_DSP,

        // Register indirect with 32-bit scaled signed 32-bit index and signed displacement (d32, rN, rN.l*4)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*4\s*\)$/' => self::REG_IND_IDXL_4_DSP,

        // Register indirect with 32-bit scaled signed 64-bit index and signed displacement d32(rN, rN.q*4)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*4\s*\)$/' => self::REG_IND_IDXQ_4_DSP,

        // Register indirect with 32-bit scaled signed 64-bit index and signed displacement (d32, rN, rN.q*4)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*4\s*\)$/' => self::REG_IND_IDXQ_4_DSP,

        // Register indirect with 64-bit scaled signed 8-bit index and signed displacement d32(rN, rN.b*8)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*8\s*\)$/' => self::REG_IND_IDXB_8_DSP,

        // Register indirect with 64-bit scaled signed 8-bit index and signed displacement (d32, rN, rN.b*8)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*8\s*\)$/' => self::REG_IND_IDXB_8_DSP,

        // Register indirect with 64-bit scaled signed 16-bit index and signed displacement d32(rN, rN.w*8)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*8\s*\)$/' => self::REG_IND_IDXW_8_DSP,

        // Register indirect with 64-bit scaled signed 16-bit index and signed displacement d32(rN, rN.w*8)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*8\s*\)$/' => self::REG_IND_IDXW_8_DSP,

        // Register indirect with 64-bit scaled signed 32-bit index and signed displacement d32(rN, rN.l*8)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*8\s*\)$/' => self::REG_IND_IDXL_8_DSP,

        // Register indirect with 64-bit scaled signed 32-bit index and signed displacement d32(rN, rN.l*8)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*8\s*\)$/' => self::REG_IND_IDXL_8_DSP,

        // Register indirect with 64-bit scaled signed 64-bit index and signed displacement d32(rN, rN.q*8)
        '/^' . self::D32 . '\s*\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*8\s*\)$/' => self::REG_IND_IDXQ_8_DSP,

        // Register indirect with 64-bit scaled signed 64-bit index and signed displacement d32(rN, rN.q*8)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*8\s*\)$/' => self::REG_IND_IDXQ_8_DSP,
    ];

    const
        MATCHED_BASE_NAME  = 2,
        MATCHED_INDEX_NAME = 3,
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
        foreach (self::MATCHES as $sMatch => $iBaseEAMode) {
            if (preg_match($sMatch, $sSource, $aMatches)) {
                $iRegisterPair = Register\Enumerator::getGPRNumber($aMatches[self::MATCHED_BASE_NAME]) << 4
                               | Register\Enumerator::getGPRNumber($aMatches[self::MATCHED_INDEX_NAME]);
                $iDisplacement = Parser\Utils\Integer::parseLiteral($aMatches[self::MATCHED_DISP], IIntLimits::LONG);
                $iOffset       = $this->fitsByte($iDisplacement) ? 0 : 1;
                return
                    chr($iBaseEAMode + self::BASE_EA[$iOffset]) .
                    chr($iRegisterPair) .
                    pack(self::BIN_FMT[$iOffset], $iDisplacement);
            }
        }
        return null;
    }
}
