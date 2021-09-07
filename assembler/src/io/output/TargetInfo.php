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

use function \array_column, \implode;
use function \count, \strlen, \pack, \chr;

/**
 * TargetInfo
 *
 * Contains information about the compiled target. This includes a set of flags that say what the target is,
 * i.e. an executable or a library, etc. Next follows a dependency list of version ID values and names.
 *
 * The first entry in the dependency list is the target itself. For executable targets, the next entry in the
 * list is always the expected host.
 *
 * The raw chunk body data format is:
 *     Flags             uint32
 *     Number of entries uint32
 *     Version table     uint32[Number of Labels]
 *     Dependency names  char[]
 *
 * Dependency names are 4-256 characters (including a null terminator) and the label names section is
 * stored as a string blob, including null terminators to allow the data to be safely loaded in a C
 * runtime. Knowing how many labels there are, the runtime just needs to scan through the blob and
 * each time it encounters a null terminator, record the start of the string preceding it.
 */
class TargetInfo implements IBinaryChunk {
    const
        TYPE  = 'TrgtInfo'
    ;

    private string $sBinary;

    /**
     * Constructor. Builds the binary represntation for the export list.
     *
     * @param State\Target $oTarget
     */
    public function __construct(State\Target $oTarget) {
        $aDependencies = $oTarget->getDependencySet()->getList();
        $this->sBinary = pack(
            'V*',
            $oTarget->getFlags(),
            count($aDependencies),
            ...array_column($aDependencies, 'iVersion')
        ) . implode(chr(0), array_column($aDependencies, 'sName')) . chr(0);
    }

    /**
     * Returns the chunk ID (8 bytes)
     *
     * @return string
     */
    public function getChunkType(): string {
        return self::TYPE;
    }

    /**
     * Returns the chunk length in bytes (not including the type header)
     *
     * @return int
     */
    public function getChunkLength(): int {
        return strlen($this->sBinary);
    }

    /**
     * Returns the chunk data as a binary string.
     *
     * @return string
     */
    public function getChunkData(): string {
        return $this->sBinary;
    }
}
