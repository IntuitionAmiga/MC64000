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

namespace ABadCafe\MC64K;

/**
 * If you aren't using 7.4, no dice.
 */
if (PHP_VERSION_ID < 70400) {
    throw new \RuntimeException("Requires at least PHP 7.4");
}

/**
 * In the absence of an autoloader, we use a simple C-like header. Each top level include pulls in whatever is defined
 * within it's corresponding subdirectory.
 */
require_once 'classmap.php';
spl_autoload_register(function(string $str_class) {
    if (isset(CLASS_MAP[$str_class])) {
        require_once __DIR__ . CLASS_MAP[$str_class];
    }
});
