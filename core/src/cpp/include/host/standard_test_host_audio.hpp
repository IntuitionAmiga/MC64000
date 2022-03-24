#ifndef MC64K_STANDARD_TEST_HOST_AUDIO_HPP
    #define MC64K_STANDARD_TEST_HOST_AUDIO_HPP

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

#include "standard_test_host.hpp"

namespace MC64K::StandardTestHost::Audio {

/**
 * Call
 *
 * Enumeration of calls in the Mem namespace
 */
enum Call {
    INIT = 0,
    DONE,
    OPEN,
    CLOSE,
    WRITE
};

/**
 * Error return values
 */
enum Result {
    ERR_NO_DEVICE      = 1000,
    ERR_INVALID_HZ     = 1001,
    ERR_INVALID_BUF    = 1002,
    ERR_INVALID_FMT    = 1003,
    ERR_INVALID_CHN    = 1004
};

/**
 * Sub namespace for output related constants.
 */
namespace Output {

    // Keeping it real simple for now
    enum ChannelMode {
        CH_MONO   = 1,
        CH_STEREO = 2
    };

    // Sample formats
    enum Format {
        INT_8  = 0,
        INT_16 = 1
    };

    // Frequency limits. Inputs will be clamped against this.
    enum RateLimits {
        MIN_HZ = 4000,
        MAX_HZ = 48000
    };

    // Buffer limits. Inputs will be clamped against this.
    enum BufferLimits {
        MIN_MS = 5,
        MAX_MS = 5000
    };
};
Interpreter::Status hostVector(uint8 uFunctionID);

} // namespace

#endif
