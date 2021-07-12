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

/**
 * ChunkList
 *
 * List of chunks to be included
 */
class ChunkList implements IBinaryChunk {

    const
        TYPE  = 'CHNKLIST',
        PAD   = self::ALIGNMENT - 1,
        ENTRY = 16
    ;

    private array $aChunks = [];
    private int   $iLocation;

    /**
     * Constructor. Assumes by default the chunk list will immediately follow the file header. If this is not the case
     * a different offset can be specified. The offset must be aligned.
     *
     * @param int $iOffset
     */
    public function __construct(int $iOffset = Binary::HEADER_SIZE) {
        if ($iOffset & self::PAD) {
            throw new \Exception("Misaligned offset location " . $iOffset);
        }
        $this->iLocation = $iOffset;
    }

    /**
     * Add a chunk to the list. The list cannot contain itself due to the fact the offsets are determined on the fly.
     *
     * @param  IBinaryChunk $oChunk
     * @return self - fluent
     * @throws \Exception
     */
    public function registerChunk(IBinaryChunk $oChunk) : self {
        if ($oChunk === $this) {
            throw new \Exception('ChunkList should not contain itself');
        }
        $sChunkType = $oChunk->getChunkType();
        if (self::TYPE_SIZE !== strlen($sChunkType)) {
            throw new \Exception('Invalid Chunk Type ID');
        }
        $iExpectWriteSize = self::TYPE_SIZE + ($oChunk->getChunkLength() + self::PAD) & ~self::PAD;
        $this->aChunks[] = [
            $oChunk->getChunkType(),
            $iExpectWriteSize
        ];
        return $this;
    }

    /**
     * Returns the chunk ID (8 bytes)
     *
     * @return string
     */
    public function getChunkType() : string {
        return self::TYPE;
    }

    /**
     * Returns the chunk length in bytes (not including the type header)
     */
    public function getChunkLength() : int {
        return count($this->aChunks) * self::ENTRY;
    }

    /**
     * Returns the chunk data
     *
     * @return string
     */
    public function getChunkData() : string {
        $sBody = '';
        $iChunkOffset = $this->iLocation + self::TYPE_SIZE + $this->getChunkLength();
        foreach ($this->aChunks as $aData) {
            $sBody .= $aData[0];
            $sBody .= pack(IIntLimits::QUAD_BIN_FORMAT, $iChunkOffset);
            $iChunkOffset += $aData[1];
        }
        return $sBody;
    }
}
