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

    private const TYPE_COERCE = [
        Defs\Project\IOptions::TYPE_BOOL   => '\boolval',
        Defs\Project\IOptions::TYPE_INT    => '\intval',
        Defs\Project\IOptions::TYPE_FLOAT  => '\floatval',
        Defs\Project\IOptions::TYPE_STRING => '\strval'
    ];

    /**
     * @var bool[] $aBoolOptions
     */
    private array $aBoolOptions = [];

    /**
     * @var mixed[] $aAllOptions
     */
    private array $aAllOptions = [];

    /**
     * Constructor. Ensures the statically declared defaults in IOptions are set.
     */
    public function __construct() {
        $this->import(Defs\Project\IOptions::DEFAULTS);
    }

    /**
     * Import a key-value set of options
     *
     * @param  mixed[] $aOptions
     * @return self (fluent)
     */
    public function import(array $aOptions): self {
        foreach ($aOptions as $sOption => $mValue) {
            $this->set($sOption, $mValue);
        }
        return $this;
    }

    /**
     * Enables a boolean option
     *
     * @param  string $sOption
     * @return self   fluent
     */
    public function enable(string $sOption): self {
        $this->aAllOptions[$sOption]  =
        $this->aBoolOptions[$sOption] = true;
        return $this;
    }

    /**
     * Disables a boolean option
     *
     * @param  string $sOption
     * @return self   fluent
     */
    public function disable(string $sOption): self {
        $this->aAllOptions[$sOption]  =
        $this->aBoolOptions[$sOption] = false;
        return $this;
    }

    /**
     * Checks a boolean option
     *
     * @param  string $sOption
     * @return bool   false unless specifically enabled
     */
    public function isEnabled(string $sOption): bool {
        return $this->aBoolOptions[$sOption] ?? false;
    }

    /**
     * Returns a value set for an option, or the supplied default if the option has no defined value.
     *
     * @param  string $sOption
     * @param  mixed  $mDefault
     * @return mixed
     */
    public function get(string $sOption, $mDefault = null) {
        return $this->aAllOptions[$sOption] ?? $mDefault;
    }

    /**
     * Set an option. If the option has a known type associated with the key, the corresponding value
     * will be coerced using a cast.
     *
     * @param  string $sOption
     * @param  mixed  $mValue
     * @return self   fluent
     */
    public function set(string $sOption, $mValue): self {
        $iType = Defs\Project\IOptions::TYPE_MAP[$sOption] ?? -1;
        if (isset(self::TYPE_COERCE[$iType])) {
            $cCast = self::TYPE_COERCE[$iType];
            $mValue = $cCast($mValue);
        }
        if (Defs\Project\IOptions::TYPE_BOOL === $iType) {
            $this->aBoolOptions[$sOption] = $mValue;
        }
        $this->aAllOptions[$sOption] = $mValue;
        return $this;
    }
}
