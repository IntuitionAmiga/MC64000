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

#include <host/standard_test_host_mem.hpp>
#include <host/standard_test_host_display.hpp>
#include <host/display/context.hpp>
#include <machine/register.hpp>

#include <sys/select.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Display {

const uint8 aPixelSize[] = {
    1, 4
};

class X11Manager : public Manager {
    private:
        Context    oContext;
        ::XEvent   oEvent;
        ::fd_set   oInputFD;
        ::timeval  oTimeVal;
        ::Display* poDisplay;
        ::Window   uWindowID;
        int        iWindowFD;

    public:
        X11Manager(uint16 uWidth, uint16 uHeight, PixelFormat uFormat, uint16 uFlags);
        virtual ~X11Manager();

        Context* getContext();
        void     runEventLoop();

    private:
        void    invokeVMCallback(uint8 const* puBytecode);
};

X11Manager::X11Manager(uint16 uWidth, uint16 uHeight, PixelFormat uFormat, uint16 uFlags):
    oContext(),
    oEvent(),
    oInputFD(),
    oTimeVal(),
    poDisplay(nullptr),
    uWindowID(0),
    iWindowFD(0)
{
    poDisplay = ::XOpenDisplay(nullptr);
    if (!poDisplay) {
        throw Error();
    }
    std::fprintf(stderr, "X11Manager: Found Display at %p\n", poDisplay);

    uWindowID = ::XCreateSimpleWindow(
        poDisplay,
        RootWindow(poDisplay, 0),
        128, 128,
        uWidth, uHeight,
        0,
        BlackPixel(poDisplay, 0),
        BlackPixel(poDisplay, 0)
    );

    std::fprintf(stderr, "X11Manager: Got window handle %u\n", (unsigned)uWindowID);

    ::XSelectInput(poDisplay, uWindowID,
        ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
        ButtonPressMask | ButtonReleaseMask  | StructureNotifyMask
    );

    ::XStoreName(poDisplay, uWindowID, "MC64K");

    // Open the window
    ::XMapWindow(poDisplay, uWindowID);
    ::XFlush(poDisplay);

    size_t uBufferSize = uWidth * uHeight * aPixelSize[uFormat];
    oContext.pDisplayBuffer.puByte = new uint8[uBufferSize];
    oContext.uWidth                = uWidth;
    oContext.uHeight               = uHeight;
    oContext.uFlags                = uFlags;
    oContext.uPixelFormat          = uFormat;
    oContext.poManager             = this;

    std::fprintf(
        stderr,
        "X11Manager: Soft Buffer %d x %d x %d allocated at %p\n",
        (int)uWidth, (int)uHeight, (int)aPixelSize[uFormat],
        oContext.pDisplayBuffer.puByte
    );

    std::fprintf(stderr, "X11Manager: RAII Complete, we live at: %p\n", this);
}

X11Manager::~X11Manager() {
    if (poDisplay) {
        ::XCloseDisplay(poDisplay);
    }

    delete[] oContext.pDisplayBuffer.puByte;
    std::fprintf(stderr, "X11Manager destroyed\n");
}

Context* X11Manager::getContext() {
    return &oContext;
}

void     X11Manager::runEventLoop() {

    // Get the file descriptor associated with the display
    iWindowFD = ConnectionNumber(poDisplay);

    // Event Loop. Use select() to react to both X11 events and timer.
    int iRunning = 1;
    while (iRunning) {
        // Create a File Description Set containing iWindowFD
        FD_ZERO(&oInputFD);
        FD_SET(iWindowFD, &oInputFD);

        oTimeVal.tv_usec = 100000; // ten times a second
        oTimeVal.tv_sec  = 0;

        int iReadyFD = select(iWindowFD + 1, &oInputFD, nullptr, nullptr, &oTimeVal);
        if (iReadyFD > 0) {
            //std::printf("Event %d\r", iEvents);
        } else if (iReadyFD == 0) {
            invokeVMCallback(oContext.puOnFrameVMEntry);
        }

        // Handle XEvents and flush the input. If a key is pressed, we'll exit the main loop.
        while (::XPending(poDisplay)) {
            ::XNextEvent(poDisplay, &oEvent);
            invokeVMCallback(oContext.puOnEventVMEntry);
            if (oEvent.type == KeyPress) {
                iRunning = 0;
                break;
            }
        }
        std::fflush(stdout);
    }
}

void X11Manager::invokeVMCallback(uint8 const* puBytecode) {
    if (puBytecode) {
        Interpreter::setProgramCounter(puBytecode);
        Interpreter::gpr<ABI::PTR_REG_0>().pAny = &oContext;
        Interpreter::run();
    }
}

Manager* createManager(uint16 uWidth, uint16 uHeight, PixelFormat uFormat, uint16 uFlags) {
    return new X11Manager(uWidth, uHeight, uFormat, uFlags);
}

} // namespace

