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
 * DisplacedIndirectEATest
 *
 * Tests for d32(rX) / (d32, rX) addressing modes.
 * We don't test the different integer formatting options here as there are separate tests for the formatter.
 */
class DisplacedIndirectEATest extends TestCase {

    const DISPLACEMENTS = [
        '16'           => "\x10\x00\x00\x00",
        '-1'           => "\xff\xff\xff\xff",
    ];

    const NATIVE_TEST_CASE = [
        '%s(%sr0)'  => IRegisterIndirect::R0_IND_DSP,
        '%s(%sr1)'  => IRegisterIndirect::R1_IND_DSP,
        '%s(%sr2)'  => IRegisterIndirect::R2_IND_DSP,
        '%s(%sr3)'  => IRegisterIndirect::R3_IND_DSP,
        '%s(%sr4)'  => IRegisterIndirect::R4_IND_DSP,
        '%s(%sr5)'  => IRegisterIndirect::R5_IND_DSP,
        '%s(%sr6)'  => IRegisterIndirect::R6_IND_DSP,
        '%s(%sr7)'  => IRegisterIndirect::R7_IND_DSP,
        '%s(%sr8)'  => IRegisterIndirect::R8_IND_DSP,
        '%s(%sr9)'  => IRegisterIndirect::R9_IND_DSP,
        '%s(%sr10)' => IRegisterIndirect::R10_IND_DSP,
        '%s(%sr11)' => IRegisterIndirect::R11_IND_DSP,
        '%s(%sr12)' => IRegisterIndirect::R12_IND_DSP,
        '%s(%sr13)' => IRegisterIndirect::R13_IND_DSP,
        '%s(%sr14)' => IRegisterIndirect::R14_IND_DSP,
        '%s(%sr15)' => IRegisterIndirect::R15_IND_DSP,
    ];

    const LEGACY_TEST_CASE = [
        '%s(%sa0)' => IRegisterIndirect::R8_IND_DSP,
        '%s(%sa1)' => IRegisterIndirect::R9_IND_DSP,
        '%s(%sa2)' => IRegisterIndirect::R10_IND_DSP,
        '%s(%sa3)' => IRegisterIndirect::R11_IND_DSP,
        '%s(%sa4)' => IRegisterIndirect::R12_IND_DSP,
        '%s(%sa5)' => IRegisterIndirect::R13_IND_DSP,
        '%s(%sa6)' => IRegisterIndirect::R14_IND_DSP,
        '%s(%sa7)' => IRegisterIndirect::R15_IND_DSP,
        '%s(%ssp)' => IRegisterIndirect::R15_IND_DSP,
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        $oParser = new EffectiveAddress\GPRIndirectDisplacement();
        foreach (self::DISPLACEMENTS as $sDisplacement => $sImmediate) {
            $this->testNativeIndirect($oParser, (string)$sDisplacement, $sImmediate);
            $this->testLegacyIndirect($oParser, (string)$sDisplacement, $sImmediate);
        }
    }

    private function testNativeIndirect(
        EffectiveAddress\GPRIndirectDisplacement $oParser,
        string $sDisplacement,
        string $sImmediate
    ): void {
        echo "\ttesting: native gpr indirect with ", $sDisplacement, " displacement\n";
        foreach (self::NATIVE_TEST_CASE as $sTestCaseTemplate => $iOpcode) {
            $sExpectCode = chr($iOpcode) . $sImmediate;
            $sPrefixCase = sprintf($sTestCaseTemplate, $sDisplacement, '');
            $this->assertSame($sExpectCode, $oParser->parse($sPrefixCase));
            $sInnerCase = sprintf($sTestCaseTemplate, '', $sDisplacement . ', ');
            $this->assertSame($sExpectCode, $oParser->parse($sPrefixCase));
        }
    }

    private function testLegacyIndirect(
        EffectiveAddress\GPRIndirectDisplacement $oParser,
        string $sDisplacement,
        string $sImmediate
    ): void {
        echo "\ttesting: legacy gpr indirect with ", $sDisplacement, " displacement\n";
        foreach (self::LEGACY_TEST_CASE as $sTestCaseTemplate => $iOpcode) {
            $sExpectCode = chr($iOpcode) . $sImmediate;
            $sPrefixCase = sprintf($sTestCaseTemplate, $sDisplacement, '');
            $this->assertSame($sExpectCode, $oParser->parse($sPrefixCase));
            $sInnerCase = sprintf($sTestCaseTemplate, '', $sDisplacement . ', ');
            $this->assertSame($sExpectCode, $oParser->parse($sPrefixCase));
        }
    }
}

