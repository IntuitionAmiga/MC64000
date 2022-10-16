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
use ABadCafe\MC64K\Defs\EffectiveAddress\IByteCodeGroups;
//use ABadCafe\MC64K\Defs\EffectiveAddress\IRegisterIndirect;

/**
 * DisplacedIndirectEATest
 *
 * Tests for d32(rX) / (d32, rX) addressing modes.
 * We don't test the different integer formatting options here as there are separate tests for the formatter.
 */
class DisplacedIndirectEATest extends TestCase {

    /**
     * Immediate value test cases grouped by the base effective addressing mode.
     */
    const DISPLACEMENT_TEST_CASE = [
        // Displacements in the range -128 ... 127 should give an 8-bit displacement mode
        IByteCodeGroups::OFS_GPR_IND_DSP8 => [
            '-128' => "\x80",
            '-1'   => "\xff",
            '1'    => "\x01",
            '127'  => "\x7f",
        ],
        // Displacements outside the range -128 ... 127 should give a 32-bit displacement mode
        IByteCodeGroups::OFS_GPR_IND_DSP => [
            '-129'       => "\x7f\xff\xff\xff",
            '128'        => "\x80\x00\x00\x00",
            '0x12345678' => "\x78\x56\x34\x12", // note little endian order
        ],
    ];

    const NATIVE_TEST_CASE = [
        '%s(%sr0)'  => 0,
        '%s(%sr1)'  => 1,
        '%s(%sr2)'  => 2,
        '%s(%sr3)'  => 3,
        '%s(%sr4)'  => 4,
        '%s(%sr5)'  => 5,
        '%s(%sr6)'  => 6,
        '%s(%sr7)'  => 7,
        '%s(%sr8)'  => 8,
        '%s(%sr9)'  => 9,
        '%s(%sr10)' => 10,
        '%s(%sr11)' => 11,
        '%s(%sr12)' => 12,
        '%s(%sr13)' => 13,
        '%s(%sr14)' => 14,
        '%s(%sr15)' => 15,
    ];

    const LEGACY_TEST_CASE = [
        '%s(%sa0)' => 8,
        '%s(%sa1)' => 9,
        '%s(%sa2)' => 10,
        '%s(%sa3)' => 11,
        '%s(%sa4)' => 12,
        '%s(%sa5)' => 13,
        '%s(%sa6)' => 14,
        '%s(%sa7)' => 15,
        '%s(%ssp)' => 15,
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        $oParser = new EffectiveAddress\GPRIndirectDisplacement();
        foreach (self::DISPLACEMENT_TEST_CASE as $iBaseEAMode => $aImmediates) {
            foreach ($aImmediates as $sDisplacement => $sImmediate) {
                $this->testNativeIndirect(
                    $oParser,
                    (string)$sDisplacement,
                    $sImmediate,
                    $iBaseEAMode
                );
                $this->testLegacyIndirect(
                    $oParser,
                    (string)$sDisplacement,
                    $sImmediate,
                    $iBaseEAMode
                );
            }
        }
    }

    private function testNativeIndirect(
        EffectiveAddress\GPRIndirectDisplacement $oParser,
        string $sDisplacement,
        string $sImmediate,
        int    $iBaseEAMode
    ): void {
        if ($this->isVerbose()) {
            echo "\ttesting: native gpr indirect with ", $sDisplacement, " displacement\n";
        }
        foreach (self::NATIVE_TEST_CASE as $sTestCaseTemplate => $iRegNum) {
            $sExpectCode = chr($iBaseEAMode|$iRegNum) . $sImmediate;
            $sPrefixCase = sprintf($sTestCaseTemplate, $sDisplacement, '');
            $this->assertSame($sExpectCode, $oParser->parse($sPrefixCase));
            $sInnerCase = sprintf($sTestCaseTemplate, '', $sDisplacement . ', ');
            $this->assertSame($sExpectCode, $oParser->parse($sPrefixCase));
        }
    }

    private function testLegacyIndirect(
        EffectiveAddress\GPRIndirectDisplacement $oParser,
        string $sDisplacement,
        string $sImmediate,
        int    $iBaseEAMode
    ): void {
        if ($this->isVerbose()) {
            echo "\ttesting: legacy gpr indirect with ", $sDisplacement, " displacement\n";
        }
        foreach (self::LEGACY_TEST_CASE as $sTestCaseTemplate => $iRegNum) {
            $sExpectCode = chr($iBaseEAMode|$iRegNum) . $sImmediate;
            $sPrefixCase = sprintf($sTestCaseTemplate, $sDisplacement, '');
            $this->assertSame($sExpectCode, $oParser->parse($sPrefixCase));
            $sInnerCase = sprintf($sTestCaseTemplate, '', $sDisplacement . ', ');
            $this->assertSame($sExpectCode, $oParser->parse($sPrefixCase));
        }
    }
}

