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
#include <machine/register.hpp>
#include <machine/timing.hpp>

#include <sys/select.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using MC64K::Machine::Interpreter;
using MC64K::Machine::Nanoseconds;

namespace MC64K::StandardTestHost::Display {

const uint8 aPixelSize[] = {
    1, 4
};

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

    ~X11Context() {
        if (pDisplayBuffer.puByte) {
            delete[] pDisplayBuffer.puByte;
            std::fprintf(stderr, "X11Context RAII: Freed Pixel Buffer\n");
        }
    }

    void allocateBuffer() {
        size_t uBufferSize = uWidth * uHeight * aPixelSize[uPixelFormat];
        pDisplayBuffer.puByte = new uint8[uBufferSize];
        std::fprintf(
            stderr,
            "X11Context RAII: Allocated Pixel Buffer %d x %d x %d at %p\n",
            (int)uWidth, (int)uHeight, (int)aPixelSize[uPixelFormat],
            pDisplayBuffer.puByte
        );
    }
};

/**
 * X11 Implementation of the Manager interface.
 */
class X11Manager : public Manager {
    private:
        X11Context    oContext;
        ::XEvent      oEvent;
        DisplayHandle oDisplay;
        XImageHandle  oImage;
        ::Window      uWindowID;
        ::Pixmap      uPixmapID;
        GC            pGC;
        int           iWindowFD, iWidth, iHeight, iDepth;
;
    public:
        /**
         * Constructor. Follows RAII principle.
         *
         * @param  uint16 uWidth
         * @param  uint16 uHeight
         * @param  uint16 uFlags
         * @param  uint8  uFormat
         * @param  uint8  uRateHz
         * @throws Error
         * @throws std::bad_alloc
         */
        X11Manager(uint16 uWidth, uint16 uHeight, uint16 uFlags, uint8 uFormat, uint8 uRateHz);
        virtual ~X11Manager();

        /**
         * @inheritDoc
         */
        Context* getContext();

        /**
         * @inheritDoc
         */
        void runEventLoop();

    private:
        /**
         * Returns a read-only type-cast reference to the actual X11 event structure.
         * We do this because XEvent is actually a union type of all possible X11 events.
         */
        template<typename T>
        T const& event() const {
            return *((T const*)&oEvent);
        }

        /**
         * Set up the display input handling based on the currently set callbacks
         */
        long configureInputMask();

        /**
         * Handle an X11 event
         */
        void handleEvent();

        /**
         * Invoke a VM callback.
         */
        void invokeVMCallback(Interpreter::VMCodeEntryPoint pBytecode);
};

/**
 * Constructor. We use RAII here and throw exceptions if a requirement can't be met.
 */
X11Manager::X11Manager(uint16 uWidth, uint16 uHeight, uint16 uFlags, uint8 uFormat, uint8 uRateHz):
    oContext(),
    oEvent(),
    oDisplay(),
    oImage(),
    uWindowID(0),
    uPixmapID(0),
    pGC(nullptr),
    iWindowFD(0),
    iWidth(uWidth),
    iHeight(uHeight),
    iDepth(aPixelSize[uFormat] << 3)
{
    ::Display* poDisplay = oDisplay.get();
    std::fprintf(stderr, "X11Manager: Found Display at %p\n", poDisplay);
    uWindowID = ::XCreateSimpleWindow(
        poDisplay,
        DefaultRootWindow(poDisplay),
        0, 0,
        uWidth, uHeight,
        0,
        0,
        BlackPixel(poDisplay, 0)
    );

    std::fprintf(stderr, "X11Manager: Got window handle %u\n", (unsigned)uWindowID);

    ::XStoreName(poDisplay, uWindowID, "MC64K [Press Esc to exit]");

    int iX11Depth = DefaultDepth(poDisplay, 0);
    std::fprintf(stderr, "Allocating Pixmap %d x %d x %d (bits)\n", iWidth, iHeight, iX11Depth);

    uPixmapID = ::XCreatePixmap(
        poDisplay,
        uWindowID,
        iWidth,
        iHeight,
        iX11Depth//iDepth
    );

    std::fprintf(stderr, "Allocated Pixmap ID %lu\n", uPixmapID);

    pGC = ::XCreateGC(poDisplay, uPixmapID, 0, nullptr);
    std::fprintf(stderr, "Graphics Context at %p\n", pGC);


    oContext.uNumPixels   = uWidth * uHeight;
    oContext.uNumBytes    = oContext.uNumPixels * aPixelSize[uFormat];
    oContext.uWidth       = uWidth;
    oContext.uHeight      = uHeight;
    oContext.uFlags       = uFlags;
    oContext.uPixelFormat = uFormat;
    oContext.uRateHz      = uRateHz < 1 ? 1 : uRateHz;
    oContext.poManager    = this;
    oContext.allocateBuffer();

    XImage* poImage = ::XCreateImage(
        poDisplay,
        DefaultVisual(poDisplay, DefaultScreen(poDisplay)),
        iX11Depth,//iDepth,
        ZPixmap,
        0, // offset
        (char*)oContext.pDisplayBuffer.puByte,
        iWidth,
        iHeight,
        32, // bitmap_pad
        iWidth * 4  // bytes_per_line
    );
    if (!poImage) {
        throw Error();
    }
    oImage.set(poImage);

    std::fprintf(stderr, "XImage allocated at %p\n", poImage);

    // Open the window
    ::XMapWindow(poDisplay, uWindowID);
    ::XFlush(poDisplay);

    std::fprintf(stderr, "X11Manager: RAII Complete, we live at: %p\n", this);
}

