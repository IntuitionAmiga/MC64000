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

namespace ABadCafe\MC64K\IO;

use function \file_exists, \is_readable, \fopen, \fclose, \fgets, \strlen, \substr;

/**
 * SourceFile
 *
 * Primary implementation of the ISourceFile interface for reading source files from disk.
 */
final class SourceFile implements ISourceFile {

    const SHORT_LENGTH = 48;

    /** @var resource $rSource */
    private        $rSource;
    private string $sFilename;
    private string $sCurrentLine = '', $sLastLine = '';
    private int    $iLineNumber = 0;

    /**
     * @param  string $sFilename
     * @throws \Exception
     */
    public function __construct(string $sFilename) {
        $rSource = null;
        if (
            !file_exists($sFilename) ||
            !is_readable($sFilename) ||
            (!($rSource = fopen($sFilename, 'r')))
        ) {
            throw new \Exception($sFilename . ' could not be opened');
        }
        $this->rSource   = $rSource;
        $this->sFilename = $sFilename;
    }

    /**
     * Make sure the file is closed on destruction
     */
    public function __destruct() {
        fclose($this->rSource);
    }

    /**
     * @inheritDoc
     */
    public function readLine(): ?string {
        $sLine = fgets($this->rSource);
        if (false !== $sLine) {
            ++$this->iLineNumber;
            $this->sLastLine = $this->sCurrentLine;
            $this->sCurrentLine = $sLine;
            return $sLine;
        }
        return null;
    }

    /**
     * @inheritDoc
     */
    public function getFilename(): string {
        return $this->sFilename;
    }

    /**
     * @inheritDoc
     */
    public function getShortFilename(): string {
        return self::shortenFilename($this->sFilename);
    }

    /**
     * @inheritDoc
     */
    public function getLineNumber(): int {
        return $this->iLineNumber;
    }

    /**
     * @inheritDoc
     */
    public function getLine(): string {
        return $this->sCurrentLine;
    }

    /**
     * @inheritDoc
     */
    public function getPrecedingLine(): string {
        return $this->sLastLine;
    }

    /**
     * @param  string $sFilename
     * @return string
     */
    public static function shortenFilename(string $sFilename): string {
        return strlen($sFilename) > self::SHORT_LENGTH ?
            '...' . substr($sFilename, 3 - self::SHORT_LENGTH) :
            $sFilename
        ;
    }
}
