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
use ABadCafe\MC64K\Parser\SourceLine\Data;

/**
 * DeclareConstantTest
 *
 * Validates the expected behaviours of the Declaration SourceLine parser, which handles all dc.b/w/l/q/s/d cases.
 */
class DeclareConstantTest extends TestCase {

    /**
     * Basic test cases for checkLine(). It specifically looks for dc.(b/w/l/q/s/d) followed by anything.
     *
     * @const array<string, bool>
     */
    const CHECK_LINE_TEST_CASES = [
        ''                     => false,
        '    '                 => false,
        '    ; comment'        => false,
        '.label'               => false,
        '    mnem.x #1, r0'    => false,
        '    @directive'       => false,
        '    dc.x anything'    => false,
        '    dc.b'             => false,
        '    dc.b anything'    => true,
        '    dc.w anything'    => true,
        '    dc.l anything'    => true,
        '    dc.q anything'    => true,
        '    dc.s anything'    => true,
        '    dc.d anything'    => true,
    ];

    /**
     * Expected outcomes for valid dc.b string cases.
     *
     * @const array<string, string>
     */
    const DECLARE_STRING_PASS_CASES = [
        '    dc.b "\0"'               => "\0",
        '    dc.b "Hello World!\n\0"' => "Hello World!\n\0"
    ];

    /**
     * Expected failure outcomes for dc.b string cases
     *
     * @const array<string, class-string>
     */
    const DECLARE_STRING_FAIL_CASES = [
        '    dc.b ""' => \LengthException::class
    ];

    /**
     * Expected outcomes for valid dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_BYTE_PASS_CASES = [
        '    dc.b 0'             => "\x00",
        '    dc.b 0, 1, 2'       => (
            "\x00" .
            "\x01" .
            "\x02"
        ),
        '    dc.b -1, -128, 255' => (
            "\xff" .
            "\x80" .
            "\xff"
        ),
        '    dc.b 9, %1010, 0b1011, 014, @15, $0E, 0x0F' => (
            "\x09" .
            "\x0a" .
            "\x0b" .
            "\x0c" .
            "\x0d" .
            "\x0e" .
            "\x0f"
        )
    ];

    /**
     * Expected failure outcomes for dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_BYTE_FAIL_CASES = [
        '    dc.b 256'  => \RangeException::class,
        '    dc.b -129' => \RangeException::class,
        '    dc.b #1'   => \InvalidArgumentException::class
    ];

    /**
     * Expected outcomes for valid dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_WORD_PASS_CASES = [
        '    dc.w 0'             => "\x00\x00",
        '    dc.w 0x0123'        => "\x23\x01",
        '    dc.w 0, 1, 2'       => (
            "\x00\x00" .
            "\x01\x00" .
            "\x02\x00"
        ),
        '    dc.w -1, -128, 255' => (
            "\xff\xff" .
            "\x80\xff" .
            "\xff\x00"
        ),
        '    dc.w 9, %1010, 0b1011, 014, @15, $0E, 0x0F' => (
            "\x09\x00" .
            "\x0a\x00" .
            "\x0b\x00" .
            "\x0c\x00" .
            "\x0d\x00" .
            "\x0e\x00" .
            "\x0f\x00"
        ),
    ];

    /**
     * Expected failure outcomes for dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_WORD_FAIL_CASES = [
        '    dc.w 65536'  => \RangeException::class,
        '    dc.w -32769' => \RangeException::class,
        '    dc.w #1'     => \InvalidArgumentException::class
    ];

    /**
     * Expected outcomes for valid dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_LONG_PASS_CASES = [
        '    dc.l 0'             => "\x00\x00\x00\x00",
        '    dc.l 0x01234567'    => "\x67\x45\x23\x01",
        '    dc.l 0, 1, 2'       => (
            "\x00\x00\x00\x00" .
            "\x01\x00\x00\x00" .
            "\x02\x00\x00\x00"
        ),
        '    dc.l -1, -128, 255' => (
            "\xff\xff\xff\xff" .
            "\x80\xff\xff\xff" .
            "\xff\x00\x00\x00"
        ),
        '    dc.l 9, %1010, 0b1011, 014, @15, $0E, 0x0F' => (
            "\x09\x00\x00\x00" .
            "\x0a\x00\x00\x00" .
            "\x0b\x00\x00\x00" .
            "\x0c\x00\x00\x00" .
            "\x0d\x00\x00\x00" .
            "\x0e\x00\x00\x00" .
            "\x0f\x00\x00\x00"
        ),
    ];

    /**
     * Expected failure outcomes for dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_LONG_FAIL_CASES = [
        '    dc.l 4294967296'  => \RangeException::class,
        '    dc.l -2147483649' => \RangeException::class,
        '    dc.l #1'          => \InvalidArgumentException::class
    ];


    /**
     * Expected outcomes for valid dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_QUAD_PASS_CASES = [
        '    dc.q 0'                   => "\x00\x00\x00\x00\x00\x00\x00\x00",
        '    dc.q 0x0123456789abcdef'  => "\xef\xcd\xab\x89\x67\x45\x23\x01",
        '    dc.q 0, 1, 2'             => (
            "\x00\x00\x00\x00\x00\x00\x00\x00" .
            "\x01\x00\x00\x00\x00\x00\x00\x00" .
            "\x02\x00\x00\x00\x00\x00\x00\x00"
        ),
        '    dc.q -1, -128, 255' => (
            "\xff\xff\xff\xff\xff\xff\xff\xff" .
            "\x80\xff\xff\xff\xff\xff\xff\xff" .
            "\xff\x00\x00\x00\x00\x00\x00\x00"
        ),
        '    dc.q 9, %1010, 0b1011, 014, @15, $0E, 0x0F' => (
            "\x09\x00\x00\x00\x00\x00\x00\x00" .
            "\x0a\x00\x00\x00\x00\x00\x00\x00" .
            "\x0b\x00\x00\x00\x00\x00\x00\x00" .
            "\x0c\x00\x00\x00\x00\x00\x00\x00" .
            "\x0d\x00\x00\x00\x00\x00\x00\x00" .
            "\x0e\x00\x00\x00\x00\x00\x00\x00" .
            "\x0f\x00\x00\x00\x00\x00\x00\x00"
        )
    ];

    /**
     * Expected failure outcomes for dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_QUAD_FAIL_CASES = [
       '    dc.q 18446744073709551616'  => \RangeException::class,
       '    dc.q -9223372036854775809'  => \RangeException::class,
       '    dc.q #1'                    => \InvalidArgumentException::class
    ];

    /**
     * Expected outcomes for valid dc.s list cases
     *
     * @const array<string, string>
     */
    const DECLARE_SINGLE_PASS_CASES = [
        '    dc.s 0.0'  => "\x00\x00\x00\x00",
        '    dc.s 1.0'  => "\x00\x00\x80\x3f",
        '    dc.s -1.0' => "\x00\x00\x80\xbf"
    ];

