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

namespace ABadCafe\MC64K\Process;
use ABadCafe\MC64K\State;
use ABadCafe\MC64K\Defs;
use ABadCafe\MC64K\Utils\Log;

/**
 * SecondPass
 *
 * Process for performing various jobs after assembling the initial output buffer.
 */
class SecondPass {

    /**
     * Resolves forwards branch target references that were not resolved during the first pass. This will throw if
     * any of the unresolved references refer to a target that has not been declared at any time.
     *
     * @param  State\LabelLocation $oLabelLocation
     * @param  State\Output $oOutput
     * @throws \Exception
     */
    public function resolveForwardsBranchReferences(State\LabelLocation $oLabelLocation, State\Output $oOutput) : self {
        $aBranchTargets = $oLabelLocation->resolveBranchTargetList();

        $bLogResolution = State\Coordinator::get()
            ->getOptions()
            ->isEnabled(Defs\Project\IOptions::LOG_LABEL_RESOLVE);

        $bLogResolution &&
        Log::printf('There are %d 32-bit forwards branch references to resolve...', count($aBranchTargets));
        foreach ($aBranchTargets as $iReferenceOffset => $oResolved) {
            $iTargetOffset = $oResolved->iLabelPosition;
            $iDisplacement = $iTargetOffset - $iReferenceOffset - Defs\IBranchLimits::DISPLACEMENT_SIZE;
            $bLogResolution &&
            Log::printf(
                "\tReference to %s (%s@%d) at offset %d points to offset %d, displacement size is %d",
                $oResolved->sLabel,
                $oResolved->sFilename,
                $oResolved->iLineNumber,
                $iReferenceOffset,
                $iTargetOffset,
                $iDisplacement
            );
            $oOutput->patch(
                pack(Defs\IIntLimits::LONG_BIN_FORMAT, $iDisplacement),
                $iReferenceOffset
            );
        }
        return $this;
    }

    public function enumerateImportReferences(State\LabelLocation $oLabelLocation, State\Output $oOutput) : self {
        $aImportReferences = $oLabelLocation->getImports();
        $iImportID = 0;

        $bLogResolution = State\Coordinator::get()
            ->getOptions()
            ->isEnabled(Defs\Project\IOptions::LOG_LABEL_RESOLVE);

        $bLogResolution &&
        Log::printf("There are %d imported symbols to enumerate...", count($aImportReferences));

        foreach ($aImportReferences as $sLabel => $aReferences) {
            if (empty($aReferences)) {
                $bLogResolution &&
                Log::printf("\tImported label '%s' is unused, enumeration skipped", $sLabel);
                continue;
            }

            $iEnumeration = $iImportID++;

            $bLogResolution &&
            Log::printf("\tEnumerated label '%s' as #%d", $sLabel, $iEnumeration);

            foreach ($aReferences as $oReference) {
                $bLogResolution &&
                Log::printf(
                    "\t\tReference (%s@%d) at offset %d",
                    $oReference->sFilename,
                    $oReference->iLineNumber,
                    $oReference->iLocation
                );
                $oOutput->patch(
                    pack(Defs\IIntLimits::LONG_BIN_FORMAT, $iEnumeration),
                    $oReference->iLocation
                );
            }
        }
        return $this;
    }

}
