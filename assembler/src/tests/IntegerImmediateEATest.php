<?php

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
 * test for that.
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
    }

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

    private function testByte(EffectiveAddress\IntegerImmediate $oParser): void {
        $aTestCases = [
            // All values map to byte immediate
            '#9'    => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(9),
            '#127'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(127),
            '#-1'   => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(255),
            '#-128' => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(128),

            // Unsigned
            '#128'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(128),
            '#255'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(255),
        ];
        $oParser->setOperationSize(1);
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

    private function testWord(EffectiveAddress\IntegerImmediate $oParser): void {
        $aTestCases = [
            // Byte immediate
            '#9'    => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(9),
            '#127'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(127),
            '#-1'   => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(255),
            '#-128' => chr(Defs\EffectiveAddress\IOther::INT_IMM_BYTE) . chr(128),

            // Word immediate
            '#128'  => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . chr(128) . chr(0),
            '#-129' => chr(Defs\EffectiveAddress\IOther::INT_IMM_WORD) . chr(127) . chr(255),

        ];


        $oParser->setOperationSize(2);
        foreach ($aTestCases as $sTestCase => $sExpect) {
            echo $sTestCase, ": ", bin2hex($oParser->parse($sTestCase)), "\n";

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

}

