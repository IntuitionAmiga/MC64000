#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_X11_FILTH_GENERIC_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_X11_FILTH_GENERIC_HPP

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

/**
 * These functions handle the conversion of the virtual framebuffer pixels to a given RGB format
 * for the case when the virtual framebuffer and viewport are the not the same size or there are
 * modificatons to the scroll position.
 *
 * One function is provided for converting an indexed colour format and one for transferring RGB
 * direct. The indexed version requires a conversion template parameter. The conversion it provides
 * is inlined in the template generation.
 */

namespace MC64K::StandardTestHost::Display::x11 {

/**
 * Updates the visible portion of an 8-bit surface, for some palette mapped display type
 *
 * Palette is assumed to be the same data format as the target display
 *
 * T is expected to be an integer type
 * C is expected to be one of the conversion classes
 */
template<typename T, class C>
void* updatePalettedViewModified(Context& roContext) {
    static_assert(std::is_integral<T>::value, "Invalid template type for pixel");

    if (T const* puPalette = roContext.oPaletteData.as<T const>()) {
        C oConversion;
        T*       pDst = (T*)roContext.puImageBuffer;
        unsigned y1   = 0;
        for (
            unsigned y2 = roContext.uViewYOffset;
            y2 < roContext.uBufferHeight && y1 < roContext.uViewHeight;
            ++y1, ++y2
        ) {
            // Start at uViewXOffset, uViewYOffset
            uint8 const* pSrc = roContext.oDisplayBuffer.puByte + (y2 * roContext.uBufferWidth) +
                roContext.uViewXOffset;

            // Upper left quadrant
            unsigned x1 = 0;
            for (
                unsigned x2 = roContext.uViewXOffset;
                x2 < roContext.uBufferWidth && x1 < roContext.uViewWidth;
                ++x1, ++x2
            ) {
                *pDst++ = oConversion.convert(puPalette, *pSrc++);
            }

            if (x1 < roContext.uViewWidth) {
                // Start at 0, uViewYOffset
                pSrc = roContext.oDisplayBuffer.puByte + (y2 * roContext.uBufferWidth);

                // Upper right quadrant
                for (
                    unsigned x2 = 0;
                    x1 < roContext.uViewWidth;
                    ++x1, ++x2
                ) {
                    *pDst++ = oConversion.convert(puPalette, *pSrc++);
                }
            }
        }

        if (y1 < roContext.uViewHeight) {
            for (
                unsigned y2 = 0;
                y1 < roContext.uViewHeight;
                ++y1, ++y2
            ) {
                // Start at uViewXOffset, 0
                uint8 const* pSrc = roContext.oDisplayBuffer.puByte + (y2 * roContext.uBufferWidth) + roContext.uViewXOffset;

                // Lower left quadrant
                unsigned x1 = 0;
                for (
                    unsigned x2 = roContext.uViewXOffset;
                    x2 < roContext.uBufferWidth && x1 < roContext.uViewWidth;
                    ++x1, ++x2
                ) {
                    *pDst++ = oConversion.convert(puPalette, *pSrc++);
                }

                if (x1 < roContext.uViewWidth) {

                    // Start at 0, 0
                    pSrc = roContext.oDisplayBuffer.puByte + (y2 * roContext.uBufferWidth);

                    // Lower right quadrant
                    for (
                        unsigned x2 = 0;
                        x1 < roContext.uViewWidth;
                        ++x1, ++x2
                    ) {
                        *pDst++ = oConversion.convert(puPalette, *pSrc++);
                    }
                }
            }
        }
    }
    return roContext.puImageBuffer;
}


/**
 * Updates the visible portion of an RGB surface, for some palette mapped display type
 *
 * T is expected to be an integer type
 */
template<typename T>
void* updateRGBViewModified(Context& roContext) {
    static_assert(std::is_integral<T>::value, "Invalid template type for pixel");

    T* pDst = (T*)roContext.puImageBuffer;
    T const* pBaseSrc = roContext.oDisplayBuffer.as<T const>();
    unsigned y1 = 0;
    for (
        unsigned y2 = roContext.uViewYOffset;
        y2 < roContext.uBufferHeight && y1 < roContext.uViewHeight;
        ++y1, ++y2
    ) {

        // Start at uViewXOffset, uViewYOffset
        T const* pSrc = pBaseSrc + (y2 * roContext.uBufferWidth) + roContext.uViewXOffset;

        // Upper left quadrant
        unsigned x1 = 0;
        for (
            unsigned x2 = roContext.uViewXOffset;
            x2 < roContext.uBufferWidth && x1 < roContext.uViewWidth;
            ++x1, ++x2
        ) {
            *pDst++ = *pSrc++;
        }

        if (x1 < roContext.uViewWidth) {
            // Start at 0, uViewYOffset
            pSrc = pBaseSrc + (y2 * roContext.uBufferWidth);

            // Upper right quadrant
            for (
                unsigned x2 = 0;
                x1 < roContext.uViewWidth;
                ++x1, ++x2
            ) {
                *pDst++ = *pSrc++;
            }
        }
    }

    if (y1 < roContext.uViewHeight) {
        for (
            unsigned y2 = 0;
            y1 < roContext.uViewHeight;
            ++y1, ++y2
        ) {
            // Start at uViewXOffset, 0
            T const* pSrc = pBaseSrc + (y2 * roContext.uBufferWidth) + roContext.uViewXOffset;

            // Lower left quadrant
            unsigned x1 = 0;
            for (
                unsigned x2 = roContext.uViewXOffset;
                x2 < roContext.uBufferWidth && x1 < roContext.uViewWidth;
                ++x1, ++x2
            ) {
                *pDst++ = *pSrc++;
            }

            if (x1 < roContext.uViewWidth) {
                // Start at 0, 0
                pSrc = pBaseSrc + (y2 * roContext.uBufferWidth);

                // Lower right quadrant
                for (
                    unsigned x2 = 0;
                    x1 < roContext.uViewWidth;
                    ++x1, ++x2
                ) {
                    *pDst++ = *pSrc++;
                }
            }
        }
    }
    return roContext.puImageBuffer;
}

}

#endif
