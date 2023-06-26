#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_FORMAT_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_FORMAT_HPP

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

#include <host/standard_test_host_display.hpp>

namespace MC64K::StandardTestHost::Display::Format {

/**
 * Basic pixel descriptions. The minimum requirement is that each of these classes defines the expected
 * word type for the pixel. Beyond this, they can enumerate channel arrangements and so forth for any
 * RGBx formats.
 */
class LUT8 {
    public:
        typedef uint8 Pixel; // 1 byte per pixel
};

class ARGB32 {
    public:
        typedef uint32 Pixel; // 4 bytes per pixel, as 1-byte channels, enumerated as:
        enum {
            ALPHA = 3,
            RED   = 2,
            GREEN = 1,
            BLUE  = 0,
        };
};

class RGB555 {
    public:
        typedef uint16 Pixel; // 2 bytes per pixel, as right-justified 5 bit fields, offets enumerateed as:
        enum {
            ALPHA = 15,
            RED   = 10,
            GREEN = 5,
            BLUE  = 0,
        };
        enum { // Masks for isolating channels
            MASK_BITS  = 31,
            MASK_ALPHA = 1 << ALPHA,
            MASK_RED   = MASK_BITS << RED,
            MASK_GREEN = MASK_BITS << GREEN,
            MASK_BLUE  = MASK_BITS << BLUE,
        };
};

}
#endif
