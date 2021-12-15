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
use ABadCafe\MC64K\Tokeniser;

/**
 * TokeniserTest
 *
 * Tests the basic instruction tokeniser. The main job of the tokeniser is to split apart an instruction into it's
 * implied operation and set of operands. Operands are comma separated, but various addressing modes may contain commas
 * in their syntax, e.g. (a0, d0.w * 2). The tokeniser is not aware of specific mnemonic names.
 */
class TokeniserTest extends TestCase {

    const
        MNEMONIC = 0,
        OPERANDS = 1
    ;

    const TEST_CASES = [

        // Synthetic cases

        // No operands
        'zero' => [
            'zero', []
        ],

        // Single operand
        'one a' => [
            'one', ['a']
        ],

        // Two operands
        'two a, b' => [
            'two', ['a', 'b']
        ],

        // Three operands
        'three a, b, c' => [
            'three', ['a', 'b', 'c']
        ],

        // Four operands
        'four a, b, c, d' => [
            'four', ['a', 'b', 'c', 'd']
        ],

        'groupone (a, b)' => [
            'groupone', ['(a, b)']
        ],

        'groupone (a, b, c)' => [
            'groupone', ['(a, b, c)']
        ],

        'grouptwo a, (b, c)' => [
            'grouptwo', ['a', '(b, c)']
        ],

        'grouptwo (a, b), c' => [
            'grouptwo', ['(a, b)', 'c']
        ],

        'groupthree a, b, (c, d)' => [
            'groupthree', ['a', 'b', '(c, d)']
        ],

        'groupthree a, (b, c), d' => [
            'groupthree', ['a', '(b, c)', 'd']
        ],

        'groupthree (a, b), c, d' => [
            'groupthree', ['(a, b)', 'c', 'd']
        ],

        // Real examples
        'rts' => [
            'rts', []
        ],

        'jmp (a0)' => [
            'jmp', ['(a0)']
        ],

        'add.b #1, r0' => [
            'add.b', ['#1', 'r0']
        ],

        'sub.w r0, r1' => [
            'sub.w', ['r0', 'r1']
        ],

        'and.l r1, (r0)' => [
            'and.l', ['r1', '(r0)']
        ],

        'beq.q (r0), (r1), label1' => [
            'beq.q', ['(r0)', '(r1)', 'label1']
        ],

        'bgt.b (r1, r0.w), r0, label2' => [
            'bgt.b', ['(r1, r0.w)', 'r0', 'label2']
        ],

        'ble.q 4(r1), 4(r0, r2.w), label3' => [
            'ble.q', ['4(r1)', '4(r0, r2.w)', 'label3']
        ],

        'flog2.d (r0, r1.w * 2), 4(r0, r2.w)' => [
            'flog2.d', ['(r0, r1.w * 2)', '4(r0, r2.w)']
        ]
    ];

    /**
     * @inheritDoc
     */
    public function run(): void {
        $oTokeniser = new Tokeniser\Instruction;
        foreach (self::TEST_CASES as $sInput => $aResult) {
            $oToken = $oTokeniser->tokenise($sInput);
            if (null === $oToken) {
                throw new \Exception();
            }
            $this->assertInstanceOf(Tokeniser\Token::class, $oToken);
            $this->assertEqual($aResult[self::MNEMONIC], $oToken->sMnemonic);
            $this->assertEqual(count($aResult[self::OPERANDS]), count($oToken->aOperands));
            foreach ($aResult[self::OPERANDS] as $i => $sOperand) {
                $this->assertEqual($sOperand, $oToken->aOperands[$i]);
            }
        }
    }
}