    /**
     * Expected failure outcomes for dc.s list cases
     *
     * @const array<string, string>
     */
    const DECLARE_SINGLE_FAIL_CASES = [
        '    dc.s #1.0'   => \InvalidArgumentException::class,
        '    dc.s 1.e+39' => \RangeException::class,
        '    dc.s 1.e-39' => \RangeException::class,
    ];

    /**
     * Expected outcomes for valid dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_DOUBLE_PASS_CASES = [
        '    dc.d 0.0'  => "\x00\x00\x00\x00\x00\x00\x00\x00",
        '    dc.d 1.0'  => "\x00\x00\x00\x00\x00\x00\xf0\x3f",
        '    dc.d -1.0' => "\x00\x00\x00\x00\x00\x00\xf0\xbf"
    ];

    /**
     * Expected failure outcomes for dc.b list cases
     *
     * @const array<string, string>
     */
    const DECLARE_DOUBLE_FAIL_CASES = [
        '    dc.d #1.0' => \InvalidArgumentException::class,
        '    dc.d 2.e+308' => \RangeException::class,
    ];


    /**
     * @inheritDoc
     */
    public function run(): void {
        $oParser = new Data\Declaration();
        $this->testCheckLine($oParser);
        $this->testDeclareString($oParser);
        $this->testDeclareBytes($oParser);
        $this->testDeclareWords($oParser);
        $this->testDeclareLongs($oParser);
        $this->testDeclareQuads($oParser);
        $this->testDeclareSingles($oParser);
        $this->testDeclareDoubles($oParser);
    }

    /**
     * Tests the checkLine() behaviour.
     */
    private function testCheckLine(Data\Declaration $oParser): void {
        echo "\ttesting: match\n";
        foreach (self::CHECK_LINE_TEST_CASES as $sTestCase => $bExpect) {
            $this->assertSame($bExpect, $oParser->checkLine($sTestCase));
        }
    }

