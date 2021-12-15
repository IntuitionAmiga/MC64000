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
use ABadCafe\MC64K\Utils\TestAssertionFailure;
/**
 * TestCaseTest
 *
 * Tests the test case fundamentals
 */
class TestCaseTest extends TestCase {

    /**
     * @inheritDoc
     */
    public function run(): void {
        // Postitive assertions - none of these should fail
        $this->assertTrue(true);
        $this->assertFalse(false);
        $this->assertEqual(1, 1);
        $this->assertSame($this, $this);
        $this->assertInstanceOf(self::class, $this);

        // Negative assertions - all of these should fail
        try {
            $this
                ->disregardNextAssertion()
                ->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(TestAssertionFailure::class, $oError);
        }

        try {
            $this
                ->disregardNextAssertion()
                ->assertFalse(true);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(TestAssertionFailure::class, $oError);
        }

        try {
            $this
                ->disregardNextAssertion()
                ->assertEqual(0, 1);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(TestAssertionFailure::class, $oError);
        }

        try {
            $this
                ->disregardNextAssertion()
                ->assertSame(new \stdClass, new \stdClass);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(TestAssertionFailure::class, $oError);
        }

        try {
            $this
                ->disregardNextAssertion()
                ->assertInstanceOf(self::class, new \stdClass);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(TestAssertionFailure::class, $oError);
        }
    }
}

