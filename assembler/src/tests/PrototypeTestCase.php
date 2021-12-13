<?php

declare(strict_types = 1);

namespace ABadCafe\MC64K\Tests;
use ABadCafe\MC64K\Utils\TestCase;

/**
 * PrototypeTestCase
 *
 * Quickly checks key assertions
 */
class PrototypeTestCase extends TestCase {

    /**
     * @inheritDoc
     */
    public function run(): void {
        $this->assertTrue(true);
        $this->assertFalse(false);
        $this->assertEqual(1, 1);
        $this->assertInstanceOf(self::class, $this);
        try {
            $this->assertTrue(false);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\LogicException::class, $oError);
        }
        try {
            $this->assertFalse(true);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\LogicException::class, $oError);
        }
        try {
            $this->assertEqual(0, 1);
        } catch (\Throwable $oError) {
            $this->assertInstanceOf(\LogicException::class, $oError);
        }
    }
}

