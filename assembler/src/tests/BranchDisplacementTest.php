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
use ABadCafe\MC64K\Defs\Mnemonic;
use ABadCafe\MC64K\Parser;

use function \chr;

/**
 * BranchDisplacementTest
 *
 * Tests basic branch displacement fundamentals. Branch displacements are measured from the end of the
 * instruction bytecode such that a distance of zero is the very next position in the bytecode stream.
 * This means that negative displacements must be further than the branch instruction length to be
 * valid.
 */
class BranchDisplacementTest extends TestCase {

    /**
     * Short branches have 1-byte displacements. An unconditional short branch instruction is 2 bytes.
     */
    const SHORT_BRANCH_PASS_CASES = [
        'bra.b #1'    => "\x01",
        'bra.b #127'  => "\x7f",
        'bra.b #-3'   => "\xfd",
        'bra.b #-128' => "\x80",
    ];

    const SHORT_BRANCH_FAIL_CASES = [
        'bra.b #128'  => \RangeException::class,
        'bra.b #-1'   => \UnexpectedValueException::class,
        'bra.b #-2'   => \UnexpectedValueException::class,
        'bra.b #-129' => \RangeException::class,
        'bra.b .lbl'  => \UnexpectedValueException::class
    ];

    /**
     * Normal branches have 4-byte displacements. An unconditional branch instruction is 5 bytes.
     */
    const BRANCH_PASS_CASES = [
        'bra #1'     => "\x01\x00\x00\x00",
        'bra #127'   => "\x7f\x00\x00\x00",
        'bra #128'   => "\x80\x00\x00\x00",
        'bra #-6'    => "\xfa\xff\xff\xff",
        'bra #-129'  => "\x7f\xff\xff\xff",
    ];

    const BRANCH_FAIL_CASES = [
        'bra #-1'   => \UnexpectedValueException::class,
        'bra #-2'   => \UnexpectedValueException::class,
        'bra #-3'   => \UnexpectedValueException::class,
        'bra #-4'   => \UnexpectedValueException::class,
        'bra #-5'   => \UnexpectedValueException::class
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        $this->testShortBranch();
        $this->testBranch();
    }

    /**
     * Tests regular (4-byte) displacement. Basic bra is used which allows the total bytecode emitted
     * to be validated. Also tests that a zero branch is completely folded out.
     *
     * Since branch displacements are relative to the end of the instruction bytecode, negative displacments
     * less than or equal to the instruction length are illegal and should throw an exception. This is also
     * tested for.
     */
     private function testBranch(): void {
        if ($this->isVerbose()) {
            echo "\ttesting bra valid cases\n";
        }
        $oParser = new Parser\SourceLine\Instruction\Statement();

        // A zero branch is never emitted but would be folded out
        $this->assertTrue(empty($oParser->parse('bra #0')));

        $sOpcode = chr(Mnemonic\IControl::BRA);
        foreach (self::BRANCH_PASS_CASES as $sTestCase => $sDisplacement) {
            $sByteCode = $sOpcode . $sDisplacement;
            $this->assertSame($sByteCode, $oParser->parse($sTestCase));
        }

        if ($this->isVerbose()) {
            echo "\ttesting bra invalid cases\n";
        }
        foreach (self::BRANCH_FAIL_CASES as $sTestCase => $sErrorClass) {
            try {
                $oParser->parse($sTestCase);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf($sErrorClass, $oError);
            }
        }
    }

    /**
     * Tests short (1-byte) displacement. Basic bra.b is used which allows the total bytecode emitted
     * to be validated. Also tests that a zero branch is completely folded out.
     *
     * Since branch displacements are relative to the end of the instruction bytecode, negative displacments
     * less than or equal to the instruction length are illegal and should throw an exception. This is also
     * tested for.
     */
    private function testShortBranch(): void {
        if ($this->isVerbose()) {
            echo "\ttesting bra.b valid cases\n";
        }
        $oParser = new Parser\SourceLine\Instruction\Statement();

        // A zero branch is never emitted but would be folded out
        $this->assertTrue(empty($oParser->parse('bra.b #0')));

        $sOpcode = chr(Mnemonic\IControl::BRA_B);
        foreach (self::SHORT_BRANCH_PASS_CASES as $sTestCase => $sDisplacement) {
            $sByteCode = $sOpcode . $sDisplacement;
            $this->assertSame($sByteCode, $oParser->parse($sTestCase));
        }
        if ($this->isVerbose()) {
            echo "\ttesting bra.b invalid cases\n";
        }
        foreach (self::SHORT_BRANCH_FAIL_CASES as $sTestCase => $sErrorClass) {
            try {
                $oParser->parse($sTestCase);
                $this->assertTrue(false);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf($sErrorClass, $oError);
            }
        }
    }
}

