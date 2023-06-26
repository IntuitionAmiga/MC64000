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

#include <host/display/x11/filth/simple.hpp>
#include <host/display/x11/filth/generic.hpp>
#include <host/display/x11/filth/script.hpp>

#include <host/display/x11/filth/conversion.hpp>

#include <host/display/x11/quirks.hpp>

namespace MC64K::StandardTestHost::Display::x11 {

/**
 * Word size of pixels, by format
 */
uint8 const aPixelSize[] = {
    sizeof(Format::LUT8::Pixel),       // PXL_LUT_8
    sizeof(Format::LUT8::Pixel),       // PXL_HAM_555
    sizeof(Format::RGB555::Pixel),     // PXL_RGB_555
    sizeof(Format::ARGB32::Pixel),     // PXL_ARGB_32
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

    uint32 uPixelSize = aPixelSize[uPixelFormat];

    size_t uTotalAlloc = 0;

    switch (uPixelFormat) {

        case PXL_LUT_8: {
            // 8 bit index, 256 colours
            uNumBufferBytes = uNumBufferPixels;

            // Calculate the total allocation size including the viewport sized transfer buffer and palette.
            uTotalAlloc = uNumBufferBytes + (uNumViewPixels + 256) * sizeof(Format::ARGB32::Pixel);

            oDisplayBuffer.puByte =
            puData                = new uint8[uTotalAlloc];
            oPaletteData.puLong   = (Format::ARGB32::Pixel*)(puData + uNumBufferBytes);
            puImageBuffer         = (Format::LUT8::Pixel*)(oPaletteData.puLong + 256);
            break;
        }

        case PXL_HAM_555: {
            // 5 bit index, 32 colours
            uNumBufferBytes = uNumBufferPixels;

            // Calculate the total allocation size including the viewport sized transfer buffer and palette.
            uTotalAlloc = uNumBufferBytes + (uNumViewPixels + 32) * sizeof(Format::RGB555::Pixel);

            oDisplayBuffer.puByte =
            puData                = new uint8[uTotalAlloc];
            oPaletteData.puWord   = (Format::RGB555::Pixel*)(puData + uNumBufferBytes);
            puImageBuffer         = (Format::LUT8::Pixel*)(oPaletteData.puWord + 32);
            break;
        }

        // Basic symmetric formats have the same allocation semantics
		case PXL_RGB_555:
        case PXL_ARGB_32: {
            // Calculate the VM buffer size
            uNumBufferBytes = uNumBufferPixels * uPixelSize;

            // Calculate the total allocation size,
            // including the viewport sized transfer
            // buffer
            uTotalAlloc = uNumBufferBytes + uNumViewPixels * uPixelSize;

            oPaletteData.puAny    = nullptr;
            oDisplayBuffer.puByte = puData = new uint8[uTotalAlloc];
            puImageBuffer         = puData + uNumBufferBytes;
            break;
		}

        default: {
            throw Error();
            break;
        }
    }
    std::fprintf(
        stderr,
        "X11Context RAII: Allocated Pixel Buffer %d x %d x %d at %p, total allocation is %u bytes\n",
        (int)uBufferWidth, (int)uBufferHeight, (int)aPixelSize[uPixelFormat],
        oDisplayBuffer.puByte,
        (unsigned)uTotalAlloc
    );
}

typedef void* (*UpdateFunction)(Context& roContext);

/**
 * Index: (width|height|offset diff) << 2 | format
 */
UpdateFunction aUpdateFunctions[] = {
    updatePaletted<PaletteTo32Bit<Format::ARGB32>>,
    updatePaletted<PaletteHAM555To15Bit<Format::RGB555>>,
    updateRGB<Format::RGB555>,
    updateRGB<Format::ARGB32>,
    updatePalettedViewModified<PaletteTo32Bit<Format::ARGB32>>,
    updatePalettedViewModified<PaletteHAM555To15Bit<Format::RGB555>>,
    updateRGBViewModified<Format::RGB555>,
    updateRGBViewModified<Format::ARGB32>,
};

/**
 * Index: format
 */
UpdateFunction aComplexUpdateFunctions[] = {
    updatePalettedScripted<PaletteTo32Bit<Format::ARGB32>>,   // Format::ARGB32::Pixel is the the palette format here
    updatePalettedScripted<PaletteHAM555To15Bit<Format::RGB555>>, // Format::RGB555::Pixel is the the palette format here
    updateRGBScripted<Format::RGB555>,
    updateRGBScripted<Format::ARGB32>,
};

void* Context::updateBuffers() {

    // If there is a FILTH script, it takes precedence as it can change any of the other attributes
    if (puFilthScript) {
        return aComplexUpdateFunctions[uPixelFormat](*this);
    }

    // Otherwise choose between simple and generic based on other factors
    unsigned uSelect = uPixelFormat | (
        (
            uViewXOffset | uViewYOffset | // Nonzero View offsets ?
            (uViewWidth ^ uBufferWidth) | // Buffer/View width difference ?
            (uViewHeight ^ uBufferHeight) // Buffer/View height difference ?
        ) ? 4 : 0);

    return aUpdateFunctions[uSelect](*this);
}

} // namespace

