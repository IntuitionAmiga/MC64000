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
 * ExportList
 *
 * Chunk that contains the exported global labels.
 * As the maximum code size is 2^31-1 bytes, the offsets are recorded as unsigned 32-bit integers.
 *
 *
 * The raw chunk body data format is:
 *     Number of labels uint32
 *     Offset table     uint32[Number of Labels]
 *     Label names      char[]
 *
 * Labels are between 2-256 characters (including a null terminator) and the label names section is
 * stored as a string blob, including null terminators to allow the data to be safely loaded in a C
 * runtime. Knowing how many labels there are, the runtime just needs to scan through the blob and
 * each time it encounters a null terminator, record the start of the string preceding it.
 */
class ExportList implements IBinaryChunk {

    const
        TYPE  = 'Exported'
    ;

    private string $sBinary;

    /**
     * Constructor. Builds the binary represntation for the export list.
     *
     * @param State\LabelLocation $oLabelLocation
     */
    public function __construct(State\LabelLocation $oLabelLocation) {
        $aExports = $oLabelLocation->resolveExports();
        $aLabels  = array_column($aExports, 'sLabel');
        $aIEQualifications = $oLabelLocation->getImportExportQualifications();
        foreach ($aLabels as $i => $sLabel) {
            if (!isset($aIEQualifications[$sLabel])) {
                throw new \Exception("Could not find Import/Export access for label " . $sLabel);
            }
            $aLabels[$i] = $sLabel . chr($aIEQualifications[$sLabel]);
        }
        $this->sBinary =
            // Label Count
            pack('V', count($aExports)) .

            // Offset table
            pack('V*', ...array_column($aExports, 'iLabelPosition')) .

            // Strings blob. Null terminated strings that are to be rescanned on loading.
            implode("", $aLabels)
        ;
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
