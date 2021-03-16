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
 * Process for performing various jobs after assembling the initial output buffer
 *
 */
class SecondPass {

    /**
     * Resolve forwards branch target references that were not resolved during the first pass. This will throw if
     * any of the unresolved references refer to a target that has not been declared at any time.
     *
     * @param  State\LabelLocation $oLabelLocation
     * @param  State\Output $oOutput
     * @throws \Exception
     */
    public function resolveForwardsBranchReferences(State\LabelLocation $oLabelLocation, State\Output $oOutput) : void {
        $aBranchTargets = $oLabelLocation->resolveBranchTargetList();
        Log::printf('There are %d 32-bit forwards branch references to resolve...', count($aBranchTargets));
        foreach ($aBranchTargets as $iReferenceOffset => $oResolved) {
            $iTargetOffset = $oResolved->iLabelPosition;
            $iDisplacement = $iTargetOffset - $iReferenceOffset - Defs\IBranchLimits::DISPLACEMENT_SIZE;
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
                pack('V', $iDisplacement),
                $iReferenceOffset
            );
        }
    }
}
