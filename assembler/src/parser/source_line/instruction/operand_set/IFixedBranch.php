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

namespace ABadCafe\MC64K\Parser\SourceLine\Instruction\OperandSet;
use ABadCafe\MC64K\Defs\Mnemonic\IControl;
use ABadCafe\MC64K\Defs\IIntLimits;

/**
 * IFixedBranch
 *
 * Defines a number of encoding properties for fixed branches.
 */
interface IFixedBranch {
    const
        MAX_REVERSE = 0,
        MAX_FORWARD = 1,
        ENCODE_SIZE = 2,
        DATA_FORMAT = 3
    ;

    const RANGES = [
        IControl::BRA_B => [
            IIntLimits::BYTE_MIN_SIGNED,
            IIntLimits::BYTE_MAX_SIGNED,
            IIntLimits::BYTE,
            IIntLimits::BYTE_BIN_FORMAT
        ],
        IControl::BRA   => [
            IIntLimits::LONG_MIN_SIGNED,
            IIntLimits::LONG_MAX_SIGNED,
            IIntLimits::LONG,
            IIntLimits::LONG_BIN_FORMAT
        ]
    ];
}
