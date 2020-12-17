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

namespace ABadCafe\MC64K\Parser\EffectiveAddress;

/**
 * IMatch
 *
 * Common regex motifs
 */
interface IMatch {

    const
        // Matches a displacement value, either a signed decimal or 0x prefixed hexadecimal literal
        D32 = '([\+\-]{0,1}\d+|(0x)[0-9A-Fa-f]+)',

        // Matches an address register qualification (rN or aN)
        RA  = '([ra]\d+|sp)',

        // Matches any integer register qualification (rN, aN or dN | sp)
        RDA = '([rda]\d+|sp)'
    ;
}
