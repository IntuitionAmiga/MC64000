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

#include <misc/version.hpp>

namespace MC64K::Misc {

/**
 * @inheritDoc
 */
Version::Version(uint32 const uMajor, uint32 const uMinor, uint32 const uPatch) {
    if (uMajor > MAX_MAJOR || uMinor > MAX_MINOR || uPatch > MAX_PATCH) {
        throw InvalidVersion();
    }
    uPackedVersion = uMajor << (PATCH_BITS + MINOR_BITS) | uMinor << PATCH_BITS | uPatch;
}

/**
 * @inheritDoc
 */
bool Version::isCompatible(Version const& roVersion) const {
    return (
        // Major part must be equal
        (uPackedVersion & MASK_MAJOR) == (roVersion.uPackedVersion & MASK_MAJOR)
    ) && (
        // Remainder must be greater or equal
        (uPackedVersion & ~MASK_MAJOR) >= (roVersion.uPackedVersion & ~MASK_MAJOR)
    );
}

} // namespace
