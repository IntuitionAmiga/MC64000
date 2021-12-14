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

namespace ABadCafe\MC64K\Tokeniser;

use function \str_replace, \array_keys, \array_values, \trim, \preg_match, \preg_replace_callback, \array_map, \explode;

/**
 * Basic instruction tokeniser
 */
class Instruction {
    const
        MATCH            = '/^\s*([a-z2]+(?:\.[bwlqsd]){0,1})\s*(.*?)$/',
        MATCH_INDIRECT   = '/\(.*?\)/',
        MATCHED_MNEMONIC = 1,
        MATCHED_OPERANDS = 2
    ;

    /**
     * Tokenises an instruction statement of the form:
     *
     *   mnemonic[.size] operand1, operand2, ...
     *
     * The return is a structure containing a string field for the mnemonic and an array for the operands, in lexical order.
     * If the input text could not be tokenised, null is returned.
     *
     * @param  string $sInput
     * @return Token|null
     */
    public function tokenise(string $sInput): ?Token {
        if (preg_match(self::MATCH, $sInput, $aMatches)) {
            $sMnemonic = $aMatches[self::MATCHED_MNEMONIC];

            // There is some complexity separating out the operands because they are comma separated while at the same time
            // a comma can also be part of an operand, e.g. indexed addressing modes.
            //
            // To solve this, we first take the operand string and replace all indirect operands with placeholders, keeping
            // track of the original operand text. We then split the remaining text on commas and for each element of the
            // output, restore the original text.
            $aOperands = [];
            if (!empty($aMatches[self::MATCHED_OPERANDS])) {
                $iPlaceholderKey = 0;
                $aPlaceholderMap = [];
                $sOperands = preg_replace_callback(
                    self::MATCH_INDIRECT,
                    function(array $aMatches) use (&$iPlaceholderKey, &$aPlaceholderMap) : string {
                        $sKey = '(I:' . $iPlaceholderKey++ . ')';
                        $aPlaceholderMap[$sKey] = $aMatches[0];
                        return $sKey;
                    },
                    $aMatches[self::MATCHED_OPERANDS]
                );

                $aOperands = array_map(
                    function(string $sFragment) use (&$aPlaceholderMap) : string {
                        return str_replace(
                            array_keys($aPlaceholderMap),
                            array_values($aPlaceholderMap),
                            trim($sFragment)
                        );
                    },
                    explode(',', $sOperands)
                );

            }
            return new Token(
                $sMnemonic,
                $aOperands
            );
        }
        return null;
    }
}
