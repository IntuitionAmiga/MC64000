<?php

declare(strict_types = 1);

namespace ABadCafe\MC64K\Tests;
use ABadCafe\MC64K\Utils\TestCase;
use ABadCafe\MC64K\Tokeniser;

/**
 * TokeniserTest
 *
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

