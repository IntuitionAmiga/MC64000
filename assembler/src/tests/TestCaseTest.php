<?php

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

