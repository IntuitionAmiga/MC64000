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
use ABadCafe\MC64K\Parser\EffectiveAddress;
use ABadCafe\MC64K\Defs;

use function \chr;

/**
 * IntegerImmediateEATest
 *
 * Tests the parsing of integer immediate operands. Does not test the formatting as there is a separate
 * test for that. This tests the encoding into a binary effective address mode string, which comprises
 * an appropriate EA mode byte, followed by zero, 1, 2, 4 or 8 bytes to encode the integer. The smallest
 * representation will always be used when taking into consideration the operation size. For example,
 * values 0-8 should always use the small immediate EA mode, -1 will always be a byte immediate, etc.
 *
 * Also note, encoded values are little endian.
 */
class IntegerImmediateEATest extends TestCase {

    /**
     * @inheritDoc
     */
    public function run(): void {
        $oParser = new EffectiveAddress\IntegerImmediate();
        $this->testSmall($oParser);
        $this->testByte($oParser);
        $this->testWord($oParser);
        $this->testLong($oParser);
        $this->testQuad($oParser);
    }

    /**
     * Tests for small immediate only
     */
    private function testSmall(EffectiveAddress\IntegerImmediate $oParser): void {
        $aSmallCases = [
            '#0' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_0),
            '#1' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_1),
            '#2' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_2),
            '#3' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_3),
            '#4' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_4),
            '#5' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_5),
            '#6' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_6),
            '#7' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_7),
            '#8' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_8),
        ];
        $oParser->setOperationSize(1);
        foreach ($aSmallCases as $sTestCase => $sExpect) {
            $this->assertSame($sExpect, $oParser->parse($sTestCase));
        }
    }

    /**
     * Tests for immediate up to byte
     */
    private function testByte(EffectiveAddress\IntegerImmediate $oParser): void {
        $aTestCases = [
            // Small immediate representation
            '#0' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_0),
            '#8' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_8),

            // Byte immediate representation
            '#9'    => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x09",
            '#127'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x7f",
            '#-1'   => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\xff",
            '#-128' => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x80",

            // Unsigned
            '#128'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x80",
            '#255'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\xff",
        ];
        $oParser->setOperationSize(Defs\IIntLimits::BYTE);
        foreach ($aTestCases as $sTestCase => $sExpect) {
            $this->assertSame($sExpect, $oParser->parse($sTestCase));
        }

        // Legal number of digits but outside of range
        try {
            $oParser->parse('#+256');
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\RangeException::class, $oError);
        }

        // Too many digits
        try {
            $oParser->parse('#1000');
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\LengthException::class, $oError);
        }
    }

    /**
     * Tests for immediate up to word
     */
    private function testWord(EffectiveAddress\IntegerImmediate $oParser): void {
        $aTestCases = [
            // Small immediate
            '#0' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_0),
            '#8' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_8),

            // Byte immediate
            '#9'     => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x09",
            '#127'   => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x7f",
            '#-1'    => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\xff",
            '#-128'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x80",
            '#65535' => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\xff",

            // Word immediate
            '#128'    => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x80\x00",
            '#-129'   => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x7f\xff",
            '#32767'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\xff\x7f",
            '#-32768' => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x00\x80",

            // Unsigned
            '#32768'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x00\x80",
        ];

        $oParser->setOperationSize(Defs\IIntLimits::WORD);
        foreach ($aTestCases as $sTestCase => $sExpect) {
            $this->assertSame($sExpect, $oParser->parse($sTestCase));
        }

        // Legal number of digits but outside of range
        try {
            $oParser->parse('#65536');
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\RangeException::class, $oError);
        }

        // Too many digits
        try {
            $oParser->parse('#100000');
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\LengthException::class, $oError);
        }
    }

    /**
     * Tests for immediate up to long
     */
    private function testLong(EffectiveAddress\IntegerImmediate $oParser): void {
        $aTestCases = [
            // Small immediate
            '#0' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_0),
            '#8' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_8),

            // Byte immediate
            '#9'          => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x09",
            '#127'        => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x7f",
            '#-1'         => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\xff",
            '#-128'       => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x80",
            '#4294967295' => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\xff",

            // Word immediate
            '#128'    => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x80\x00",
            '#-129'   => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x7f\xff",
            '#32767'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\xff\x7f",
            '#-32768' => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x00\x80",

            // Long immediate
            '#32768'      => chr(Defs\EffectiveAddress\IOther::INT_IMM_LONG) . "\x00\x80\x00\x00",
            '#2147483647' => chr(Defs\EffectiveAddress\IOther::INT_IMM_LONG) . "\xff\xff\xff\x7f",

            // Unsigned
            '#2147483648' => chr(Defs\EffectiveAddress\IOther::INT_IMM_LONG) . "\x00\x00\x00\x80",
        ];

        $oParser->setOperationSize(Defs\IIntLimits::LONG);
        foreach ($aTestCases as $sTestCase => $sExpect) {
            $this->assertSame($sExpect, $oParser->parse($sTestCase));
        }

        // Legal number of digits but outside of range
        try {
            $oParser->parse('#4294967296');
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\RangeException::class, $oError);
        }

        // Too many digits
        try {
            $oParser->parse('#42949672960');
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\LengthException::class, $oError);
        }
    }

    /**
     * Tests for immediate up to quad
     */
    private function  testQuad(EffectiveAddress\IntegerImmediate $oParser): void {
        $aTestCases = [
            // Small immediate
            '#0' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_0),
            '#8' => chr(Defs\EffectiveAddress\IOther::INT_SMALL_8),

            // Byte immediate
            '#9'    => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x09",
            '#127'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x7f",
            '#-1'   => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\xff",
            '#-128' => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . "\x80",

            // Word immediate
            '#128'    => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x80\x00",
            '#-129'   => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x7f\xff",
            '#32767'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\xff\x7f",
            '#-32768' => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . "\x00\x80",

            // Long immediate
            '#32768'      => chr(Defs\EffectiveAddress\IOther::INT_IMM_LONG) . "\x00\x80\x00\x00",
            '#2147483647' => chr(Defs\EffectiveAddress\IOther::INT_IMM_LONG) . "\xff\xff\xff\x7f",

            // Quad
            '#4294967296'          => chr(Defs\EffectiveAddress\IOther::INT_IMM_QUAD) . "\x00\x00\x00\x00\x01\x00\x00\x00",
            '#9223372036854775807' => chr(Defs\EffectiveAddress\IOther::INT_IMM_QUAD) . "\xff\xff\xff\xff\xff\xff\xff\x7f"
        ];

        $oParser->setOperationSize(Defs\IIntLimits::QUAD);
        foreach ($aTestCases as $sTestCase => $sExpect) {
            $this->assertSame($sExpect, $oParser->parse($sTestCase));
        }

        // Legal number of digits but outside of range
        try {
            $oParser->parse('#18446744073709551616');
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\RangeException::class, $oError);
        }

        // Too many digits
        try {
            $oParser->parse('#184467440737095516160');
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\LengthException::class, $oError);
        }
    }
}

