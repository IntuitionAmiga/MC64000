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

use function \strlen, \uasort, \str_replace, \array_keys, \array_values;

/**
 * DefinitionSet
 */
class DefinitionSet {

    private array $aDefinitions = [];
    private bool  $bolSorted    = false;

    /**
     * Constructor
     */
    public function __construct(array $aDefinitions = []) {
        $this->aDefinitions = $aDefinitions;
    }

    /**
     * Add a new definition. Throws an exception if the definition already exists.
     *
     * @param  string $sDefinition
     * @param  string $sValue
     * @return self   fluent
     * @throws \Exception
     */
    public function add(string $sDefinition, string $sValue) : self {
        if (isset($this->aDefinitions[$sDefinition])) {
            throw new \Exception("Duplicate definition");
        }
        $this->aDefinitions[$sDefinition] = $sValue;
        $this->bolSorted = false;
        return $this;
    }

    /**
     * Silently removes a definition.
     *
     * @param  string $sDefinition
     * @return self   fluent
     */
    public function remove($sDefinition) : self {
        unset($this->aDefinitions[$sDefinition]);
        return $this;
    }

    /**
     * Returns the set of current definitions. These are sorted value longest first.
     * The return is associative.
     *
     * @return string[]
     */
    public function getDefinitions() : array {
        if (false === $this->bolSorted) {
            uasort(
                $this->aDefinitions,
                function (string $sA, string $sB) {
                    return strlen($sA) - strlen($sB);
                }
            );
            $this->bolSorted = true;
        }
        return $this->aDefinitions;
    }

    /**
     * Apply the corrent definition set to a string performing a set of search and replace terms.
     *
     * @todo This needs doing properly. The current implementation is a dumb search and replace that is not
     * sensible - it will result in later search terms overwriting earlier ones. What we should do is to
     * search and replace with protector expressions first, until all the searches are done, then substitute
     * each distinct protector expression with the corresponding value.
     *
     * @param  string $sInput
     * @return string
     */
    public function applyTo(string $sInput) : string {
        if (empty($this->aDefinitions)) {
            return $sInput;
        }
        $aDefinitions = $this->getDefinitions();
        return str_replace(array_keys($aDefinitions), array_values($aDefinitions), $sInput);
    }
}