/**
 * Destructor.
 */
X11Manager::~X11Manager() {
    if (uPixmapID) {
        ::XFreePixmap(oDisplay.get(), uPixmapID);
    }
    std::fprintf(stderr, "X11Manager destroyed\n");
}

/**
 * Get the context.
 */
Context* X11Manager::getContext() {
    return &oContext;
}

/**
 * Run the event loop.
 */
void X11Manager::runEventLoop() {

    ::Display* poDisplay = oDisplay.get();

    long iCurrentXInputFlags = configureInputMask();
    ::XSelectInput(poDisplay, uWindowID, iCurrentXInputFlags);

    Nanoseconds::Value uFrametime = 1000000000 / oContext.uRateHz;

    // Get the file descriptor associated with the display
    iWindowFD = ConnectionNumber(poDisplay);

    while (true) {

        Nanoseconds::Value uMark = Nanoseconds::mark();

        // Handle XEvents and flush the input..
        while (::XPending(poDisplay)) {
            ::XNextEvent(poDisplay, &oEvent);
            handleEvent();
            if (oEvent.type == KeyPress && event<::XKeyEvent>().keycode == 9) {
                return;
            }
        }

        // Check for changes to the handlers.
        long iNewXInputFlags = configureInputMask();
        if (iNewXInputFlags != iCurrentXInputFlags) {
            iCurrentXInputFlags = iNewXInputFlags;
            ::XSelectInput(poDisplay, uWindowID, iCurrentXInputFlags);
        }

        if (oContext.apVMCall[CALL_FRAME]) {
            invokeVMCallback(oContext.apVMCall[CALL_FRAME]);
        }

        // Check if we need to copy the pixel buffer to the offscreen buffer
        if (oContext.uFlags & (FLAG_DRAW_BUFFER_NEXT_FRAME|FLAG_DRAW_BUFFER_ALL_FRAMES)) {
            ::XPutImage(
                poDisplay,
                uPixmapID,
                pGC,
                oImage.get(),
                0, 0, 0, 0, iWidth, iHeight
            );
            oContext.uFlags &= (uint16)~FLAG_DRAW_BUFFER_NEXT_FRAME;
        }

        // Check if we need to flip the offscreen buffer
        if (oContext.uFlags & (FLAG_FLIP_NEXT_FRAME|FLAG_FLIP_ALL_FRAMES)) {
            ::XCopyArea(poDisplay, uPixmapID, uWindowID, pGC, 0, 0, iWidth, iHeight, 0, 0);
            oContext.uFlags &= (uint16)~FLAG_FLIP_NEXT_FRAME;
        }
        Nanoseconds::Value uElapsed = Nanoseconds::mark() - uMark;

        if (uElapsed < uFrametime) {
            Nanoseconds::sleep(uFrametime - uElapsed);
        }
    }
}

/**
 * Configures the X11 Input Mask based on which VM callbacks are set. For example. there is no point
 * receiving mouse movements if there isn't a callback to handle them.
 */
