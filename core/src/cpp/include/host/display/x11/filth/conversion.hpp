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
#include <host/standard_test_host_display.hpp>

namespace MC64K::StandardTestHost::Display::x11 {

/**
 * Palette Lookup for 32-bit colour model. Each 32-bit word is assumed to have byte addressable
 * pixels here.
 *
 * PaletteGeneric32<Format::X> where X is a valid 32-bit, 8-bit per gun formats
 *
 */
template<typename Format>
class PaletteTo32Bit {

    static_assert(
        std::is_same<uint32, typename Format::Pixel>::value,
        "Invalid Format::Pixel type for this template"
    );

    public:
        typedef typename Format::Pixel Pixel;

        Pixel convert(Pixel const* puPalette, uint8 uPixel) {
            return puPalette[uPixel];
        }

        /**
         * Support for FILTH palette operations
         */
        void setPaletteAlpha(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::ALPHA] = *puCode++;
        }

        void setPaletteRed(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::RED] = *puCode++;
        }

        void setPaletteGreen(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::GREEN] = *puCode++;
        }

        void setPaletteBlue(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::BLUE] = *puCode++;
        }

        void addPaletteAlpha(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::ALPHA] += *puCode++;
        }

        void addPaletteRed(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::RED] += *puCode++;
        }

        void addPaletteGreen(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::GREEN] += *puCode++;
        }

        void addPaletteBlue(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::BLUE] += *puCode++;
        }

        void subPaletteRed(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::RED] -= *puCode++;
        }

        void subPaletteGreen(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::GREEN] -= *puCode++;
        }

        void subPaletteBlue(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::BLUE] -= *puCode++;
        }

        void addPaletteRGB(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::BLUE]  += puCode[Format::BLUE];
            ((uint8*)(&puPalette[uIndex]))[Format::GREEN] += puCode[Format::GREEN];
            ((uint8*)(&puPalette[uIndex]))[Format::RED]   += puCode[Format::RED];
            puCode += sizeof(Pixel);
        }

        void subPaletteRGB(Pixel* puPalette, uint8* &puCode) {
            uint8 uIndex = *puCode++;
            ((uint8*)(&puPalette[uIndex]))[Format::BLUE]  -= puCode[Format::BLUE];
            ((uint8*)(&puPalette[uIndex]))[Format::GREEN] -= puCode[Format::GREEN];
            ((uint8*)(&puPalette[uIndex]))[Format::RED]   -= puCode[Format::RED];
            puCode += sizeof(Pixel);
        }
};



/**
 * Palette Lookup. Converts an assumed 8-bit pixel value into a 15-bit target RGB value
 * using a Hold And Modify approach in which we can either:
 *
 * Set the 15 bit from one of 32 palette entries
 * Hold a previous value and set one RGB component to the immediate 5-bit value
 * Hold a previous value and set a pair of RGB components simultaneously to the immediate 5-bit value
 * Set all RGB components to the immediate 5-bit value (greyscale)
 */
template<typename Format>
class PaletteHAM555To15Bit {

    static_assert(
        std::is_same<uint16, typename Format::Pixel>::value,
        "Invalid Format::Pixel type for this template"
    );

    public:

        typedef typename Format::Pixel Pixel;

    private:
        Pixel uPrevRGB;

    public:
        PaletteHAM555To15Bit() : uPrevRGB(0) {}

        Pixel convert(Pixel const* puPalette, uint8 uPixel) {
            typename Format::Pixel uVal = uPixel & 0b00011111;
            switch (uPixel >> 5) {
                case 0b000: // Palette entry
                    return uPrevRGB = puPalette[uPixel];
                    break;

                case 0b001: // Hold R, G, set B
                    return uPrevRGB = (Pixel)(
                        (uPrevRGB & ~Format::MASK_BLUE) |
                        (uVal << Format::BLUE)
                    );
                    break;

                case 0b010: // Hold R, B, set G
                    return uPrevRGB = (typename Format::Pixel)(
                        (uPrevRGB & ~Format::MASK_GREEN) |
                        (uVal << Format::GREEN)
                    );
                    break;

                case 0b011: // Hold R, set G, B
                    return uPrevRGB = (Pixel)(
                        (uPrevRGB & Format::MASK_RED) |
                        (uVal << Format::GREEN) |
                        (uVal << Format::BLUE)
                    );
                    break;

                case 0b100: // Hold G, B, set R
                    return uPrevRGB = (Pixel)(
                        (uPrevRGB & ~Format::MASK_RED) |
                        (uVal << Format::RED)
                    );
                    break;

                case 0b101: // Hold G, set R, B
                    return uPrevRGB = (Pixel)(
                        (uPrevRGB & Format::MASK_GREEN) |
                        (uVal << Format::RED) |
                        (uVal << Format::BLUE)
                    );
                    break;

                case 0b110: // Hold B, set G, B
                    return uPrevRGB = (Pixel)(
                        (uPrevRGB & Format::MASK_BLUE) |
                        (uVal << Format::RED) |
                        (uVal << Format::GREEN)
                    );
                    break;

                case 0b111: // Set R, G and B
                    return uPrevRGB = (Pixel)(
                        (uVal << Format::RED) |
                        (uVal << Format::GREEN) |
                        (uVal << Format::BLUE)
                    );
                    break;
            }
            return uPrevRGB;
        }

        /**
         * Support for FILTH palette operations
         */
        void setPaletteAlpha(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::ALPHA] = *puCode++;
        }

        void setPaletteRed(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::RED] = *puCode++;
        }

        void setPaletteGreen(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::GREEN] = *puCode++;
        }

        void setPaletteBlue(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::BLUE] = *puCode++;
        }

        void addPaletteAlpha(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::ALPHA] += *puCode++;
        }

        void addPaletteRed(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::RED] += *puCode++;
        }

        void addPaletteGreen(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::GREEN] += *puCode++;
        }

        void addPaletteBlue(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::BLUE] += *puCode++;
        }

        void subPaletteRed(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::RED] -= *puCode++;
        }

        void subPaletteGreen(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::GREEN] -= *puCode++;
        }

        void subPaletteBlue(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::BLUE] -= *puCode++;
        }

        void addPaletteRGB(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::BLUE]  += puCode[Format::BLUE];
            //((uint8*)(&puPalette[uIndex]))[Format::GREEN] += puCode[Format::GREEN];
            //((uint8*)(&puPalette[uIndex]))[Format::RED]   += puCode[Format::RED];
            //puCode += sizeof(Pixel);
        }

        void subPaletteRGB(Pixel* puPalette, uint8* &puCode) {
            //uint8 uIndex = *puCode++;
            //((uint8*)(&puPalette[uIndex]))[Format::BLUE]  -= puCode[Format::BLUE];
            //((uint8*)(&puPalette[uIndex]))[Format::GREEN] -= puCode[Format::GREEN];
            //((uint8*)(&puPalette[uIndex]))[Format::RED]   -= puCode[Format::RED];
            //puCode += sizeof(Pixel);
        }
    };

}

#endif
