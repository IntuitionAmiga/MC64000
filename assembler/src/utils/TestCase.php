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

use function \printf, \var_export;

/**
 * TestCase
 *
 * Minimalist base for unit tests. Will be ran in a sandboxed process so that no two different test
 * classes can affect each other.
 */
abstract class TestCase {

    private int $iAssertsMade = 0;
    private int $iAssertsPass = 0;
    private int $iIncrement   = 1;

    private bool $bVerbose  = false;

    /**
     * Tests to override this.
     */
    public abstract function run(): void;

    public function __construct(bool $bVerbose = false) {
        $this->bVerbose = $bVerbose;
    }

    /**
     * Report outcome on destruction. If a test case makes no assertions, that's a red flag.
     */
    public function __destruct() {
        if ($this->iAssertsMade > 0) {
            printf("\t[Assertions %d/%d]\n", $this->iAssertsPass, $this->iAssertsMade);
        } else {
            echo "\t[No assertions made!]\n";
        }
    }

    /**
     * Called before running a test, can be overridden to set up any necessary mocks.
     */
    public function setUp(): void {

    }

    /**
     * Called after running a test, can be overridden to tidy up any necessary mocks.
     */
    public function tearDown(): void {

    }

    /**
     * Cause the next assertion to be made to be ignored in the total count.
     * Method is fluent as the intent is the very next thing to call should be the assertion to ignore.
     */
    protected final function disregardNextAssertion(): self {
        $this->iIncrement = 0;
        return $this;
    }

    protected final function assertTrue(bool $bTest): void {
        $this->iAssertsMade += $this->iIncrement;
        $this->iIncrement = 1;

        if (false === $bTest) {
            throw new TestAssertionFailure(__METHOD__ . '() input was false');
        }
        ++$this->iAssertsPass;
    }

    protected final function assertFalse(bool $bTest): void {
        $this->iAssertsMade += $this->iIncrement;
        $this->iIncrement = 1;
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
        $this->iAssertsMade += $this->iIncrement;
        $this->iIncrement = 1;
        if ($a !== $b) {
            throw new TestAssertionFailure(
                __METHOD__ . '() inputs ' .
                var_export($a, true) . ' / ' .
                var_export($b, true) . ' are different'
            );
        }
        ++$this->iAssertsPass;
    }

    /**
     * @template T
     * @param T $a
     * @param T $b
     */
    protected final function assertEqual($a, $b): void {
        $this->iAssertsMade += $this->iIncrement;
        $this->iIncrement = 1;
        if ($a != $b) {
            throw new TestAssertionFailure(
                __METHOD__ . '() inputs ' .
                var_export($a, true) . ' / ' .
                var_export($b, true) . ' are different'
            );
        }
        ++$this->iAssertsPass;
    }

    /**
     * @template T
     * @param class-string $sType
     * @param T $oEntity
     */
    protected final function assertInstanceOf(string $sType, $oEntity): void {
        $this->iAssertsMade += $this->iIncrement;
        $this->iIncrement = 1;
        if (!($oEntity instanceof $sType)) {
            throw new TestAssertionFailure(__METHOD__ . '() input is not an instance of ' . $sType);
        }
        ++$this->iAssertsPass;
    }

    /**
     * @return bool
     */
    protected function isVerbose(): bool {
        return $this->bVerbose;
    }
}
