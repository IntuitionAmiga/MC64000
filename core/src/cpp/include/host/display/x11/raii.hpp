#ifndef __MC64K_STANDARD_TEST_HOST_DISPLAY_X11_RAII_HPP__
    #define __MC64K_STANDARD_TEST_HOST_DISPLAY_X11_RAII_HPP__

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
#include <ctime>
#include <new>
#include <host/standard_test_host_mem.hpp>
#include <host/standard_test_host_display.hpp>
#include <host/display/context.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace MC64K::StandardTestHost::Display {

extern const uint8 aPixelSize[];

/**
 * RAII Handle for Display Pointer
 */
class DisplayHandle {
    private:
        ::Display* poDisplay;

    public:
        DisplayHandle(): poDisplay(::XOpenDisplay(nullptr)) {
            if (!poDisplay) {
                throw Error();
            }
            std::fprintf(stderr, "DisplayHandle RAII: Opened Display Handle %p\n", poDisplay);
        }

        ~DisplayHandle() {
            if (poDisplay) {
                ::XCloseDisplay(poDisplay);
                std::fprintf(stderr, "DisplayHandle RAII: Closed Display Handle %p\n", poDisplay);
            }
        }

        /**
         * Obtain the handle
         */
        ::Display* get() {
            return poDisplay;
        }
};

/**
 * RAII Handle for XImage Pointer. Destroys the data pointer so that the destroy function
 * doesn't free our externally allocated image data for us as that's a double free waiting
 * to happen.
 */
class XImageHandle {
    private:
        ::XImage* poImage;

    public:
        XImageHandle() : poImage(nullptr) {}

        ~XImageHandle() {
            if (poImage) {
                // Don't allow X to destroy the buffer data as it's managed by us.
                poImage->data = nullptr;
                XDestroyImage(poImage);
                std::fprintf(stderr, "XImageHandle RAII: Destoyed Image Handle %p\n", poImage);
            }
        }

        /**
         * Set the handle
         */
        void set(::XImage* poImage) {
            this->poImage = poImage;
        }

        /**
         * Obtain the handle
         */
        ::XImage* get() {
            return poImage;
        }
};

/**
 * RAII Extension of Context to handle pixel buffer allocation
 */
struct X11Context : public Context {

    uint8* puData;
    uint8* puImageBuffer;

    X11Context(): Context(), puData(nullptr), puImageBuffer(nullptr) {}

    ~X11Context() {
        if (puData) {
            delete[] puData;
            std::fprintf(stderr, "X11Context RAII: Freed Buffers\n");
        }
    }

    void allocateBuffer() {
        uNumPixels = uWidth * uHeight;

        switch (uPixelFormat) {
            case PXL_ARGB_32: {
                uNumBytes = uNumPixels * sizeof(uint32);
                puPalette = nullptr;
                oDisplayBuffer.puByte =
                    puImageBuffer =
                    puData        = new uint8[uNumBytes];
                break;
            }
            case PXL_LUT_8: {
                uNumBytes = uNumPixels;

                // Align to an 8 byte size
                size_t uBufferSize = (uNumPixels + 7) & ~7;

                // Allocate space for 32-bit buffer, 256 32-bit palette entries and the 8-bit buffer
                oDisplayBuffer.puByte =
                    puData    = new uint8[((uBufferSize + 256) * sizeof(uint32)) + uBufferSize];
                puPalette     = (uint32*)(puData + uBufferSize);
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
            (int)uWidth, (int)uHeight, (int)aPixelSize[uPixelFormat],
            oDisplayBuffer.puByte
        );
    }

    void updateBuffers() {
        if (uPixelFormat == PXL_LUT_8 && puPalette) {
            uint32* pDst = (uint32*)puImageBuffer;
            uint8*  pSrc = oDisplayBuffer.puByte;
            uint32  uCnt = uNumPixels;
            while (uCnt--) {
                *pDst++ = puPalette[*pSrc++];
            }
        }
    }
};

};
#endif
