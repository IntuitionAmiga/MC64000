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

namespace ABadCafe\MC64K\Utils;

/**
 * TestCase
 */
abstract class TestCase {

    private int $iAssertsMade = 0;
    private int $iAssertsPass = 0;

    public abstract function run(): void;

    public function __destruct() {
        printf("[Assertions %d/%d]\n", $this->iAssertsPass, $this->iAssertsMade);
    }

    public function setUp(): void {

    }

    public function tearDown(): void {

    }

    protected final function assertTrue(bool $bTest): void {
        ++$this->iAssertsMade;
        if (false === $bTest) {
            throw new TestAssertionFailure(__METHOD__ . '() input was false');
        }
        ++$this->iAssertsPass;
    }

    protected final function assertFalse(bool $bTest): void {
        ++$this->iAssertsMade;
        if (false !== $bTest) {
            throw new TestAssertionFailure(__METHOD__ . '() input was true');
        }
        ++$this->iAssertsPass;
    }

    /**
     * @template T
     * @param T $a
     * @param T $b
     */
    protected final function assertSame($a, $b): void {
        ++$this->iAssertsMade;
        if ($a !== $b) {
            throw new TestAssertionFailure(__METHOD__ . '() inputs are different');
        }
        ++$this->iAssertsPass;
    }

    /**
     * @template T
     * @param T $a
     * @param T $b
     */
    protected final function assertEqual($a, $b): void {
        ++$this->iAssertsMade;
        if ($a != $b) {
            throw new TestAssertionFailure(__METHOD__ . '() inputs are different');
        }
        ++$this->iAssertsPass;
    }

    /**
     * @template T
     * @param class-string $sType
     * @param T $oEntity
     */
    protected final function assertInstanceOf(string $sType, $oEntity): void {
        ++$this->iAssertsMade;
        if (!($oEntity instanceof $sType)) {
            throw new TestAssertionFailure(__METHOD__ . '() input is not an instance of ' . $sType);
        }
        ++$this->iAssertsPass;
    }
}
