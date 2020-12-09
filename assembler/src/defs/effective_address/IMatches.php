<?php

namespace ABadCafe\MC64K\Defs\EffectiveAddress;

/**
 * Effective Adddress Modes
 *
 * Defines a set of regular expression rules for matching trimmed Effective Address strings
 */
interface IMatches extends IModes {

    const
        // Matches a displacement value, either a signed decimal or 0x prefixed hexadecimal literal
        D32 = '([\+\-]{0,1}\d+|0x[0-9A-Fa-f]+)',

        // Matches an address register qualification (rN or aN)
        RA  = '([ra]\d+)',

        // Matches any integer register qualification (rN, aN or dN)
        RDA = '([rda]\d+)'
    ;

    /**
     *
     */
    const MATCHES = [

        // Signed integer literal #d8 / #d16 / #d32 / #d64
        '/^#' . self::D32 . '$/' => self::INT_LIT,

        // Integer literal, hex representation #0x...
        //'/^#(0x[0-9A-F]+)$/' => self::INT_LIT,

        // Register direct rN
        '/^[rda]\d+$/' => self::REG,

        // Floating point direct fpN
        '/^fp\d+$/' => self::REG_FLT,

        //////////////////////////////////////////////////////////////////

        // Register indirect (rN)
        '/^\(\s*' . self::RA . '\s*\)$/' => self::REG_IND,

        // Register indirect, post increment (rN)+
        '/^\(\s*' . self::RA . '\s*\)\+$/' => self::REG_IND_POST_INC,

        // Register indirect, post decrement (rN)-
        '/^\(\s*' . self::RA . '\s*\)\-$/' => self::REG_IND_POST_DEC,

        // Register indirect, pre increment +(rN)
        '/^\+\(\s*' . self::RDA . '\s*\)$/' => self::REG_IND_PRE_INC,

        // Register indirect, pre decrement -(rN)
        '/^\-\(\s*' . self::RDA . '\s*\)$/' => self::REG_IND_PRE_DEC,

        // Register indirect with signed displacement d32(rN)
        '/^' . self::D32 . '\(\s*' . self::RA . '\s*\)$/' => self::REG_IND_DSP,

        // Register indirect with signed displacement (d32, rN)
        '/^\(\s*' . self::D32 . '\s*,\s*' . self::RA . '\s*\)$/' => self::REG_IND_DSP,

        //////////////////////////////////////////////////////////////////


        // Register indirect with signed 8-bit index (rN, rN.b)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\)$/' => self::REG_IND_IDXB,

        // Register indirect with signed 16-bit index (rN, rN.w)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\)$/' => self::REG_IND_IDXW,

        // Register indirect with signed 32-bit index (rN, rN.l)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\)$/' => self::REG_IND_IDXL,

        // Register indirect with signed 64-bit index (rN, rN.q)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\)$/' => self::REG_IND_IDXQ,


        //////////////////////////////////////////////////////////////////

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

        //////////////////////////////////////////////////////////////////

        // Register indirect with 16-bit scaled signed 8-bit index (rN, rN.b*2)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*2\s*\)$/' => self::REG_IND_IDXB_2,

        // Register indirect with 16-bit scaled signed 16-bit index (rN, rN.w*2)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*2\s*\)$/' => self::REG_IND_IDXW_2,

        // Register indirect with 16-bit scaled signed 32-bit index (rN, rN.l*2)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*2\s*\)$/' => self::REG_IND_IDXL_2,

        // Register indirect with 16-bit scaled signed 64-bit index (rN, rN.q*2)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*2\s*\)$/' => self::REG_IND_IDXQ_2,

        //////////////////////////////////////////////////////////////////

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

        //////////////////////////////////////////////////////////////////

        // Register indirect with 32-bit scaled signed 8-bit index and signed displacement d32(rN, rN.b*4)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*4\s*\)$/' => self::REG_IND_IDXB_4,

        // Register indirect with 32-bit scaled signed 16-bit index and signed displacement d32(rN, rN.w*4)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*4\s*\)$/' => self::REG_IND_IDXW_4,

        // Register indirect with 32-bit scaled signed 32-bit index and signed displacement d32(rN, rN.l*4)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*4\s*\)$/' => self::REG_IND_IDXL_4,

        // Register indirect with 32-bit scaled signed 64-bit index and signed displacement d32(rN, rN.q*4)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*4\s*\)$/' => self::REG_IND_IDXQ_4,

        //////////////////////////////////////////////////////////////////

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

        //////////////////////////////////////////////////////////////////

        // Register indirect with 64-bit scaled signed 8-bit index (rN, rN.b*8)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.b\s*\*\s*8\s*\)$/' => self::REG_IND_IDXB_8,

        // Register indirect with 64-bit scaled signed 16-bit index (rN, rN.w*8)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.w\s*\*\s*8\s*\)$/' => self::REG_IND_IDXW_8,

        // Register indirect with 64-bit scaled signed 32-bit index (rN, rN.l*8)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.l\s*\*\s*8\s*\)$/' => self::REG_IND_IDXL_8,

        // Register indirect with 64-bit scaled signed 64-bit index (rN, rN.q*8)
        '/^\(\s*' . self::RA . '\s*,\s*' . self::RDA . '\.q\s*\*\s*8\s*\)$/' => self::REG_IND_IDXQ_8,

        //////////////////////////////////////////////////////////////////

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

        //////////////////////////////////////////////////////////////////

        // Program counter with signed displacement d32(pc)
        '/^' . self::D32 . '\(\s*pc\s*\)$/' => self::PC_IND_DSP,

        // Program counter with signed displacement (d32, pc)
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*\)$/' => self::PC_IND_DSP,

