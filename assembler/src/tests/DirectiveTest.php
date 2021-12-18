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
use ABadCafe\MC64K\Parser\SourceLine\Directive\Processor;

/**
 * DirectiveTest
 *
 */
class DirectiveTest extends TestCase {

    const ALIGN_TEST_CASES = [
        ' @align 2, 2'  => "\x01\x02",
        ' @align 3, 3'  => "\x01\x02\x03",
        ' @align 4, 4'  => "\x01\x02\x03\x04",
        ' @align 5, 2'  => "\x01\x02\x03\x04",
        ' @align 6, 3'  => "\x01\x02\x03\x04\x06\x06",
        ' @align 8, 8'  => "\x01\x02\x03\x04\x06\x06\x08\x08",
        ' @align 1, 10' => "\x01\x02\x03\x04\x06\x06\x08\x08\x01\x01",
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        $this->testAlign();
        $this->testFlag();
    }

    /**
     */
    private function testAlign(): void {
        echo "\ttesting @align\n";
        $oBuffer = State\Coordinator::get()->getOutput();
        $this->assertSame(0, $oBuffer->getCurrentOffset());
        $oBuffer->appendStatement("\x01");
        $this->assertSame(1, $oBuffer->getCurrentOffset());

        $oProcessor = new Processor\Align();
        foreach (self::ALIGN_TEST_CASES as $sTestCase => $sExpect) {
            $oProcessor->process($sTestCase);
            $this->assertSame($sExpect, $oBuffer->getBytecode());
        }
    }

    private function testFlag(): void {
        echo "\ttesting @en/@enable\n";
        $oOptions = State\Coordinator::get()->getOptions();
        $this->assertFalse($oOptions->isEnabled('unit_test_flag'));
        $this->assertFalse($oOptions->isEnabled('unit_test_flag_short'));

        $oProcessor = new Processor\Flag();
        $oProcessor->process(' @enable unit_test_flag');
        $oProcessor->process(' @en unit_test_flag_short');

        $this->assertTrue($oOptions->isEnabled('unit_test_flag'));
        $this->assertTrue($oOptions->isEnabled('unit_test_flag_short'));

        echo "\ttesting @dis/@disable\n";
        $oProcessor->process(' @disable unit_test_flag');
        $oProcessor->process(' @dis unit_test_flag_short');
        $this->assertFalse($oOptions->isEnabled('unit_test_flag'));
        $this->assertFalse($oOptions->isEnabled('unit_test_flag_short'));

    }



}

