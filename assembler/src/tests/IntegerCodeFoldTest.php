<?php

declare(strict_types = 1);

namespace ABadCafe\MC64K\Tests;
use ABadCafe\MC64K\Utils\TestCase;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Parser;

/**
 * IntegerCodeFoldTest
 *
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
     * Map of array of potentially foldable integer operation test cases along with their expected outcomes.
     *
     *    ILLEGAL_VALUE when we expect \UnexpectedValueException to be thrown
     *    PASS          when we expect code to be generated.
     *    CLEAR_B/W/L/Q when we expect the code to be folded to an equivalent clear operation.
     *    FOLD          when we expect the code to be folded completely.
     */
    const TEST_CASES = [
        'clr.b r0'      => self::CLEAR_B,
        'clr.w r0'      => self::CLEAR_W,
        'clr.l r0'      => self::CLEAR_L,
        'clr.q r0'      => self::CLEAR_Q,
        'and.b #1, r0'  => self::PASS,
        'and.w #1, r0'  => self::PASS,
        'and.l #1, r0'  => self::PASS,
        'and.q #1, r0'  => self::PASS,
        'and.b #0, r0'  => self::CLEAR_B,
        'and.w #0, r0'  => self::CLEAR_W,
        'and.l #0, r0'  => self::CLEAR_L,
        'and.q #0, r0'  => self::CLEAR_Q,
        'or.b #1, r0'   => self::PASS,
        'or.w #1, r0'   => self::PASS,
        'or.l #1, r0'   => self::PASS,
        'or.q #1, r0'   => self::PASS,
        'or.b #0, r0'   => self::FOLD,
        'or.w #0, r0'   => self::FOLD,
        'or.l #0, r0'   => self::FOLD,
        'or.q #0, r0'   => self::FOLD,
        'lsl.b #1, r0'  => self::PASS,
        'lsl.w #1, r0'  => self::PASS,
        'lsl.l #1, r0'  => self::PASS,
        'lsl.q #1, r0'  => self::PASS,
        'lsl.b #0, r0'  => self::FOLD,
        'lsl.w #0, r0'  => self::FOLD,
        'lsl.l #0, r0'  => self::FOLD,
        'lsl.q #0, r0'  => self::FOLD,
        'lsr.b #1, r0'  => self::PASS,
        'lsr.w #1, r0'  => self::PASS,
        'lsr.l #1, r0'  => self::PASS,
        'lsr.q #1, r0'  => self::PASS,
        'lsr.b #0, r0'  => self::FOLD,
        'lsr.w #0, r0'  => self::FOLD,
        'lsr.l #0, r0'  => self::FOLD,
        'lsr.q #0, r0'  => self::FOLD,
        'add.b #1, r0'  => self::PASS,
        'add.w #1, r0'  => self::PASS,
        'add.l #1, r0'  => self::PASS,
        'add.q #1, r0'  => self::PASS,
        'add.b #0, r0'  => self::FOLD,
        'add.w #0, r0'  => self::FOLD,
        'add.l #0, r0'  => self::FOLD,
        'add.q #0, r0'  => self::FOLD,
        'sub.b #1, r0'  => self::PASS,
        'sub.w #1, r0'  => self::PASS,
        'sub.l #1, r0'  => self::PASS,
        'sub.q #1, r0'  => self::PASS,
        'sub.b #0, r0'  => self::FOLD,
        'sub.w #0, r0'  => self::FOLD,
        'sub.l #0, r0'  => self::FOLD,
        'sub.q #0, r0'  => self::FOLD,
        'muls.b #3, r0' => self::PASS,
        'muls.w #3, r0' => self::PASS,
        'muls.l #3, r0' => self::PASS,
        'muls.q #3, r0' => self::PASS,
        'muls.b #1, r0' => self::FOLD,
        'muls.w #1, r0' => self::FOLD,
        'muls.l #1, r0' => self::FOLD,
        'muls.q #1, r0' => self::FOLD,
        'muls.b #0, r0' => self::CLEAR_B,
        'muls.w #0, r0' => self::CLEAR_W,
        'muls.l #0, r0' => self::CLEAR_L,
        'muls.q #0, r0' => self::CLEAR_Q,
        'mulu.b #3, r0' => self::PASS,
        'mulu.w #3, r0' => self::PASS,
        'mulu.l #3, r0' => self::PASS,
        'mulu.q #3, r0' => self::PASS,
        'mulu.b #1, r0' => self::FOLD,
        'mulu.w #1, r0' => self::FOLD,
        'mulu.l #1, r0' => self::FOLD,
        'mulu.q #1, r0' => self::FOLD,
        'mulu.b #0, r0' => self::CLEAR_B,
        'mulu.w #0, r0' => self::CLEAR_W,
        'mulu.l #0, r0' => self::CLEAR_L,
        'mulu.q #0, r0' => self::CLEAR_Q,
        'divs.l #3, r0' => self::PASS,
        'divs.q #3, r0' => self::PASS,
        'divs.l #1, r0' => self::FOLD,
        'divs.q #1, r0' => self::FOLD,
        'divs.l #0, r0' => self::ILLEGAL_VALUE,
        'divs.q #0, r0' => self::ILLEGAL_VALUE,
        'divu.l #3, r0' => self::PASS,
        'divu.q #3, r0' => self::PASS,
        'divu.l #1, r0' => self::FOLD,
        'divu.q #1, r0' => self::FOLD,
        'divu.l #0, r0' => self::ILLEGAL_VALUE,
        'divu.q #0, r0' => self::ILLEGAL_VALUE,
    ];


    /**
     * @inheritDoc
     */
    public function run(): void {

        $aFoldBytecode = [
            self::CLEAR_B => chr(Defs\Mnemonic\IDataMove::CLR_B) . chr(0),
            self::CLEAR_W => chr(Defs\Mnemonic\IDataMove::CLR_W) . chr(0),
            self::CLEAR_L => chr(Defs\Mnemonic\IDataMove::CLR_L) . chr(0),
            self::CLEAR_Q => chr(Defs\Mnemonic\IDataMove::CLR_Q) . chr(0)
        ];

        $oParser = new Parser\SourceLine\Instruction\Statement();
        foreach (self::TEST_CASES as $sInput => $iExpect) {
            try {
                $sBytecode = (string)$oParser->parse($sInput);

                switch ($iExpect) {
                    case self::PASS:
                        $this->assertTrue(0 < strlen($sBytecode));
                        break;
                    case self::FOLD:
                        $this->assertTrue(0 === strlen($sBytecode));
                        break;
                    case self::CLEAR_B:
                    case self::CLEAR_W:
                    case self::CLEAR_L:
                    case self::CLEAR_Q:
                        $this->assertSame($aFoldBytecode[$iExpect], $sBytecode);
                        break;
                    default;
                        $this->assertTrue(false);
                        break;
                }
            } catch (\UnexpectedValueException $oError) {
                $this->assertEqual(self::ILLEGAL_VALUE, $iExpect);
            }
        }
    }
}

