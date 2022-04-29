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

namespace ABadCafe\MC64K\Parser\Utils;

use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\IParser;
use ABadCafe\MC64K\Utils\Log;
use ABadCafe\MC64K\State;

use function \array_keys, \array_values, \is_numeric, \preg_match, \preg_replace_callback, \strlen, \str_replace, \substr_count;

/**
 * ConstFloatExpression
 *
 * Utility class for solving some simplistic expressions.
 *
 * For simplicity, we reserve braces for expressions. Otherwise we run into some challenges with our
 * indirect addressing syntax that is more effort to fix than this is to live with.
 */
class ConstFloatExpression implements IParser {

    // First character must be a an open brace
    private const MATCH_FIRST   = '\{';

    // Further charcters can be digits, binary operators, open or closing parenthesis or space
    private const MATCH_FURTHER = '[0-9\.\+\-\*\\/\{\}\s]{0,}';

    // Final character must be a close brace
    private const MATCH_LAST    = '\}';

    // Final match expression
    private const MATCH_FULL    = '/(' . self::MATCH_FIRST . self::MATCH_FURTHER . self::MATCH_LAST . ')/';

    /**
     * @inheritDoc
     */
    public function parse(string $sSource): string {

        // If the entire string lacks either digits or operator, just bail now
        if (!$this->hasRequired($sSource)) {
            return $sSource;
        }

        $bLogEval = State\Coordinator::get()->getOptions()->isEnabled(Defs\Project\IOptions::LOG_EXPR_EVAL);

        // Extract from the string all the parts that look like they may be expressions.
        static $iPlaceholderKey = 0;

        $aPlaceholderMap = [];
        $sIntermediate = (string)preg_replace_callback(
            self::MATCH_FULL,
            function(array $aMatches) use (&$iPlaceholderKey, &$aPlaceholderMap): string {
                $sKey = '<=<F:' . $iPlaceholderKey++ . '>=>';
                $aPlaceholderMap[$sKey] = (string)$aMatches[0];
                return $sKey;
            },
            $sSource
        );

        /** @var array<string, string> $aPlaceholderMap : phpstan 1.4.9 gets confused here */

        // For each possible expression, first check that it still has digits and an operator and if it
        // does, try to evaluate it.
        foreach ($aPlaceholderMap as $sKey => $sPossibleExpression) {
            if (
                $this->hasRequired($sPossibleExpression)
            ) {
                // By now we are pretty sure we are in an expression so we can get more assertive.
                $this->assertBalanced($sPossibleExpression);
                try {
                    /** @var string|null */
                    $sResult = null;
                    $sPossibleExpression = str_replace(
                        ['{', '}'],
                        ['(', ')'],
                        $sPossibleExpression
                    );

                    // This is where the evil happens...
                    eval('$sResult = ' . $sPossibleExpression . ';');

                    // If the evaluation worked, we replace the extracted expression with the integer result.
                    if (is_numeric($sResult)) {
                        $fResult = floatval($sResult);
                        $aPlaceholderMap[$sKey] = $fResult;

                        if ($bLogEval) {
                            Log::printf(
                                'Evaluated constant expression \'%s\' to value \'%f\' in %s',
                                $sPossibleExpression,
                                $fResult,
                                $sSource
                            );
                        }
                    }
                } catch (\Throwable $oError) {
                    throw new \ParseError(
                        'Error evaluating expression ' . $sPossibleExpression .
                        ': ' . $oError->getMessage()
                    );
                }
            }
        }
        return str_replace(array_keys($aPlaceholderMap), array_values($aPlaceholderMap), $sIntermediate);
    }

    /**
     * Checks that the string contains both digits and operators, or there is little point in doing the effort.
     */
    private function hasRequired(string $sPossibleExpression): bool {
        return preg_match('/\d/', $sPossibleExpression) &&
               preg_match('/[\+\-\*\\/\<\>]/', $sPossibleExpression);
    }

    /**
     * Checks that any parenthesis in the string is balanced. At each character position, the open bracket
     * count must be greater than, or equal to the close bracket count.
     */
    private function assertBalanced(string $sPossibleExpression): void {
        if (preg_match("/\{\s*\}/", $sPossibleExpression)) {
            throw new \ParseError("Empty parenthesis in expression " . $sPossibleExpression);
        }
        $iOpenCount  = 0;
        $iCloseCount = 0;
        $iLength = strlen($sPossibleExpression);
        for ($i = 0; $i < $iLength; $i++) {
            $iOpenCount  += (int)($sPossibleExpression[$i] === '{');
            $iCloseCount += (int)($sPossibleExpression[$i] === '}');
            if ($iCloseCount > $iOpenCount) {
                throw new \ParseError("Unbalanced parenthesis in expression " . $sPossibleExpression);
            }
        }
        if ($iOpenCount !== $iCloseCount) {
            throw new \ParseError("Unbalanced parenthesis in expression " . $sPossibleExpression);
        }
    }
}
