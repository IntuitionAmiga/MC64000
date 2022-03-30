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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <host/standard_test_host_mem.hpp>
#include <host/standard_test_host_display.hpp>
#include <host/display/context.hpp>
#include <host/display/x11/device.hpp>

namespace MC64K::StandardTestHost::Display::x11 {

uint8 const aPixelSize[] = {
    1, 4
};

void Context::allocateBuffer() {

    // Note that the buffer and view widths have been pre-aligned to 32 pixels

    uNumBufferPixels = uBufferWidth * uBufferHeight;
    uNumViewPixels   = uViewWidth   * uViewHeight;

    std::fprintf(
        stderr,
        "Buffer: %d x %d [%u pixels]\nView: %d x %d [%u pixels]\n",
        (int)uBufferWidth, (int)uBufferHeight, uNumBufferPixels,
        (int)uViewWidth,   (int)uViewHeight,   uNumViewPixels
    );

    switch (uPixelFormat) {
        case PXL_ARGB_32: {
            // Calculate the VM buffer size
            uNumBufferBytes = uNumBufferPixels * sizeof(uint32);

            // Calculate the total allocation size,
            // including the viewport sized transfer
            // buffer
            size_t uTotalAlloc = uNumBufferBytes + uNumViewPixels * sizeof(uint32);

            puPalette = nullptr;
            oDisplayBuffer.puByte = puData = new uint8[uTotalAlloc];
            puImageBuffer = puData + uNumBufferBytes;
            break;
        }
        case PXL_LUT_8: {
            uNumBufferBytes = uNumBufferPixels;

            // Calculate the total allocation size including the viewport sized transfer buffer and palette.
            size_t uTotalAlloc = uNumBufferBytes + (uNumViewPixels + 256) * sizeof(uint32);

            oDisplayBuffer.puByte =
                puData    = new uint8[uTotalAlloc];
            puPalette     = (uint32*)(puData + uNumBufferBytes);
            puImageBuffer = (uint8*)(puPalette + 256);
            break;
        }
        default: {
            throw Error();
            break;
        }
    }
    std::fprintf(
        stderr,
        "X11Context RAII: Allocated Pixel Buffer %d x %d x %d at %p\n",
        (int)uBufferWidth, (int)uBufferHeight, (int)aPixelSize[uPixelFormat],
        oDisplayBuffer.puByte
    );
}

void* Context::updateBuffers() {
    if (uPixelFormat == PXL_LUT_8 && puPalette) {
        uint32* pDst = (uint32*)puImageBuffer;
        uint8*  pSrc = oDisplayBuffer.puByte;
        uint32  uCnt = uNumBufferPixels;
        while (uCnt--) {
            *pDst++ = puPalette[*pSrc++];
        }
        return puImageBuffer;
    }
    return oDisplayBuffer.puByte;

}

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
    uint32*       pDst = (uint32*)roContext.puImageBuffer;
    uint8 const*  pSrc = roContext.oDisplayBuffer.puByte;
    uint32        uCnt = roContext.uNumBufferPixels;
    while (uCnt--) {
        *pDst++ = roContext.puPalette[*pSrc++];
    }
    return roContext.puImageBuffer;
}

/**
 * Transfer routine for:
 *
 * Pixel Format  == LUT8
 * Buffer Size   != View Size
 * X Offset      != 0
 * Y Offset      != 0
 * Filth Script  == nullptr
 */
void* updateLUT8Generic(Context& roContext) {
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
            *pDst++ = roContext.puPalette[*pSrc++];
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
                *pDst++ = roContext.puPalette[*pSrc++];
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
            uint8 const* pSrc = roContext.oDisplayBuffer.puByte + roContext.uViewXOffset;

            // Lower left quadrant
            unsigned x1 = 0;
            for (
                unsigned x2 = roContext.uViewXOffset;
                x2 < roContext.uBufferWidth && x1 < roContext.uViewWidth;
                ++x1, ++x2
            ) {
                *pDst++ = roContext.puPalette[*pSrc++];
            }

            if (x1 < roContext.uViewWidth) {

                // Start at 0, 0
                pSrc = roContext.oDisplayBuffer.puByte;

                // Lower right quadrant
                for (
                    unsigned x2 = 0;
                    x1 < roContext.uViewWidth;
                    ++x1, ++x2
                ) {
                    *pDst++ = roContext.puPalette[*pSrc++];
                }
            }
        }
    }
    return roContext.puImageBuffer;
}

void* updateLUT8Filth(Context& roContext) {
    // TODO
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
    return roContext.puImageBuffer; // Just return the display buffer as-is
}

void* updateARGB32Generic(Context& roContext) {
    return roContext.puImageBuffer; // Just return the display buffer as-is
}

void* updateARGB32Filth(Context& roContext) {
    // TODO
    return roContext.puImageBuffer;
}


} // namespace

