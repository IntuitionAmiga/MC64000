#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_X11_FILTH_SIMPLE_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_X11_FILTH_SIMPLE_HPP

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
#include <host/display/x11/raii.hpp>

#include "conversion.hpp"

namespace MC64K::StandardTestHost::Display::x11 {

/**
 * Transfer routine for:
 *
 * Pixel Format  == LUT8/HAM
 * Buffer Size   == View Size
 * X Offset      == 0
 * Y Offset      == 0
 * Filth Script  == nullptr
 */
template<typename T, typename C>
void* updateLUT8(Context& roContext) {
    static_assert(std::is_integral<T>::value, "Invalid template type for pixel");

    // Just expand out the palette to the target
    if (T const* puPalette = roContext.oPaletteData.as<T const>()) {
        C::init();
        T*            pDst = (T*)roContext.puImageBuffer;
        uint8 const*  pSrc = roContext.oDisplayBuffer.puByte;
        uint32        uCnt = roContext.uNumBufferPixels;
        while (uCnt--) {
            *pDst++ = C::convert(puPalette, *pSrc++);
        }
    }
    return roContext.puImageBuffer;
}

/**
 * Transfer routine for:
 *
 * Pixel Format  == Any non-palette
 * Buffer Size   == View Size
 * X Offset      == 0
 * Y Offset      == 0
 * Filth Script  == nullptr
 */
template<typename T>
void* update(Context& roContext) {
     // Just return the display buffer as-is. This doesn't even really need templating.
    return roContext.oDisplayBuffer.puByte;
}

}

#endif
