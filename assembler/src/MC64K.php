<?php

/**
 * In the absence of an autoloader, we use a simple C-like header. Each top level include pulls in whatever is defined
 * within it's corresponding subdirectory.
 */
namespace ABadCafe\MC64K;

if (PHP_VERSION_ID < 70400) {
    throw new \RuntimeException("Requires at least PHP 7.4");
}

require_once 'classmap.php';
spl_autoload_register(function(string $str_class) {
    if (isset(CLASS_MAP[$str_class])) {
        require_once __DIR__ . CLASS_MAP[$str_class];
    }
});


