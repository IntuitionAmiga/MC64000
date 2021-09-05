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
 * DependencySet
 */
class DependencySet {

    /**
     * Valid dependency names must be 4-255 non-control ASCII characters (32-126)
     */
    const
        NAME_MATCH    = '/^[\x20-\x7E]{4,255}$/',
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

    private array $aDependencies = [];

    /**
     * Adds a dependency. If the dependency is already known, performs some version checking here
     * to make sure that the semantic versioning requirements are maintained.
     *
     * @param  string $sName
     * @param  string $sVersion
     * @return self
     * @throws \UnexpectedValueException
     */
    public function add(string $sName, string $sVersion): self {
        if (!preg_match(self::NAME_MATCH, $sName)) {
            throw new \UnexpectedValueException("Invalid dependency name '" . $sName . "'");
        }

        $aVersion = $this->parseVersionNumber($sVersion);

        if (isset($this->aDependencies[$sName])) {
            $aVersion = $this->checkVersion($aVersion, $this->aDependencies[$sName]);
        }
        $this->aDependencies[$sName] = $aVersion;
        return $this;
    }

    /**
     * Returns the linear dependency list as an array of tuples of the form
     * {
     *     string $sName;
     *     int    $iMinVersion;
     *     int    $iMaxVersion;
     * }
     *
     * @return object[]
     */
    public function getList(): array {
        $aDepedencyList = [];
        foreach ($this->aDependencies as $sName => $aVersion) {
            $aDependencyList[] = (object)[
                'sName'       => $sName,
                'iVersion' => $this->encodeVersion($aVersion),
            ];
        }
        return $aDependencyList;
    }

    /**
     * @param  int[3] $aVersion
     * @param  int[3] $aCurrentVersion
     * @return int[3]
     * @throws \UnexpectedValueException
     */
    private function checkVersion(array $aVersion, array $aCurrentVersion): array {
        // Major version must match
        if ($aVersion[self::MAJOR] !== $aCurrentVersion[self::MAJOR]) {
            throw new \UnexpectedValueException(
                'Major version mismatch ' . $aVersion[self::MAJOR] .
                ' <> ' . $aCurrentVersion[self::MAJOR]
            );
        }
        // We select the current version when:
        // 1) It has a higher minor number
        // 2) It has an equal minor number but higher patch number.
        if (
            $aCurrentVersion[self::MINOR] > $aVersion[self::MINOR]
            || (
                $aCurrentVersion[self::MINOR] === $aVersion[self::MINOR]
                && $aCurrentVersion[self::PATCH] > $aVersion[self::PATCH]
            )
        ) {
            return $aCurrentVersion;
        }
        return $aVersion;
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
                throw new \RangeException('Invalid version fragment #' . $iPart . ' value ' . $iValue);
            }
        }
        return $aVersion;
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
