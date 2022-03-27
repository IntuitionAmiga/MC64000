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

#include <host/display/x11/manager.hpp>

using MC64K::Machine::Interpreter;
using MC64K::Machine::Nanoseconds;

namespace MC64K::StandardTestHost::Display {

const uint8 aPixelSize[] = {
    1, 4
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
    iWidth(uWidth),
    iHeight(uHeight)
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

    ::XStoreName(poDisplay, uWindowID, "MC64K");

    int iX11Depth = DefaultDepth(poDisplay, 0);
    std::fprintf(stderr, "Allocating Pixmap %d x %d x %d (bits)\n", iWidth, iHeight, iX11Depth);

    uPixmapID = ::XCreatePixmap(
        poDisplay,
        uWindowID,
        iWidth,
        iHeight,
        iX11Depth
    );

    std::fprintf(stderr, "Allocated Pixmap ID %lu\n", uPixmapID);

    pGC = ::XCreateGC(poDisplay, uPixmapID, 0, nullptr);
    std::fprintf(stderr, "Graphics Context at %p\n", pGC);

    oContext.uNumBufferPixels = uWidth * uHeight;
    oContext.uNumBufferBytes  = oContext.uNumBufferPixels * aPixelSize[uFormat];
    oContext.uBufferWidth     = uWidth;
    oContext.uBufferHeight    = uHeight;
    oContext.uFlags           = uFlags;
    oContext.uPixelFormat     = uFormat;
    oContext.uRateHz          = uRateHz < 1 ? 1 : uRateHz;
    oContext.poManager        = this;
    oContext.allocateBuffer();

    XImage* poImage = ::XCreateImage(
        poDisplay,
        DefaultVisual(poDisplay, DefaultScreen(poDisplay)),
        iX11Depth,//iDepth,
        ZPixmap,
        0, // offset
        (char*)oContext.puImageBuffer,
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
 * Update the display
 */
void X11Manager::updateDisplay() {
    ::Display* poDisplay = oDisplay.get();
    ::XPutImage(
        poDisplay,
        uPixmapID,
        pGC,
        oImage.get(),
        0, 0, 0, 0, iWidth, iHeight
    );
    ::XCopyArea(poDisplay, uPixmapID, uWindowID, pGC, 0, 0, iWidth, iHeight, 0, 0);
}

/**
 * Run the event loop.
 */
void X11Manager::runEventLoop() {

    ::Display* poDisplay = oDisplay.get();

    long iCurrentXInputFlags = configureInputMask();
    ::XSelectInput(poDisplay, uWindowID, iCurrentXInputFlags);

    Nanoseconds::Value uFrametime = 1000000000 / oContext.uRateHz;
    Nanoseconds::Value uIdle    = 0;
    Nanoseconds::Value uBegin   = Nanoseconds::mark();


    ulong uFrames = 0;

    oContext.uFlags |= FLAG_RUNNING;

    while (true) {

        Nanoseconds::Value uMark = Nanoseconds::mark();

        // Handle XEvents and flush the input..
        while (::XPending(poDisplay)) {
            ::XNextEvent(poDisplay, &oEvent);
            handleEvent();
        }

        // Trap 1 : Maybe a keypress or something triggered exit condition
        if (!(oContext.uFlags & FLAG_RUNNING)) {
            break;
        }

        // Check for changes to the handlers.
        long iNewXInputFlags = configureInputMask();
        if (iNewXInputFlags != iCurrentXInputFlags) {
            iCurrentXInputFlags = iNewXInputFlags;
            ::XSelectInput(poDisplay, uWindowID, iCurrentXInputFlags);
        }

        if (oContext.apVMCall[CALL_FRAME]) {
            invokeVMCallback(oContext.apVMCall[CALL_FRAME]);

            // Trap 2 : Maybe main callback triggered exit condition
            if (!(oContext.uFlags & FLAG_RUNNING)) {
                break;
            }
        }

//        Nanoseconds::Value uMark2 = Nanoseconds::mark();
        // Check if we need to copy the pixel buffer to the offscreen buffer
        if (oContext.uFlags & (FLAG_DRAW_BUFFER_NEXT_FRAME|FLAG_DRAW_BUFFER_ALL_FRAMES)) {
            oContext.updateBuffers();
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
            uIdle += (uFrametime - uElapsed);
            Nanoseconds::sleep(uFrametime - uElapsed);
        }
        ++uFrames;
    }

    Nanoseconds::Value uTotal = Nanoseconds::mark() - uBegin;
    std::fprintf(
        stderr,
        "Total: %lu, Idle: %lu, Free: %0.2f%%, %lu frames, %.2f fps\n",
        uTotal,
        uIdle,
        (100.0 * (float64)uIdle)/(float64)uTotal,
        uFrames,
        1e9 * (float64)uFrames / (float64)uTotal
    );
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

