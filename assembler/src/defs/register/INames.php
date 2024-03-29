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

namespace ABadCafe\MC64K\Defs\Register;

/**
 * INames
 *
 * Defines and maps allowed Register names and aliases.
 */
interface INames {

    const GPR_MAP = [
        // GPR names
        'r0'   =>  0, 'r1'   =>  1, 'r2'  =>  2,  'r3'   =>  3,
        'r4'   =>  4, 'r5'   =>  5, 'r6'  =>  6,  'r7'   =>  7,
        'r8'   =>  8, 'r9'   =>  9, 'r10' => 10,  'r11'  => 11,
        'r12'  => 12, 'r13'  => 13, 'r14' => 14,  'r15'  => 15,

        // 68K data register aliases
        'd0'   =>  0, 'd1'   =>  1, 'd2'  =>  2,  'd3'   =>  3,
        'd4'   =>  4, 'd5'   =>  5, 'd6'  =>  6,  'd7'   =>  7,

        // 68K address register aliases
        'a0'   =>  8, 'a1'   =>  9, 'a2'  => 10,  'a3'   => 11,
        'a4'   => 12, 'a5'   => 13, 'a6'  => 14,  'a7'   => 15,
        'sp'   => 15,
    ];

    const FPR_MAP = [
        // FPR names
        'fp0'  =>  0, 'fp1'  =>  1, 'fp2'  =>  2, 'fp3'  =>  3,
        'fp4'  =>  4, 'fp5'  =>  5, 'fp6'  =>  6, 'fp7'  =>  7,
        'fp8'  =>  8, 'fp9'  =>  9, 'fp10' => 10, 'fp11' => 11,
        'fp12' => 12, 'fp13' => 13, 'fp14' => 14, 'fp15' => 15,
    ];
}