    /**
     * Tests dc.b "text" behaviour.
     */
    private function testDeclareString(Data\Declaration $oParser): void {
        echo "\ttesting: dc.b string\n";
        foreach (self::DECLARE_STRING_PASS_CASES as $sTestCase => $sResult) {
            $oParser->checkLine($sTestCase);
            $this->assertSame($sResult, $oParser->parse($sTestCase));
        }
        foreach (self::DECLARE_STRING_FAIL_CASES as $sTestCase => $sException) {
            try {
                $oParser->checkLine($sTestCase);
                $oParser->parse($sTestCase);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf($sException, $oError);
            }
        }
    }

    /**
     * Tests dc.b 0, 1, ... behaviour.
     */
    private function testDeclareBytes(Data\Declaration $oParser): void {
        echo "\ttesting: dc.b int, ...\n";
        foreach (self::DECLARE_BYTE_PASS_CASES as $sTestCase => $sResult) {
            $oParser->checkLine($sTestCase);
            $this->assertSame($sResult, $oParser->parse($sTestCase));
        }
        foreach (self::DECLARE_BYTE_FAIL_CASES as $sTestCase => $sException) {
            try {
                $oParser->checkLine($sTestCase);
                $oParser->parse($sTestCase);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf($sException, $oError);
            }
        }
    }

    /**
     * Tests dc.w 0, 1, ... behaviour.
     */
    private function testDeclareWords(Data\Declaration $oParser): void {
        echo "\ttesting: dc.w int, ...\n";
        foreach (self::DECLARE_WORD_PASS_CASES as $sTestCase => $sResult) {
            $oParser->checkLine($sTestCase);
            $this->assertSame($sResult, $oParser->parse($sTestCase));
        }
        foreach (self::DECLARE_WORD_FAIL_CASES as $sTestCase => $sException) {
            try {
                $oParser->checkLine($sTestCase);
                $oParser->parse($sTestCase);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf($sException, $oError);
            }
        }
    }

    /**
     * Tests dc.l 0, 1, ... behaviour.
     */
    private function testDeclareLongs(Data\Declaration $oParser): void {
        echo "\ttesting: dc.l int, ...\n";
        foreach (self::DECLARE_LONG_PASS_CASES as $sTestCase => $sResult) {
            $oParser->checkLine($sTestCase);
            $this->assertSame($sResult, $oParser->parse($sTestCase));
        }
        foreach (self::DECLARE_LONG_FAIL_CASES as $sTestCase => $sException) {
            try {
                $oParser->checkLine($sTestCase);
                $oParser->parse($sTestCase);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf($sException, $oError);
            }
        }
    }

    /**
     * Tests dc.q 0, 1, ... behaviour.
     */
    private function testDeclareQuads(Data\Declaration $oParser): void {
        echo "\ttesting: dc.q int, ...\n";
        foreach (self::DECLARE_QUAD_PASS_CASES as $sTestCase => $sResult) {
            $oParser->checkLine($sTestCase);
            $this->assertSame($sResult, $oParser->parse($sTestCase));
        }
        foreach (self::DECLARE_QUAD_FAIL_CASES as $sTestCase => $sException) {
            try {
                $oParser->checkLine($sTestCase);
                $oParser->parse($sTestCase);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf($sException, $oError);
            }
        }
    }

    /**
     * Tests dc.s 0.0, 1.0, ... behaviour.
     */
    private function testDeclareSingles(Data\Declaration $oParser): void {
        echo "\ttesting: dc.s float, ...\n";
        foreach (self::DECLARE_SINGLE_PASS_CASES as $sTestCase => $sResult) {
            $oParser->checkLine($sTestCase);
            $this->assertSame($sResult, $oParser->parse($sTestCase));
        }
        foreach (self::DECLARE_SINGLE_FAIL_CASES as $sTestCase => $sException) {
            try {
                $oParser->checkLine($sTestCase);
                $oParser->parse($sTestCase);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf($sException, $oError);
            }
        }
    }

    /**
     * Tests dc.s 0.0, 1.0, ... behaviour.
     */
    private function testDeclareDoubles(Data\Declaration $oParser): void {
        echo "\ttesting: dc.d float, ...\n";
        foreach (self::DECLARE_DOUBLE_PASS_CASES as $sTestCase => $sResult) {
            $oParser->checkLine($sTestCase);
            $this->assertSame($sResult, $oParser->parse($sTestCase));
        }
        foreach (self::DECLARE_DOUBLE_FAIL_CASES as $sTestCase => $sException) {
            try {
                $oParser->checkLine($sTestCase);
                $oParser->parse($sTestCase);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf($sException, $oError);
            }
        }
    }

}

