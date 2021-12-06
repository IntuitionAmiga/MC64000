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

namespace ABadCafe\MC64K\Parser\SourceLine\Directive;
use ABadCafe\MC64K\Parser\SourceLine;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs;

use function \preg_match, \strtolower, \trim;

/**
 * Statement
 *
 * Source Line Parser for Directive Statements
 */
class Statement implements SourceLine\IParser {

    private const
        BASIC_LINE_MATCH = '/^\s+@([a-zA-Z_]+)\s+/'
    ;

    /**
     * @var IProcessor[] $aDirectives
     */
    private array $aDirectives = [];

    /**
     * Constructor
     */
    public function __construct() {
        $this->addDirective(new Processor\Flag());
        $this->addDirective(new Processor\Define());
        $this->addDirective(new Processor\Undefine());
        $this->addDirective(new Processor\Export());
        $this->addDirective(new Processor\Import());
        $this->addDirective(new Processor\Align());
        $this->addDirective(new Processor\StackSize());
    }

    /**
     * Implements SourceLine\IParser::checkLine()
     */
    public function checkLine(string $sLine): bool {
        if (preg_match(self::BASIC_LINE_MATCH, $sLine, $aMatch)) {
            $sKeyword = strtolower($aMatch[1]);
            if (isset($this->aDirectives[$sKeyword])) {
                $this->aDirectives[$sKeyword]->process($sLine);
                return true;
            }
        }
        return false;
    }

    /**
     * @inheritDoc
     */
    public function parse(string $sSource): ?string {
        return '';
    }

    /**
     * Adds a directive, associating it with the keywords it exports.
     *
     * @param IProcessor $oDirective
     */
    private function addDirective(IProcessor $oDirective): void {
        foreach ($oDirective->getMatchingKeywords() as $sKeyword) {
            $this->aDirectives[$sKeyword] = $oDirective;
        }
    }
}

