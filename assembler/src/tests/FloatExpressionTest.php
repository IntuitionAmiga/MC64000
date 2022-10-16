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

namespace ABadCafe\MC64K\Tests;
use ABadCafe\MC64K\Utils\TestCase;
use ABadCafe\MC64K\Parser\Utils;
/**
 * FloatExpressionTest
 *
 * Tests the ConstFloatExpression processing
 */
class FloatExpressionTest extends TestCase {

    const CASES = [
        "0.125"         => "0.125",
        // Floating point expressions are required to be placed in braces so that the
        // assembler understands the final value is intended to be a floating point
        // immediate.
        "{0.125 + 1.0}" => "1.125",
        "{0.125 - 1.0}" => "-0.875",
        "{0.5 ** 2}"    => "0.25",
        "{1.0 / 0.125}" => "8",
        "{1.0 + 2.5e-1}" => "1.25"
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        $oParser = new Utils\ConstFloatExpression();
        foreach(self::CASES as $sCase => $sExpect) {
            if ($this->isVerbose()) {
                echo "\t", $sCase, " => ", $sExpect, "\n";
            }
            $this->assertSame(
                $sExpect,
                $oParser->parse((string)$sCase)
            );
        }

    }
}

