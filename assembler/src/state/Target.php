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

namespace ABadCafe\MC64K\State;

use ABadCafe\MC64K\Defs;

/**
 * Target
 */
class Target {

    const F_EXECUTABLE = 1;

    private DependencySet $oDependencySet;

    private int $iFlags = 0;

    /**
     * Constructor. Accepts the build target name and version as parameters and declares them as the first
     * dependency in the set.
     *
     * @param string $sName
     * @param string $sVersion
     */
    public function __construct(string $sName, string $sVersion) {
        $this->oDependencySet = new DependencySet();
        $this->oDependencySet->add($sName, $sVersion);
    }

    /**
     * Unconditionally set target flags.
     *
     * @param  int $iFlags
     * @return self (fluent)
     */
    public function setFlags(int $iFlags): self {
        $this->iFlags |= $iFlags;
        return $this;
    }

    /**
     * Unconditionally clear target flags.
     *
     * @param  int $iFlags
     * @return self (fluent)
     */
    public function clearFlags(int $iFlags): self {
        $this->iFlags &= ~$iFlags;
        return $this;
    }

    /**
     * Obtain the target flags.
     *
     * @return int
     */
    public function getFlags(): int {
        return $this->iFlags;
    }

    /**
     * Obtain the dependency set.
     *
     * @return DependencySet
     */
    public function getDependencySet(): DependencySet {
        return $this->oDependencySet;
    }
}
