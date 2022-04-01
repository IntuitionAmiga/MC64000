#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_X11_RAII_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_X11_RAII_HPP

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

namespace MC64K::StandardTestHost::Display::x11 {

extern uint8 const aPixelSize[];

/**
 * RAII Handle for Display Pointer
 */
class DisplayHandle {
    private:
        ::Display* poDisplay;

    public:
        DisplayHandle();
        ~DisplayHandle();
        DisplayHandle(DisplayHandle const&) = delete;
        DisplayHandle& operator=(DisplayHandle const&) = delete;

        ::Display* get() const;

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
        XImageHandle();
        ~XImageHandle();
        XImageHandle(XImageHandle const&) = delete;
        XImageHandle& operator=(XImageHandle const&) = delete;

        void set(::XImage* poImage);
        ::XImage* get() const;
};

/**
 * RAII Extension of Context to handle pixel buffer allocation
 */
struct Context : public Display::Context {

    /**
     * Main allocation
     */
    uint8* puData;

    /**
     * Image buffer location. This is is what the VM writes to. This has the dimensions uBufferWidth x uBufferHeight
     */
    uint8* puImageBuffer;

    Context();
    ~Context();
    Context(Context const&) = delete;
    Context& operator=(Context const&) = delete;

    void allocateBuffer();
    void* updateBuffers();

};


/**
 * DisplayHandle inlines
 */
inline DisplayHandle::DisplayHandle() : poDisplay(::XOpenDisplay(nullptr)) {
    if (!poDisplay) {
        throw Error();
    }
    std::fprintf(stderr, "DisplayHandle RAII: Opened Display Handle %p\n", poDisplay);
}

inline DisplayHandle::~DisplayHandle() {
    if (poDisplay) {
        ::XCloseDisplay(poDisplay);
        std::fprintf(stderr, "DisplayHandle RAII: Closed Display Handle %p\n", poDisplay);
    }
}

inline ::Display*  DisplayHandle::get() const {
    return poDisplay;
}


/**
 * XImageHandle inlines
 */
inline XImageHandle::XImageHandle() : poImage(nullptr) {

}

inline XImageHandle::~XImageHandle() {
    if (poImage) {
        // Don't allow X to destroy the buffer data as it's managed by us.
        poImage->data = nullptr;
        XDestroyImage(poImage);
        std::fprintf(stderr, "XImageHandle RAII: Destoyed Image Handle %p\n", poImage);
    }
}

inline void XImageHandle::set(::XImage* poImage) {
    this->poImage = poImage;
}

inline ::XImage* XImageHandle::get() const {
    return poImage;
}


/**
 * X11Context inlines
 */

Context::Context(): Display::Context(), puData(nullptr), puImageBuffer(nullptr) {

}

Context::~Context() {
    if (puData) {
        delete[] puData;
        std::fprintf(stderr, "X11Context RAII: Freed Buffers\n");
    }
}


};
#endif
