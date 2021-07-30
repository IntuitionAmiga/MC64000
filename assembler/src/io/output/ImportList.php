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
use ABadCafe\MC64K\State;

/**
 * ImportList
 *
 * Chunk that contains the imported global labels. Imported labels are enumerated incrementally
 * as encountered in the source code.
 *
 * The raw chunk body data format is:
 *     Number of labels uint32
 *     Label names      char[]
 */
class ImportList implements IBinaryChunk {

    const
        TYPE  = 'Imported'
    ;

    private string $sBinary = '';

    /**
     * Constructor. Builds the binary represntation for the export list.
     *
     * @param State\LabelLocation $oLabelLocation
     */
    public function __construct(State\LabelLocation $oLabelLocation) {
        $aImports      = $oLabelLocation->getEnumeratedImports();
        $this->sBinary =
            // Label count
            pack('V', count($aImports)) .

            // Strings blob
            implode("\0", $aImports) . "\0";
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
     *
     * @return int
     */
    public function getChunkLength() : int {
        return strlen($this->sBinary);
    }

    /**
     * Returns the chunk data as a binary string.
     *
     * @return string
     */
    public function getChunkData() : string {
        return $this->sBinary;
    }
}
