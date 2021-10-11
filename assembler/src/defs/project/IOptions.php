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

namespace ABadCafe\MC64K\Defs\Project;

/**
 * IOptions
 *
 * Defines constants for project options
 */
interface IOptions {

    const
        TYPE_ANY    = 0,
        TYPE_BOOL   = 1,
        TYPE_INT    = 2,
        TYPE_FLOAT  = 3,
        TYPE_STRING = 4
    ;

    const
        LOG_CODE_FOLD     = 'log_code_folds',
        LOG_LABEL_ADD     = 'log_label_add',
        LOG_LABEL_REF     = 'log_label_ref',
        LOG_LABEL_RESOLVE = 'log_label_resolve',
        LOG_LABEL_IMPORT  = 'log_label_import',
        LOG_LABEL_IEMODE  = 'log_label_iemode',
        LOG_CHUNK_INFO    = 'log_chunk_info',
        OPT_USE_FAST_PATH = 'opt_use_fast_path',
        APP_STACK_SIZE    = 'app_stack_size'
    ;

    const TYPE_MAP = [
        self::LOG_CODE_FOLD     => self::TYPE_BOOL,
        self::LOG_LABEL_ADD     => self::TYPE_BOOL,
        self::LOG_LABEL_REF     => self::TYPE_BOOL,
        self::LOG_LABEL_RESOLVE => self::TYPE_BOOL,
        self::LOG_LABEL_IMPORT  => self::TYPE_BOOL,
        self::LOG_LABEL_IEMODE  => self::TYPE_BOOL,
        self::OPT_USE_FAST_PATH => self::TYPE_BOOL,
        self::APP_STACK_SIZE    => self::TYPE_INT
    ];

    const DEFAULTS = [
        self::OPT_USE_FAST_PATH => true,
        self::APP_STACK_SIZE    => 256
    ];
}