        //////////////////////////////////////////////////////////////////

        // Program counter with signed 8-bit index (pc, rN.b)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\)$/' => self::PC_IND_IDXB,

        // Program counter with signed 16-bit index (pc, rN.w)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\)$/' => self::PC_IND_IDXW,

        // Program counter with signed 32-bit index (pc, rN.l)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\)$/' => self::PC_IND_IDXL,

        // Program counter with signed 64-bit index (pc, rN.q)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\)$/' => self::PC_IND_IDXQ,

        //////////////////////////////////////////////////////////////////

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

        //////////////////////////////////////////////////////////////////

        // Program counter with 16-bit scaled signed 8-bit index (pc, rN.b*2)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*2\s*\)$/' => self::PC_IND_IDXB_2,

        // Program counter with 16-bit scaled signed 16-bit index (pc, rN.w*2)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*2\s*\)$/' => self::PC_IND_IDXW_2,

        // Program counter with 16-bit scaled signed 32-bit index (pc, rN.l*2)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*2\s*\)$/' => self::PC_IND_IDXL_2,

        // Program counter with 16-bit scaled signed 64-bit index (pc, rN.q*2)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*2\s*\)$/' => self::PC_IND_IDXQ_2,

        //////////////////////////////////////////////////////////////////

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

        //////////////////////////////////////////////////////////////////

        // Program counter with 32-bit scaled signed 8-bit index (pc, rN.b*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*4\s*\)$/' => self::PC_IND_IDXB_4,

        // Program counter with 32-bit scaled signed 16-bit index (pc, rN.w*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*4\s*\)$/' => self::PC_IND_IDXW_4,

        // Program counter with 32-bit scaled signed 32-bit index (pc, rN.l*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*4\s*\)$/' => self::PC_IND_IDXL_4,

        // Program counter with 32-bit scaled signed 64-bit index (pc, rN.q*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*4\s*\)$/' => self::PC_IND_IDXQ_4,

        //////////////////////////////////////////////////////////////////

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


        //////////////////////////////////////////////////////////////////

        // Program counter with 32-bit scaled signed 8-bit index (pc, rN.b*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*4\s*\)$/' => self::PC_IND_IDXB_4,

        // Program counter with 32-bit scaled signed 16-bit index (pc, rN.w*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*4\s*\)$/' => self::PC_IND_IDXW_4,

        // Program counter with 32-bit scaled signed 32-bit index (pc, rN.l*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*4\s*\)$/' => self::PC_IND_IDXL_4,

        // Program counter with 32-bit scaled signed 64-bit index (pc, rN.q*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*4\s*\)$/' => self::PC_IND_IDXQ_4,

        //////////////////////////////////////////////////////////////////

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


        //////////////////////////////////////////////////////////////////

        // Program counter with 32-bit scaled signed 8-bit index (pc, rN.b*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.b\s*\*\s*8\s*\)$/' => self::PC_IND_IDXB_8,

        // Program counter with 32-bit scaled signed 16-bit index (pc, rN.w*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.w\s*\*\s*8\s*\)$/' => self::PC_IND_IDXW_8,

        // Program counter with 32-bit scaled signed 32-bit index (pc, rN.l*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.l\s*\*\s*8\s*\)$/' => self::PC_IND_IDXL_8,

        // Program counter with 32-bit scaled signed 64-bit index (pc, rN.q*4)
        '/^\(\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*8\s*\)$/' => self::PC_IND_IDXQ_8,

        //////////////////////////////////////////////////////////////////

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
        '/^\(\s*' . self::D32 . '\s*,\s*pc\s*,\s*' . self::RDA . '\.q\s*\*\s*8\s*\)$/' => self::PC_IND_IDXQ_8_DSP,

        // Absolute address, 16-bit
        '/^\d+\.w$/'                  => self::ABS_W,
        '/^0x[0-9A-Fa-f]{1, 4}\.w$/'  => self::ABS_W,

        // Absolute address, 32-bit
        '/^\d+\.l$/'                  => self::ABS_L,
        '/^0x[0-9A-Fa-f]{1, 8}\.l$/'  => self::ABS_L,

        // Absolute address 64-bit
        '/^\d+\.q$/'                  => self::ABS_Q,
        '/^0x[0-9A-Fa-f]{1, 16}\.q$/' => self::ABS_L,
    ];
}
