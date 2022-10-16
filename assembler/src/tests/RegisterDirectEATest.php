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
use ABadCafe\MC64K\Parser;
use ABadCafe\MC64K\Defs\EffectiveAddress\IRegisterDirect;

/**
 * RegisterDirectEATest
 *
 */
class RegisterDirectEATest extends TestCase {

    /**
     * @const array<string, int>
     */
    const NATIVE_GPR_NAMES = [
        'r0'  => IRegisterDirect::R0_DIR,
        'r1'  => IRegisterDirect::R1_DIR,
        'r2'  => IRegisterDirect::R2_DIR,
        'r3'  => IRegisterDirect::R3_DIR,
        'r4'  => IRegisterDirect::R4_DIR,
        'r5'  => IRegisterDirect::R5_DIR,
        'r6'  => IRegisterDirect::R6_DIR,
        'r7'  => IRegisterDirect::R7_DIR,
        'r8'  => IRegisterDirect::R8_DIR,
        'r9'  => IRegisterDirect::R9_DIR,
        'r10' => IRegisterDirect::R10_DIR,
        'r11' => IRegisterDirect::R11_DIR,
        'r12' => IRegisterDirect::R12_DIR,
        'r13' => IRegisterDirect::R13_DIR,
        'r14' => IRegisterDirect::R14_DIR,
        'r15' => IRegisterDirect::R15_DIR,
    ];

    /**
     * @const array<string, int>
     */
    const LEGACY_GPR_NAMES = [
        'd0' => IRegisterDirect::R0_DIR,
        'd1' => IRegisterDirect::R1_DIR,
        'd2' => IRegisterDirect::R2_DIR,
        'd3' => IRegisterDirect::R3_DIR,
        'd4' => IRegisterDirect::R4_DIR,
        'd5' => IRegisterDirect::R5_DIR,
        'd6' => IRegisterDirect::R6_DIR,
        'd7' => IRegisterDirect::R7_DIR,
        'a0' => IRegisterDirect::R8_DIR,
        'a1' => IRegisterDirect::R9_DIR,
        'a2' => IRegisterDirect::R10_DIR,
        'a3' => IRegisterDirect::R11_DIR,
        'a4' => IRegisterDirect::R12_DIR,
        'a5' => IRegisterDirect::R13_DIR,
        'a6' => IRegisterDirect::R14_DIR,
        'a7' => IRegisterDirect::R15_DIR,
        'sp' => IRegisterDirect::R15_DIR,
    ];

    /**
     * @const string[]
     */
    const BAD_GPR_NAMES = [
        'd8',  'd9', 'd10', 'd11', 'd12', 'd13', 'd14', 'd15',
        'a8',  'a9', 'a10', 'a11', 'a12', 'a13', 'a14', 'a15',
        'r16', 'r17'
    ];

    /**
     * @const string[]
     */
    const NON_GPR_NAMES = [
        'non', 'gpr', 'reg', 'name', 'rax', 'rbx', 'xmm0', 'ccr', 'sr', 'ssp'
    ];

    const FPR_NAMES = [
        'fp0'  => IRegisterDirect::FP0_DIR,
        'fp1'  => IRegisterDirect::FP1_DIR,
        'fp2'  => IRegisterDirect::FP2_DIR,
        'fp3'  => IRegisterDirect::FP3_DIR,
        'fp4'  => IRegisterDirect::FP4_DIR,
        'fp5'  => IRegisterDirect::FP5_DIR,
        'fp6'  => IRegisterDirect::FP6_DIR,
        'fp7'  => IRegisterDirect::FP7_DIR,
        'fp8'  => IRegisterDirect::FP8_DIR,
        'fp9'  => IRegisterDirect::FP9_DIR,
        'fp10' => IRegisterDirect::FP10_DIR,
        'fp11' => IRegisterDirect::FP11_DIR,
        'fp12' => IRegisterDirect::FP12_DIR,
        'fp13' => IRegisterDirect::FP13_DIR,
        'fp14' => IRegisterDirect::FP14_DIR,
        'fp15' => IRegisterDirect::FP15_DIR,
    ];

    const BAD_FPR_NAMES = [
        'fp16', 'fp17'
    ];

    /**
     * @const string[]
     */
    const NON_FPR_NAMES = [
        'f0', 'f1', 'f2', 'f3', 'f4', 'f5', 'f6', 'f7',
        'non', 'fpr', 'reg', 'name', 'rax', 'rbx', 'xmm0', 'ccr', 'sr', 'ssp'
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        $this->testNativeGPRNames();
        $this->testLegacyGPRNames();
        $this->testIllegalGPRNames();
        $this->testFPRNames();
        $this->testIllegalFPRNames();
    }

    private function testFPRNames(): void {
        if ($this->isVerbose()) {
            echo "\ttesting: native fpr names\n";
        }
        $oParser = new Parser\EffectiveAddress\FPRDirect();
        foreach (self::FPR_NAMES as $sName => $iOpcode) {
            $sBytecode = $oParser->parse($sName);
            $this->assertSame(chr($iOpcode), $sBytecode);
        }
    }

    private function testIllegalFPRNames(): void {
        if ($this->isVerbose()) {
            echo "\ttesting: illegal fpr names\n";
        }
        $oParser = new Parser\EffectiveAddress\FPRDirect();
        foreach (self::BAD_FPR_NAMES as $sName) {
            try {
                $sBytecode = $oParser->parse($sName);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf(\OutOfBoundsException::class, $oError);
            }
        }
        foreach (self::NON_FPR_NAMES as $sName) {
            $this->assertSame(null, $oParser->parse($sName));
        }
    }

    private function testNativeGPRNames(): void {
        if ($this->isVerbose()) {
            echo "\ttesting: native gpr names\n";
        }
        $oParser = new Parser\EffectiveAddress\GPRDirect();
        foreach (self::NATIVE_GPR_NAMES as $sName => $iOpcode) {
            $sBytecode = $oParser->parse($sName);
            $this->assertSame(chr($iOpcode), $sBytecode);
        }
    }

    private function testLegacyGPRNames(): void {
        if ($this->isVerbose()) {
            echo "\ttesting: legacy gpr names\n";
        }
        $oParser = new Parser\EffectiveAddress\GPRDirect();
        foreach (self::LEGACY_GPR_NAMES as $sName => $iOpcode) {
            $sBytecode = $oParser->parse($sName);
            $this->assertSame(chr($iOpcode), $sBytecode);
        }
    }

    private function testIllegalGPRNames(): void {
        if ($this->isVerbose()) {
            echo "\ttesting: illegal gpr names\n";
        }
        $oParser = new Parser\EffectiveAddress\GPRDirect();
        foreach (self::BAD_GPR_NAMES as $sName) {
            try {
                $sBytecode = $oParser->parse($sName);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf(\OutOfBoundsException::class, $oError);
            }
        }
        foreach (self::NON_GPR_NAMES as $sName) {
            $this->assertSame(null, $oParser->parse($sName));
        }
    }
}

