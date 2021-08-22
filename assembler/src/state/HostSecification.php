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
 * HostSpecification
 */
class HostSpecification {

    /**
     * Valid host ID names must be 4-255 non-control ASCII characters (32-126)
     */
    const HOST_ID_MATCH = '/^[\x20-\x7E]{4,255}$/';
    const VERSION_MATCH = '/^(\d+)\.(\d+)\.(\d+)$/';

    private string $sName;
    private array  $aMinVersion, $aMaxVersion;

    /**
     * @param string $sName
     * @param string $sMinVersion
     * @param string $sMaxVersion
     */
    public function __construct(string $sName, string $sMinVersion, string $sMaxVersion) {
        if (!preg_match(self::HOST_ID_MATCH, $sName)) {
            throw new \Exception("Invalid host identifier '" . $sName . "'");
        }
        $this->sName = $sName;
        preg_match_all(self::VERSION_MATCH, $sMinVersion, $aMatches);
        $this->aMinVersion = [
            (int)($aMatches[1][0] ?? 0),
            (int)($aMatches[2][0] ?? 0),
            (int)($aMatches[3][0] ?? 0)
        ];
        preg_match_all(self::VERSION_MATCH, $sMaxVersion, $aMatches);
        $this->aMaxVersion = [
            (int)($aMatches[1][0] ?? 0),
            (int)($aMatches[2][0] ?? 0),
            (int)($aMatches[3][0] ?? 0)
        ];

    }
}