long X11Manager::configureInputMask() {
    long int iXInputMask = 0;
    iXInputMask |= (oContext.apVMCall[CALL_KEY_PRESS]      ? KeyPressMask      : 0);
    iXInputMask |= (oContext.apVMCall[CALL_KEY_RELEASE]    ? KeyReleaseMask    : 0);
    iXInputMask |= (oContext.apVMCall[CALL_MOVEMENT]       ? PointerMotionMask : 0);
    iXInputMask |= (oContext.apVMCall[CALL_BUTTON_PRESS]   ? ButtonPressMask   : 0);
    iXInputMask |= (oContext.apVMCall[CALL_BUTTON_RELEASE] ? ButtonReleaseMask : 0);
    return iXInputMask;
}

/**
 * Handles an XEvent. Due to the nature of the protocol, we can't assume that just because we have
 * disabled a VM handler we won't have any events still waiting for it. This means we still have to
 * check each handler before we attempt to call it.
 */
void X11Manager::handleEvent() {
    switch (oEvent.type) {
        case NoExpose:
            break;
        case KeyPress:
            if (oContext.apVMCall[CALL_KEY_PRESS]) {
                oContext.uEventRawCode = (uint16) event<::XKeyEvent>().keycode;
                oContext.uEventRawMask = (uint16) event<::XKeyEvent>().state;
                oContext.uPositionX    = (uint16) event<::XKeyEvent>().x;
                oContext.uPositionY    = (uint16) event<::XKeyEvent>().y;
                invokeVMCallback(oContext.apVMCall[CALL_KEY_PRESS]);
            }
            break;
        case KeyRelease:
            if (oContext.apVMCall[CALL_KEY_RELEASE]) {
                oContext.uEventRawCode = (uint16) event<::XKeyEvent>().keycode;
                oContext.uEventRawMask = (uint16) event<::XKeyEvent>().state;
                oContext.uPositionX    = (uint16) event<::XKeyEvent>().x;
                oContext.uPositionY    = (uint16) event<::XKeyEvent>().y;
                invokeVMCallback(oContext.apVMCall[CALL_KEY_RELEASE]);
            }
            break;
        case MotionNotify:
            if (oContext.apVMCall[CALL_MOVEMENT]) {
                oContext.uEventRawMask = (uint16) event<::XKeyEvent>().state;
                oContext.uPositionX    = (uint16) event<::XKeyEvent>().x;
                oContext.uPositionY    = (uint16) event<::XKeyEvent>().y;
                invokeVMCallback(oContext.apVMCall[CALL_MOVEMENT]);
            }
            break;
        case ButtonPress:
            if (oContext.apVMCall[CALL_BUTTON_PRESS]) {
                oContext.uEventRawCode = (uint16) event<::XButtonEvent>().button;
                oContext.uEventRawMask = (uint16) event<::XButtonEvent>().state;
                oContext.uPositionX    = (uint16) event<::XButtonEvent>().x;
                oContext.uPositionY    = (uint16) event<::XButtonEvent>().y;
                invokeVMCallback(oContext.apVMCall[CALL_BUTTON_PRESS]);
            }
            break;
        case ButtonRelease:
            if (oContext.apVMCall[CALL_BUTTON_RELEASE]) {
                oContext.uEventRawCode = (uint16) event<::XButtonEvent>().button;
                oContext.uEventRawMask = (uint16) event<::XButtonEvent>().state;
                oContext.uPositionX    = (uint16) event<::XButtonEvent>().x;
                oContext.uPositionY    = (uint16) event<::XButtonEvent>().y;
                invokeVMCallback(oContext.apVMCall[CALL_BUTTON_RELEASE]);
            }
            break;
        default:
            std::printf("Other Event %d\n", oEvent.type);
            break;
    }
}

void X11Manager::invokeVMCallback(Interpreter::VMCodeEntryPoint pBytecode) {
    Interpreter::setProgramCounter(pBytecode);
    Interpreter::gpr<ABI::PTR_REG_0>().pAny = &oContext;
    Interpreter::run();
}

Manager* createManager(uint16 uWidth, uint16 uHeight, uint16 uFlags, uint8 uFormat, uint8 uRateHz) {
    return new X11Manager(uWidth, uHeight, uFlags, uFormat, uRateHz);
}

} // namespace

