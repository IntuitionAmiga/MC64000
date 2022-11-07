#ifndef MC64K_SYNTH_IFACE_HPP
    #define MC64K_SYNTH_IFACE_HPP

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

#include <misc/scalar.hpp>

namespace MC64K::Synth::Audio {

/**
 * Interface for control curve. Maps a floating point value (typically in the range 0.0 - 127.0) to
 * some other value. Used for things like velocity curves, etc.
 */
class IControlCurve {
    public:
        float32 const DEF_RANGE_MIN = 0.0;
        float32 const DEF_RANGE_MAX = 127.0;
        float32 const DEF_RANGE     = DEF_RANGE_MAX - DEF_RANGE_MIN;
        float32 const DEF_SCALE     = 1.0 / DEF_RANGE;

        virtual ~IControlCurve()           = default;
        virtual float32 map(float32 const) = 0;
};

}
#endif
