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

namespace MC64K::StandardTestHost::Display::x11 {

/**
 * Transfer routine for:
 *
 * Pixel Format  == LUT8
 * Buffer Size   == View Size
 * X Offset      == 0
 * Y Offset      == 0
 * Filth Script  == nullptr
 */
void* updateLUT8Simple(Context& roContext) {
    if (uint32 const* puPalette = roContext.puPalette) {
        uint32*       pDst = (uint32*)roContext.puImageBuffer;
        uint8 const*  pSrc = roContext.oDisplayBuffer.puByte;
        uint32        uCnt = roContext.uNumBufferPixels;
        while (uCnt--) {
            *pDst++ = puPalette[*pSrc++];
        }
    }
    return roContext.puImageBuffer;
}

/**
 * Transfer routine for:
 *
 * Pixel Format  == ARGB_32
 * Buffer Size   == View Size
 * X Offset      == 0
 * Y Offset      == 0
 * Filth Script  == nullptr
 */
void* updateARGB32Simple(Context& roContext) {
    return roContext.oDisplayBuffer.puByte; // Just return the display buffer as-is
}

}

#endif
