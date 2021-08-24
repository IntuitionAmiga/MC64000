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

namespace ABadCafe\MC64K\IO\Output;
use ABadCafe\MC64K\Defs\IIntLimits;
use ABadCafe\MC64K\Utils\Log;

use function \dirname, \is_writeable, \fopen, \fseek, \fwrite, \fclose;
use function \strlen, \pack;

/**
 * Binary
 *
 * Output file format for assembled bytecode.
 */
class Binary {

    private string $sFilename;
    private        $rOutput;
    private int    $iChunkNumber = 0, $iLoadSize = 0;

    const
        HEADER_MAGIC = "MC64000X\0\0\0\0\0\0\0\0",
        HEADER_SIZE  = 16,
        HEADER_LOAD  = 8
    ;

    const PADS = [
        "",
        "\0\0\0\0\0\0\0",
        "\0\0\0\0\0\0",
        "\0\0\0\0\0",
        "\0\0\0\0",
        "\0\0\0",
        "\0\0",
        "\0",
    ];

    /**
     * @param  string $sFilename
     * @throws \Exception
     */
    public function __construct(string $sFilename) {
        $sDirname = dirname($sFilename);
        if (!is_writeable($sDirname)) {
            throw new \Exception($sDirname . ' is not a writeable location');
        }
        if (!$this->rOutput = fopen($sFilename, 'wb')) {
            throw new \Exception($sFilename . ' could not be created');
        }
        $this->writeHeader();
        $this->sFilename = $sFilename;
    }

    /**
     * Writes a chunk to the file.
     *
     * @param  IBinaryChunk $oChunk
     * @return self (fluent)
     * @throws \Exception
     */
    public function writeChunk(IBinaryChunk $oChunk) : self {
        $iSize =
            $this->writeChunkHeader($oChunk) +
            $this->writeChunkBody($oChunk) +
            $this->writeChunkPadding($oChunk);
        $this->iLoadSize += $iSize;
        return $this;
    }

    /**
     * Complete the file write. This will update the header with the total load size for the runtime.
     */
    public function complete() : void {
        fseek($this->rOutput, self::HEADER_LOAD, SEEK_SET);
        fwrite($this->rOutput, pack(IIntLimits::QUAD_BIN_FORMAT, $this->iLoadSize));
    }

    /**
     * Make sure the file is closed on destruction
     */
    public function __destruct() {
        if ($this->rOutput) {
            fclose($this->rOutput);
            $this->rOutput = null;
        }
    }

    /**
     * Get the name of the file
     *
     * @return string
     */
    public function getFilename() : string {
        return $this->sFilename;
    }

    /**
     * Writes the header structure for a chunk and returns the count of bytes written.
     * An exception is thrown if there are problems with the size or write.
     *
     * @param  IBinaryChunk $oChunk
     * @return int
     * @throws \Exception
     */
    private function writeChunkHeader(IBinaryChunk $oChunk) : int {
        $sChunkHeader = $oChunk->getChunkType();
        if (IBinaryChunk::TYPE_SIZE !== strlen($sChunkHeader)) {
            throw new \Exception("Chunk type has illegal size");
        }
        $sChunkHeader .= pack(IIntLimits::QUAD_BIN_FORMAT, $oChunk->getChunkLength());
        $iWriteSize    = strlen($sChunkHeader);
        if (fwrite($this->rOutput, $sChunkHeader) !== $iWriteSize) {
            throw new \Exception("Unexpected write length for chunk header writing to " . $this->sFilename);
        }
        Log::printf("Wrote chunk %s header %d bytes", $oChunk->getChunkType(), $iWriteSize);
        return $iWriteSize;
    }

    /**
     * Writes the chunk data and returns the count of bytes written.
     * An exception is thrown if there are problems with the size or write.
     *
     * @param  IBinaryChunk $oChunk
     * @return int
     * @throws \Exception
     */
    private function writeChunkBody(IBinaryChunk $oChunk) : int {
        $iWriteSize = $oChunk->getChunkLength();
        if ($iWriteSize !== fwrite($this->rOutput, $oChunk->getChunkData())) {
            throw new \Exception("Unexpected write length for chunk data writing to " . $this->sFilename);
        }
        Log::printf("Wrote chunk %s body %d bytes", $oChunk->getChunkType(), $iWriteSize);
        return $iWriteSize;
    }

    /**
     * Writes padding bytes after a chunk to ensure alignment (if needed).
     * Returns number of bytes written or throws an exception if there is a problem with the write.
     *
     * @param  IBinaryChunk $oChunk
     * @return int
     * @throws \Exception
     */
    private function writeChunkPadding(IBinaryChunk $oChunk) : int {
        $iAlignment = $oChunk->getChunkLength() & (IBinaryChunk::ALIGNMENT - 1);
        if ($iAlignment) {
            $sAlignPads = self::PADS[$iAlignment];
            $iAlignment = IBinaryChunk::ALIGNMENT - $iAlignment;
            if ($iAlignment !== fwrite($this->rOutput, $sAlignPads)) {
                throw new \Exception("Unexpected write length chunk padding writing to " . $this->sFilename);
            }
            Log::printf("Wrote chunk %s padding %d bytes", $oChunk->getChunkType(), $iAlignment);
        }
        return $iAlignment;
    }

    /**
     * @throws \Exception
     */
    private function writeHeader() {
        if (self::HEADER_SIZE !== fwrite($this->rOutput, self::HEADER_MAGIC)) {
            throw new \Exception("Unable to write file header to " . $this->sFilename);
        }
    }
}
