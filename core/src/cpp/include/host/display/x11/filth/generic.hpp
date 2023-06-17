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

namespace MC64K::StandardTestHost::Display::x11 {

/**
 * Updates the visible portion of an 8-bit surface.
 */
void* updateLUT8Generic(Context& roContext) {
    if (uint32 const* puPalette = roContext.puPalette) {
        uint32* pDst = (uint32*)roContext.puImageBuffer;
        unsigned y1 = 0;
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
                *pDst++ = puPalette[*pSrc++];
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
                    *pDst++ = puPalette[*pSrc++];
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
                    *pDst++ = puPalette[*pSrc++];
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
                        *pDst++ = puPalette[*pSrc++];
                    }
                }
            }
        }

    }
    return roContext.puImageBuffer;
}

/**
 * Updates the visible portion of a 32-bit surface.
 */
void* updateARGB32Generic(Context& roContext) {

    uint32* pDst = (uint32*)roContext.puImageBuffer;
    unsigned y1 = 0;
    for (
        unsigned y2 = roContext.uViewYOffset;
        y2 < roContext.uBufferHeight && y1 < roContext.uViewHeight;
        ++y1, ++y2
    ) {

        // Start at uViewXOffset, uViewYOffset
        uint32 const* pSrc = roContext.oDisplayBuffer.puLong + (y2 * roContext.uBufferWidth) +
            roContext.uViewXOffset;

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
            pSrc = roContext.oDisplayBuffer.puLong + (y2 * roContext.uBufferWidth);

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
            uint32 const* pSrc = roContext.oDisplayBuffer.puLong + (y2 * roContext.uBufferWidth) + roContext.uViewXOffset;

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
                pSrc = roContext.oDisplayBuffer.puLong + (y2 * roContext.uBufferWidth);

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
