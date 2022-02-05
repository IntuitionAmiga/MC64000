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
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\IO;
use ABadCafe\MC64K\Defs;

/**
 * LabelLocationTest
 *
 * Tests the fundamental behaviours of LabelLocation
 */
class LabelLocationTest extends TestCase {

    const ADD_GLOBAL_TESTS = [
        'global_0' => 2,
        'global_1' => 3,
        'global_2' => 5,
        'global_3' => 7,
        'global_4' => 11
    ];

    const ADD_LOCAL_TESTS = [
        '.local_0' => 2,
        '.local_1' => 3,
        '.local_2' => 5,
        '.local_3' => 7,
        '.local_4' => 11
    ];

    const ADD_GLOBAL_DUPLICATE_TESTS = [
        'global_3' => 8,
        'global_4' => 13
    ];

    const ADD_LOCAL_DUPLICATE_TESTS = [
        '.local_3' => 8,
        '.local_4' => 13
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        $this->testLabelLengthValidation();
        $this->testReservedWordThrows();
        $this->testAddGlobal();
        $this->testAddGlobalDuplicates();
        $this->testAddLocal();
        $this->testAddLocalDuplicateSameFileThrows();
        $this->testAddLocalDuplicateDifferentFile();
    }

    private function testLabelLengthValidation(): void {
        echo "\ttesting: label length validation\n";
        $oMockSource = new IO\SourceString('', __METHOD__);
        $oLabelLocation = new State\LabelLocation();
        try {
            $sTooShort = '';
            $oLabelLocation->addGlobal($oMockSource, $sTooShort, 0);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\LengthException::class, $oError);
        }

        $sMaxLabel = str_repeat('l', Defs\ILabel::MAX_LENGTH);
        $oLabelLocation->addGlobal($oMockSource, $sMaxLabel, 17);
        $this->assertSame(17, $oLabelLocation->getPositionForGlobal($sMaxLabel));

        try {
            $sTooLong = str_repeat('x', Defs\ILabel::MAX_LENGTH + 1);
            $oLabelLocation->addGlobal($oMockSource, $sTooLong, 0);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\LengthException::class, $oError);
        }
    }

    private function testReservedWordThrows(): void {
        echo "\ttesting: reserved word throws\n";
        $oMockSource = new IO\SourceString('', __METHOD__);
        $oLabelLocation = new State\LabelLocation();

        $aNames = array_keys(Defs\Register\INames::GPR_MAP) + array_keys(Defs\Register\INames::FPR_MAP);

        foreach ($aNames as $sReserved) {
            try {
                $oLabelLocation->addGlobal($oMockSource, $sReserved, 0);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf(\InvalidArgumentException::class, $oError);
            }
        }
    }

    private function testAddGlobal(): void {
        echo "\ttesting: add global\n";
        $oMockSource = new IO\SourceString('', __METHOD__);
        $oLabelLocation = new State\LabelLocation();
        foreach (self::ADD_GLOBAL_TESTS as $sLabel => $iPosition) {
            $oLabelLocation->addGlobal($oMockSource, $sLabel, $iPosition);
            $this->assertSame($iPosition, $oLabelLocation->getPositionForGlobal($sLabel));
        }
    }

    private function testAddLocal(): void {
        echo "\ttesting: add local\n";
        $oMockSource = new IO\SourceString('', __METHOD__);
        $oLabelLocation = new State\LabelLocation();
        foreach (self::ADD_LOCAL_TESTS as $sLabel => $iPosition) {
            $oLabelLocation->addLocal($oMockSource, $sLabel, $iPosition);
            $this->assertSame($iPosition, $oLabelLocation->getPositionForLocal($oMockSource, $sLabel));
        }
    }

    private function testAddLocalDuplicateSameFileThrows(): void {
        echo "\ttesting: add local duplicate in same file throws\n";
        $oMockSource = new IO\SourceString('', __METHOD__);
        $oLabelLocation = new State\LabelLocation();
        foreach (self::ADD_LOCAL_TESTS as $sLabel => $iPosition) {
            $oLabelLocation->addLocal($oMockSource, $sLabel, $iPosition);
            $this->assertSame($iPosition, $oLabelLocation->getPositionForLocal($oMockSource, $sLabel));
        }
        foreach (self::ADD_LOCAL_DUPLICATE_TESTS as $sLabel => $iPosition) {
            try {
                $oLabelLocation->addLocal($oMockSource, $sLabel, $iPosition);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf(\RuntimeException::class, $oError);
            }
        }
    }

    private function testAddLocalDuplicateDifferentFile(): void {
        echo "\ttesting: add local duplicate in different file\n";
        $oMockSource1 = new IO\SourceString('', __METHOD__ . '_1');
        $oLabelLocation = new State\LabelLocation();
        foreach (self::ADD_LOCAL_TESTS as $sLabel => $iPosition) {
            $oLabelLocation->addLocal($oMockSource1, $sLabel, $iPosition);
            $this->assertSame($iPosition, $oLabelLocation->getPositionForLocal($oMockSource1, $sLabel));
        }

        $oMockSource2 = new IO\SourceString('', __METHOD__ . '_2');
        foreach (self::ADD_LOCAL_DUPLICATE_TESTS as $sLabel => $iPosition) {
            $oLabelLocation->addLocal($oMockSource2, $sLabel, $iPosition);
            $this->assertSame($iPosition, $oLabelLocation->getPositionForLocal($oMockSource2, $sLabel));
        }
    }

    private function testAddGlobalDuplicates(): void {
        echo "\ttesting: add global duplicate throws\n";
        $oMockSource1   = new IO\SourceString('', __METHOD__ . '_1');
        $oLabelLocation = new State\LabelLocation();
        foreach (self::ADD_GLOBAL_TESTS as $sLabel => $iPosition) {
            $oLabelLocation->addGlobal($oMockSource1, $sLabel, $iPosition);
        }

        // Use a different source for the global duplicate tests as globals should always collide.
        $oMockSource2   = new IO\SourceString('', __METHOD__ . '_2');
        foreach (self::ADD_GLOBAL_DUPLICATE_TESTS as $sLabel => $iPosition) {
            try {
                $oLabelLocation->addGlobal($oMockSource2, $sLabel, $iPosition);
            } catch (\Throwable $oError) {
                $this->assertInstanceOf(\RuntimeException::class, $oError);
            }
        }
    }
}

