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
 * IntegerCodeFoldTest
 *
 * Validates the code folding optimisations of the Instruction Statement parser. Code folding falls into two categories:
 *
 *    1) Total elimination: the statement has no overall effect and can be removed completely, e.g. multiplication by 1.
 *    2) Substitution:      the statement can be replaced with a simpler one, e.g. multiplication by 0.
 */
class IntegerCodeFoldTest extends TestCase {

    const
        ILLEGAL_VALUE = 0,
        PASS          = 1,
        CLEAR_B       = 2,
        CLEAR_W       = 3,
        CLEAR_L       = 4,
        CLEAR_Q       = 5,
        FOLD          = 6
    ;

    /**
     * Placebo cases - these should not fold, clear or be detected as illegal.
     */
    const NON_FOLDED_TEST_CASES = [
        'and.b #1, r0',
        'and.w #1, r0',
        'and.l #1, r0',
        'and.q #1, r0',
        'or.b #1, r0',
        'or.w #1, r0',
        'or.l #1, r0',
        'or.q #1, r0',
        'lsl.b #1, r0',
        'lsl.w #1, r0',
        'lsl.l #1, r0',
        'lsl.q #1, r0',
        'lsr.b #1, r0',
        'lsr.w #1, r0',
        'lsr.l #1, r0',
        'lsr.q #1, r0',
        'add.b #1, r0',
        'add.w #1, r0',
        'add.l #1, r0',
        'add.q #1, r0',
        'sub.b #1, r0',
        'sub.w #1, r0',
        'sub.l #1, r0',
        'sub.q #1, r0',
        'mulu.b #3, r0',
        'mulu.w #3, r0',
        'mulu.l #3, r0',
        'mulu.q #3, r0',
        'muls.b #3, r0',
        'muls.w #3, r0',
        'muls.l #3, r0',
        'muls.q #3, r0',
        'divs.l #3, r0',
        'divs.q #3, r0',
        'divu.l #3, r0',
        'divu.q #3, r0',
    ];

    const FOLD_TEST_CASES = [
        'or.b #0, r0',
        'or.w #0, r0',
        'or.l #0, r0',
        'or.q #0, r0',
        'lsl.b #0, r0',
        'lsl.w #0, r0',
        'lsl.l #0, r0',
        'lsl.q #0, r0',
        'lsr.b #0, r0',
        'lsr.w #0, r0',
        'lsr.l #0, r0',
        'lsr.q #0, r0',
        'add.b #0, r0',
        'add.w #0, r0',
        'add.l #0, r0',
        'add.q #0, r0',
        'sub.b #0, r0',
        'sub.w #0, r0',
        'sub.l #0, r0',
        'sub.q #0, r0',
        'muls.b #1, r0',
        'muls.w #1, r0',
        'muls.l #1, r0',
        'muls.q #1, r0',
        'mulu.b #1, r0',
        'mulu.w #1, r0',
        'mulu.l #1, r0',
        'mulu.q #1, r0',
        'divs.l #1, r0',
        'divs.q #1, r0',
        'divu.l #1, r0',
        'divu.q #1, r0',
    ];

    const CLEAR_TEST_CASES = [
        'clr.b r0'      => self::CLEAR_B,
        'clr.w r0'      => self::CLEAR_W,
        'clr.l r0'      => self::CLEAR_L,
        'clr.q r0'      => self::CLEAR_Q,
        'and.b #0, r0'  => self::CLEAR_B,
        'and.w #0, r0'  => self::CLEAR_W,
        'and.l #0, r0'  => self::CLEAR_L,
        'and.q #0, r0'  => self::CLEAR_Q,
        'muls.b #0, r0' => self::CLEAR_B,
        'muls.w #0, r0' => self::CLEAR_W,
        'muls.l #0, r0' => self::CLEAR_L,
        'muls.q #0, r0' => self::CLEAR_Q,
        'mulu.b #0, r0' => self::CLEAR_B,
        'mulu.w #0, r0' => self::CLEAR_W,
        'mulu.l #0, r0' => self::CLEAR_L,
        'mulu.q #0, r0' => self::CLEAR_Q,
    ];

    const TRAP_ILLEGAL_TEST_CASES = [
        'divs.l #0, r0',
        'divs.q #0, r0',
        'divu.l #0, r0',
        'divu.q #0, r0',
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        $oParser = new Parser\SourceLine\Instruction\Statement();
        $this->testNonFolded($oParser);
        $this->testFoldToClear($oParser);
        $this->testFoldToEmpty($oParser);
        $this->testTrapIllegal($oParser);
    }

    private function testNonFolded(Parser\SourceLine\Instruction\Statement $oParser): void {
        echo "\ttesting: non-folded\n";
        foreach (self::NON_FOLDED_TEST_CASES as $sInput) {
            $sBytecode = (string)$oParser->parse($sInput);
            $this->assertTrue(strlen($sBytecode) > 0);
        }
    }

    private function testFoldToClear(Parser\SourceLine\Instruction\Statement $oParser): void {
        echo "\ttesting: fold to clr.x\n";
        $aFoldBytecode = [
            self::CLEAR_B => chr(Defs\Mnemonic\IDataMove::CLR_B) . chr(0),
            self::CLEAR_W => chr(Defs\Mnemonic\IDataMove::CLR_W) . chr(0),
            self::CLEAR_L => chr(Defs\Mnemonic\IDataMove::CLR_L) . chr(0),
            self::CLEAR_Q => chr(Defs\Mnemonic\IDataMove::CLR_Q) . chr(0)
        ];

        foreach (self::CLEAR_TEST_CASES as $sInput => $iExpect) {
            $sBytecode = (string)$oParser->parse($sInput);
            $this->assertSame($aFoldBytecode[$iExpect], $sBytecode);
        }
    }

    private function testFoldToEmpty(Parser\SourceLine\Instruction\Statement $oParser): void {
        echo "\ttesting: fold to empty\n";
        foreach (self::FOLD_TEST_CASES as $sInput) {
            $sBytecode = (string)$oParser->parse($sInput);
            $this->assertSame(0, strlen($sBytecode));
        }
    }

    private function testTrapIllegal(Parser\SourceLine\Instruction\Statement $oParser): void {
        echo "\ttesting: trap illegal\n";
        foreach (self::TRAP_ILLEGAL_TEST_CASES as $sInput) {
            try {
                $sBytecode = (string)$oParser->parse($sInput);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf(\UnexpectedValueException::class, $oError);
            }
        }
    }
}

