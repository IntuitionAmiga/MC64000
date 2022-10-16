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
use ABadCafe\MC64K\Defs\EffectiveAddress\IRegisterIndirect;

/**
 * SimpleIndirectEATest
 *
 * Tests the simple GPR indirect modes, including pre/post inc/decrement variations.
 */
class SimpleIndirectEATest extends TestCase {

    const
        PARSER  = 0,
        PREFIX  = 1,
        POSTFIX = 2,
        INFO    = 3
    ;

    const MODE_VARIANTS = [
        IRegisterIndirect::OFS_GPR_IND          => [
            EffectiveAddress\GPRIndirect::class,
            '',
            '',
            ''
        ],
        IRegisterIndirect::OFS_GPR_IND_POST_INC => [
            EffectiveAddress\GPRIndirectUpdating::class,
            '',
            '+',
            ', post increment'
        ],
        IRegisterIndirect::OFS_GPR_IND_POST_DEC => [
            EffectiveAddress\GPRIndirectUpdating::class,
            '',
            '-',
            ', post decrement'
        ],
        IRegisterIndirect::OFS_GPR_IND_PRE_INC  => [
            EffectiveAddress\GPRIndirectUpdating::class,
            '+',
            '',
            ', pre increment'
        ],
        IRegisterIndirect::OFS_GPR_IND_PRE_DEC  => [
            EffectiveAddress\GPRIndirectUpdating::class,
            '-',
            '',
            ', pre decrement'
        ],
    ];

    const NATIVE_INDIRECT_TEST_CASE = [
        '(r0)'  => 0,
        '(r1)'  => 1,
        '(r2)'  => 2,
        '(r3)'  => 3,
        '(r4)'  => 4,
        '(r5)'  => 5,
        '(r6)'  => 6,
        '(r7)'  => 7,
        '(r8)'  => 8,
        '(r9)'  => 9,
        '(r10)' => 10,
        '(r11)' => 11,
        '(r12)' => 12,
        '(r13)' => 13,
        '(r14)' => 14,
        '(r15)' => 15,
    ];

    const LEGACY_INDIRECT_TEST_CASE = [
        '(a0)' => 8,
        '(a1)' => 9,
        '(a2)' => 10,
        '(a3)' => 11,
        '(a4)' => 12,
        '(a5)' => 13,
        '(a6)' => 14,
        '(a7)' => 15,
        '(sp)' => 15,
    ];

    const BAD_GPR_TEST_CASE = [
        '(a8)', '(a9)', '(r16)'
    ];

    const NON_GPR_TEST_CASE = [
        '(d0)', '(label)', '(#0)'
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        foreach (self::MODE_VARIANTS as $iOffset => $aVariant) {
            $oParser = new $aVariant[self::PARSER];
            $this->testNativeIndirect($oParser, $iOffset, $aVariant);
            $this->testLegacyIndirect($oParser, $iOffset, $aVariant);
            $this->testIllegalIndirect($oParser, $iOffset, $aVariant);
        }
    }

    /**
     * @param string[] $aVariant
     */
    private function testNativeIndirect(EffectiveAddress\IParser $oParser, int $iOffset, array $aVariant): void {
        if ($this->isVerbose()) {
            echo "\ttesting: native gpr indirect", $aVariant[self::INFO], "\n";
        }
        foreach (self::NATIVE_INDIRECT_TEST_CASE as $sTestCase => $iRegNum) {
            $sTestCase = $aVariant[self::PREFIX] . $sTestCase . $aVariant[self::POSTFIX];
            $this->assertSame(chr($iOffset + $iRegNum), $oParser->parse($sTestCase));
        }
    }

    /**
     * @param string[] $aVariant
     */
    private function testLegacyIndirect(EffectiveAddress\IParser $oParser, int $iOffset, array $aVariant): void {
        if ($this->isVerbose()) {
            echo "\ttesting: legacy gpr indirect", $aVariant[self::INFO], "\n";
        }
        foreach (self::LEGACY_INDIRECT_TEST_CASE as $sTestCase => $iRegNum) {
            $sTestCase = $aVariant[self::PREFIX] . $sTestCase . $aVariant[self::POSTFIX];
            $this->assertSame(chr($iOffset + $iRegNum), $oParser->parse($sTestCase));
        }
    }

    /**
     * @param string[] $aVariant
     */
    private function testIllegalIndirect(EffectiveAddress\IParser $oParser, int $iOffset, array $aVariant): void {
        if ($this->isVerbose()) {
            echo "\ttesting: illegal gpr indirect", $aVariant[self::INFO], "\n";
        }
        foreach (self::BAD_GPR_TEST_CASE as $sTestCase) {
            try {
                $sTestCase = $aVariant[self::PREFIX] . $sTestCase . $aVariant[self::POSTFIX];
                var_dump($oParser->parse($sTestCase));
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf(\OutOfBoundsException::class, $oError);
            }
        }
        foreach (self::NON_GPR_TEST_CASE as $sTestCase) {
            $sTestCase = $aVariant[self::PREFIX] . $sTestCase . $aVariant[self::POSTFIX];
            $this->assertSame(null, $oParser->parse($sTestCase));
        }
    }
}

