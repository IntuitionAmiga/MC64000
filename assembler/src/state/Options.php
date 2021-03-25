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
 * Options
 */
class Options {

    /**
     * @var bool[] $aBoolOptions
     */
    private array $aBoolOptions = [];

    /**
     * @var mixed[] $aOtherOptions
     */
    private array $aOtherOptions = [];

    /**
     * Import a key-value set of options
     */
    public function import(array $aOptions) : self {
        foreach ($aOptions as $sOption => $mValue) {
            $iKind = Defs\Project\IOptions::TYPE_MAP[$sOption] ?? -1;
            switch ($iKind) {
                case Defs\Project\IOptions::TYPE_BOOL:
                    $this->aBoolOptions[$sOption] = (bool)$mValue;
                    break;
                default:
                    $this->aOtherOptions[$sOption] = $mValue;
                    break;
            }
        }
        return $this;
    }

    /**
     * Enables a boolean option
     *
     * @param  string $sOption
     * @return self   fluent
     */
    public function enable(string $sOption) : self {
        $this->aBoolOptions[$sOption] = true;
        return $this;
    }

    /**
     * Disables a boolean option
     *
     * @param  string $sOption
     * @return self   fluent
     */
    public function disable(string $sOption) : self {
        $this->aBoolOptions[$sOption] = false;
        return $this;
    }

    /**
     * Checks a boolean option
     *
     * @param  string $sOption
     * @return bool   false unless specifically enabled
     */
    public function isEnabled(string $sOption) : bool {
        return $this->aBoolOptions[$sOption] ?? false;
    }
}
