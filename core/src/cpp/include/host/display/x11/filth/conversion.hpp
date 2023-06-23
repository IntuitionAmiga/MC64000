#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_X11_FILTH_CONVERT_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_X11_FILTH_CONVERT_HPP

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

namespace MC64K::StandardTestHost::Display::x11 {

/**
 * Palette Lookup. Converts an assumed 8-bit pixel value into the target RGB value.
 */
template<typename T>
class PaletteLookup {
    static_assert(std::is_integral<T>::value, "Invalid template type for pixel");
    public:
        void init() {
        }

        T convert(T const* puPalette, uint8 uPixel) {
            return puPalette[uPixel];
        }
};

/**
 * Palette Lookup. Converts an assumed 8-bit pixel value into a 15-bit target RGB value
 * using a Hold And Modify approach in which e can either:
 *
 * Set the 15 bit from one of 32 palette entries
 * Hold a previous value and set one RGB component to the immediate 5-bit value
 * Hold a previous value and set a pair of RGB components simultaneously to the immediate 5-bit value
 * Set all RGB components to the immediate 5-bit value (greyscale)
 *
 * This is a template in name only, to facilitate usage in the template functions that
 * convert pixels.
 */
template<typename T>
class PaletteToHAM555 {
    static_assert(std::is_integral<T>::value, "Invalid template type for pixel");
    private:
        T uPrevRGB;

    public:
        PaletteToHAM555() : uPrevRGB(0) {}

        T convert(T const* pPalette, uint8 uPixel) {
            T uVal = uPixel & 0b00011111;
            switch (uPixel >> 5) {
                case 0b000: // Palette entry
                    return uPrevRGB = pPalette[uPixel];
                    break;

                case 0b001: // Hold R, G, set B
                    return uPrevRGB = (T)((uPrevRGB & 0b111111111100000) | uVal);
                    break;

                case 0b010: // Hold R, B, set G
                    return uPrevRGB = (T)((uPrevRGB & 0b111110000011111) | (uVal << 5));
                    break;

                case 0b011: // Hold R, set G, B
                    return uPrevRGB = (T)((uPrevRGB & 0b111110000000000) | (uVal << 5) | uVal);
                    break;

                case 0b100: // Hold G, B, set R
                    return uPrevRGB = (T)((uPrevRGB & 0b000001111111111) | (uVal << 10));
                    break;

                case 0b101: // Hold G, set R, B
                    return uPrevRGB = (T)((uPrevRGB & 0b000001111100000) | (uVal << 10) | uVal);
                    break;

                case 0b110: // Hold B, set G, B
                    return uPrevRGB = (T)((uPrevRGB & 0b000000000011111) | (uVal << 10) | (uVal << 5));
                    break;

                case 0b111: // Set R, G and B
                    return uPrevRGB = (T)((uPrevRGB << 10) | (uVal << 5) | uVal);
                    break;
            }
            return uPrevRGB;
    }
};

}

#endif
