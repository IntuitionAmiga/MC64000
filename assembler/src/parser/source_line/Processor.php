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

namespace ABadCafe\MC64K\Parser\SourceLine;

use ABadCafe\MC64K\State;
use ABadCafe\MC64K\IIntLimits;
use ABadCafe\MC64K\Parser\Utils;
use function \preg_replace, \preg_replace_callback, \rtrim;

/**
 * Processor
 *
 * This is basically a composite implementation of parsers that handle different types of line,
 * e.g. instruction statements, local labels, global labels etc.
 */
class Processor implements IParser {

    const COMMENT_MATCH = '/(?:;|\/\/).*$/';

    /** @var IParser[] $aParsers */
    private array $aParsers = [];

    private Utils\ConstIntExpression   $oConstIntExpression;
    private Utils\ConstFloatExpression $oConstFloatExpression;

    public function __construct() {
        $this->aParsers = [
            new Directive\Statement(),
            new Data\Declaration(),
            new Instruction\Statement(),
            new Label\Local(),
            new Label\Exported()
        ];
        $this->oConstIntExpression   = new Utils\ConstIntExpression();
        $this->oConstFloatExpression = new Utils\ConstFloatExpression();
    }

    /**
     * Stub - always true
     *
     * @inheritDoc
     */
    public function checkLine(string $sLine): bool {
        return true;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSourceLine): ?string {
        $sSourceLine = $this->preprocessSourceLine($sSourceLine);
        if (empty($sSourceLine)) {
            return '';
        }
        foreach ($this->aParsers as $oParser) {
            if ($oParser->checkLine($sSourceLine)) {
                return $oParser->parse($sSourceLine);
            }
        }
        return '';
    }

    /**
     * Removes any comment guff and handles any definition preprocessing
     *
     * @param  string $sSourceLine
     * @return string
     */
    private function preprocessSourceLine(string $sSourceLine): string {

        // 1. Protect string literals/labels (extract and replace with anchor).
        $aPlaceholderMap = [];
        $sIntermediate = $this->protectStrings($sSourceLine, $aPlaceholderMap);

        // 2. Remove end of line comments
        $sIntermediate = rtrim((string)preg_replace(self::COMMENT_MATCH, '', $sIntermediate));

        // 3. Apply @equ/@def search/replace terms.
        $sIntermediate = State\Coordinator::get()
            ->getDefinitionSet()
            ->applyTo($sIntermediate);

        // 4. Identify any evaluatable constant expressions and substitute.
        $sIntermediate = $this->oConstFloatExpression->parse($sIntermediate);
        $sIntermediate = $this->oConstIntExpression->parse($sIntermediate);

        // 5. Reinstate protected strings and return.
        return str_replace(array_keys($aPlaceholderMap), array_values($aPlaceholderMap), $sIntermediate);
    }

    /**
     * Replaces sensitive strings (literals, labels, etc) with anchors and builds a map of the anchor to
     * original string.
     *
     * @param  string $sSourceLine
     * @param  array<string, string> $aPlaceholderMap
     * @return string
     */
    private function protectStrings(string $sSourceLine, array &$aPlaceholderMap): string {
        static $iPlaceholderKey = 0;
        return (string)preg_replace_callback(
            '/".*?"|[a-zA-Z0-9_]+\:$/',
            function(array $aMatches) use (&$iPlaceholderKey, &$aPlaceholderMap): string {
                $sKey = '{S:' . $iPlaceholderKey++ . '}';
                $aPlaceholderMap[$sKey] = $aMatches[0];
                return $sKey;
            },
            $sSourceLine
        );
    }
}
