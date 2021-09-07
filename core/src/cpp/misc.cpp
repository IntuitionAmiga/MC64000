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

#include <cstdio>
#include <cstdlib>
#include "misc/version.hpp"

namespace MC64K {
namespace Misc {

/**
 * Version long constructor
 */
Version::Version(unsigned iMajor, unsigned iMinor, unsigned iPatch) {
    if (iMajor > MAX_MAJOR || iMinor > MAX_MINOR || iPatch > MAX_PATCH) {
        throw InvalidVersion();
    }
    uPackedVersion = iMajor << (PATCH_BITS + MINOR_BITS) | iMinor << PATCH_BITS | iPatch;
}

/**
 * Version compatibility check. Returns true if the current instance has the same major version number
 * and at least the same minor/patch combination.
 */
bool Version::isCompatible(const Version& oVersion) const {
    return (
        // Major part must be equal
        (uPackedVersion & MASK_MAJOR) == (oVersion.uPackedVersion & MASK_MAJOR)
    ) && (
        // Remainder must be greater or equal
        (uPackedVersion & ~MASK_MAJOR) >= (oVersion.uPackedVersion & ~MASK_MAJOR)
    );
}

}} // namespace
