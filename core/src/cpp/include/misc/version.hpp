#ifndef __MC64K_VERSION_HPP__
    #define __MC64K_VERSION_HPP__

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

#include "scalar.hpp"

namespace MC64K {
namespace Misc {

class InvalidVersion {};

/**
 * Simple semantic version word wrapper. Word format is major[12]:minor[10]:patch[10]
 */
class Version {
    private:
        uint32 uPackedVersion;

        enum {
            MAJOR_BITS = 12,
            MINOR_BITS = 10,
            PATCH_BITS = 10,
            MAX_MAJOR  = (1 << MAJOR_BITS) - 1,
            MAX_MINOR  = (1 << MINOR_BITS) - 1,
            MAX_PATCH  = (1 << PATCH_BITS) - 1,
            MASK_MAJOR = MAX_MAJOR << (MINOR_BITS + PATCH_BITS)
        };

    public:
        Version(uint32 uVersion = 0) : uPackedVersion(uVersion) {}
        Version(unsigned iMajor, unsigned iMinor, unsigned iPatch);

        /**
         * Check if the current version instance is compatible with the supplied one.
         *
         * This is true when the major version are the same and the remaining minor/patch are greater than or
         * equal to the remaining components.
         */
        bool isCompatible(const Version& oVersion) const;
};

}} // namespace

#endif
