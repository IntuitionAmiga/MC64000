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
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Parser;

/**
 * FloatFormatTest
 *
 * Validates the behaviour of Parser\Utils\FloatingPoint::parseLiteral() which handles conversion of string literal
 * numbers in varying formats and returning them as a normalised double precision value.
 */
class FloatFormatTest extends TestCase {

    /**
     * TEST_CASES
     *
     * We run these and permute case and suffix for both single and double sizes.
     */
    const TEST_CASES = [
        '12.5'        => 12.5,
        '12.50'       => 12.5,
        '012.5'       => 12.5,
        '012.50'      => 12.5,
        '+12.5'       => 12.5,
        '+12.50'      => 12.5,
        '+012.5'      => 12.5,
        '+012.50'     => 12.5,
        '1.25e1'      => 12.5,
        '1.250e1'     => 12.5,
        '01.25e1'     => 12.5,
        '01.250e1'    => 12.5,
        '+1.25e1'     => 12.5,
        '+1.250e1'    => 12.5,
        '+01.25e1'    => 12.5,
        '+01.250e1'   => 12.5,
        '1.25e01'     => 12.5,
        '1.250e01'    => 12.5,
        '01.25e01'    => 12.5,
        '01.250e01'   => 12.5,
        '+1.25e01'    => 12.5,
        '+1.250e01'   => 12.5,
        '+01.25e01'   => 12.5,
        '+01.250e01'  => 12.5,
        '1.25e+1'     => 12.5,
        '1.250e+1'    => 12.5,
        '01.25e+1'    => 12.5,
        '01.250e+1'   => 12.5,
        '+1.25e+1'    => 12.5,
        '+1.250e+1'   => 12.5,
        '+01.25e+1'   => 12.5,
        '+01.250e+1'  => 12.5,
        '1.25e+01'    => 12.5,
        '1.250e+01'   => 12.5,
        '01.25e+01'   => 12.5,
        '01.250e+01'  => 12.5,
        '+1.25e+01'   => 12.5,
        '+1.250e+01'  => 12.5,
        '+01.25e+01'  => 12.5,
        '+01.250e+01' => 12.5,
        '-12.5'       => -12.5,
        '-12.50'      => -12.5,
        '-012.5'      => -12.5,
        '-012.50'     => -12.5,
        '-1.25e1'     => -12.5,
        '-1.250e1'    => -12.5,
        '-01.25e1'    => -12.5,
        '-01.250e1'   => -12.5,
        '-1.25e01'    => -12.5,
        '-1.250e01'   => -12.5,
        '-01.25e01'   => -12.5,
        '-01.250e01'  => -12.5,
        '-1.25e+1'    => -12.5,
        '-1.250e+1'   => -12.5,
        '-01.25e+1'   => -12.5,
        '-01.250e+1'  => -12.5,
        '-1.25e+01'   => -12.5,
        '-1.250e+01'  => -12.5,
        '-01.25e+01'  => -12.5,
        '-01.250e+01' => -12.5,
        '0.125'       => 0.125,
        '0.1250'      => 0.125,
        '00.125'      => 0.125,
        '00.1250'     => 0.125,
        '+0.125'      => 0.125,
        '+0.1250'     => 0.125,
        '+00.125'     => 0.125,
        '+00.1250'    => 0.125,
        '1.25e-1'     => 0.125,
        '1.250e-1'    => 0.125,
        '01.25e-1'    => 0.125,
        '01.250e-1'   => 0.125,
        '+1.25e-1'    => 0.125,
        '+1.250e-1'   => 0.125,
        '+01.25e-1'   => 0.125,
        '+01.250e-1'  => 0.125,
        '1.25e-01'    => 0.125,
        '1.250e-01'   => 0.125,
        '01.25e-01'   => 0.125,
        '01.250e-01'  => 0.125,
        '+1.25e-01'   => 0.125,
        '+1.250e-01'  => 0.125,
        '+01.25e-01'  => 0.125,
        '+01.250e-01' => 0.125,
        '-0.125'       => -0.125,
        '-0.1250'      => -0.125,
        '-00.125'      => -0.125,
        '-00.1250'     => -0.125,
        '-1.25e-1'     => -0.125,
        '-1.250e-1'    => -0.125,
        '-01.25e-1'    => -0.125,
        '-01.250e-1'   => -0.125,
        '-1.25e-01'    => -0.125,
        '-1.250e-01'   => -0.125,
        '-01.25e-01'   => -0.125,
        '-01.250e-01'  => -0.125,
    ];

    const SUFFIXES = [
        Defs\IFloatLimits::SINGLE => ['', 's', 'S'],
        Defs\IFloatLimits::DOUBLE => ['', 'd', 'D'],
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        foreach (self::SUFFIXES as $iWordSize => $aSuffixes) {
            foreach ($aSuffixes as $sSuffix) {
                foreach (self::TEST_CASES as $sExpression => $fExpect) {
                    $fResult = Parser\Utils\FloatingPoint::parseLiteral((string)$sExpression . $sSuffix, $iWordSize);
                    $this->assertSame($fExpect, $fResult);
                    $fResult = Parser\Utils\FloatingPoint::parseLiteral(strtoupper((string)$sExpression) . $sSuffix, $iWordSize);
                    $this->assertSame($fExpect, $fResult);
                }
            }
        }
    }
}

