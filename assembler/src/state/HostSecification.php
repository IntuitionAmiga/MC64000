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

use function \preg_match, \preg_match_all;

/**
 * HostSpecification
 */
class HostSpecification {

    /**
     * Valid host ID names must be 4-255 non-control ASCII characters (32-126)
     */
    const
        HOST_ID_MATCH = '/^[\x20-\x7E]{4,255}$/',
        VERSION_MATCH = '/^(\d+)\.(\d+)\.(\d+)$/'
    ;

    const
        // Version component match positions
        MAJOR = 0,
        MINOR = 1,
        PATCH = 2,

        // Version numbers are 32-bit fields 12[major]:10[minor]:10[patch]
        MAJOR_BITS = 12,
        MINOR_BITS = 10,
        PATCH_BITS = 10
    ;

    const VERSION_LIMITS = [
        self::MAJOR => (1 << self::MAJOR_BITS) - 1,
        self::MINOR => (1 << self::MINOR_BITS) - 1,
        self::PATCH => (1 << self::PATCH_BITS) - 1
    ];

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
        $this->aMinVersion = $this->parseVersionNumber($sMinVersion);
        $this->aMaxVersion = $this->parseVersionNumber($sMaxVersion);
    }

    /**
     * Parses a version string. Expects a dot concatenated triplet: major.minor.patch. If any given
     * component is missing, zero is assumed. Each version component is then subject to a range check
     * based on the 12:10:10 format requirements.
     *
     * @param  string $sVersion
     * @return int[3]
     * @throws \RangeException
     */
    private function parseVersionNumber(string $sVersion): array {
        preg_match_all(self::VERSION_MATCH, $sVersion, $aMatches);
        $aVersion = [
            self::MAJOR => (int)($aMatches[self::MAJOR + 1][0] ?? 0),
            self::MINOR => (int)($aMatches[self::MINOR + 1][0] ?? 0),
            self::PATCH => (int)($aMatches[self::PATCH + 1][0] ?? 0)
        ];
        foreach ($aVersion as $iPart => $iValue) {
            if ($iValue < 0 || $iValue > self::VERSION_LIMITS[$iPart]) {
                throw new \RangeException('Version fragment #' . $iPart . ' value ' . $iValue);
            }
        }
        return $aVersion;
    }

    /**
     * @return int
     */
    public function getMinVersion(): int {
        return $this->encodeVersion($this->aMinVersion);
    }

    /**
     * @return int
     */
    public function getMaxVersion(): int {
        return $this->encodeVersion($this->aMaxVersion);
    }

    /**
     * @param  int[3] $aVersion
     * @return int
     */
    private function encodeVersion(array $aVersion): int {
        return
            $aVersion[self::MAJOR] << (self::MINOR_BITS + self::PATCH_BITS) |
            $aVersion[self::MINOR] << (self::PATCH_BITS) |
            $aVersion[self::PATCH];
    }
}
