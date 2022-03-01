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

#include <sys/select.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Display {

const uint8 aPixelSize[] = {
    1, 4
};

/**
 * RAII Handle for Display Pointer
 */
class X11Display {
    private:
        ::Display* poDisplay;

    public:
        X11Display(): poDisplay(::XOpenDisplay(nullptr)) {
            if (!poDisplay) {
                throw Error();
            }
            std::fprintf(stderr, "X11Display RAII: Opened Display Handle %p\n", poDisplay);
        }

        ~X11Display() {
            if (poDisplay) {
                ::XCloseDisplay(poDisplay);
                std::fprintf(stderr, "X11Display RAII: Closed Display Handle %p\n", poDisplay);
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
        X11Context oContext;
        ::XEvent   oEvent;
        ::fd_set   oInputFD;
        ::timeval  oTimeVal;
        X11Display oDisplay;
        ::Window   uWindowID;
        int        iWindowFD;

    public:
        /**
         * Constructor. Follows RAII principle.
         *
         * @param  uint16 uWidth
         * @param  uint16 uHeight
         * @param  PixelFormat uFormat
         * @param  uint16 uFlags
         * @throws Error
         * @throws std::bad_alloc
         */
        X11Manager(uint16 uWidth, uint16 uHeight, PixelFormat uFormat, uint16 uFlags);
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
         * Set up the display input handling based on the flags
         */
        void configureInput(uint16 uFlags);

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
X11Manager::X11Manager(uint16 uWidth, uint16 uHeight, PixelFormat uFormat, uint16 uFlags):
    oContext(),
    oEvent(),
    oInputFD(),
    oTimeVal(),
    oDisplay(),
    uWindowID(0),
    iWindowFD(0)
{
    ::Display* poDisplay = oDisplay.get();
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

    ::XStoreName(poDisplay, uWindowID, "MC64K [Press Esc to exit]");

    configureInput(uFlags);

    // Open the window
    ::XMapWindow(poDisplay, uWindowID);
    ::XFlush(poDisplay);

    oContext.uWidth       = uWidth;
    oContext.uHeight      = uHeight;
    oContext.uFlags       = uFlags;
    oContext.uPixelFormat = uFormat;
    oContext.poManager    = this;
    oContext.allocateBuffer();
    std::fprintf(stderr, "X11Manager: RAII Complete, we live at: %p\n", this);
}

/**
 * Destructor.
 */
X11Manager::~X11Manager() {
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

    // Get the file descriptor associated with the display
    iWindowFD = ConnectionNumber(poDisplay);

    // Event Loop. Use select() to react to both X11 events and timer.
    bool bRunning = true;

    uint16 uCurrentFlags = oContext.uFlags;

    while ( bRunning ) {
        // Create a File Description Set containing iWindowFD
        FD_ZERO(&oInputFD);
        FD_SET(iWindowFD, &oInputFD);

        oTimeVal.tv_usec = 0;
        oTimeVal.tv_sec  = 1;

        int iReadyFD = select(iWindowFD + 1, &oInputFD, nullptr, nullptr, &oTimeVal);
        if (iReadyFD == 0 && oContext.apVMCall[CALL_FRAME]) {
            invokeVMCallback(oContext.apVMCall[CALL_FRAME]);
        }

        // Handle XEvents and flush the input. If a key is pressed, we'll exit the main loop.
        while (::XPending(poDisplay)) {
            ::XNextEvent(poDisplay, &oEvent);
            handleEvent();
            if (oEvent.type == KeyPress && event<::XKeyEvent>().keycode == 9) {
                bRunning = false;
                break;
            }
        }

        // Check to see if any of the VM code running has changed flags for our display that we need
        // to know about.
        if (oContext.uFlags != uCurrentFlags) {
            uCurrentFlags = oContext.uFlags;
            configureInput(uCurrentFlags);
        }
    }
}

void X11Manager::configureInput(uint16 uFlags) {
    long int iXInputFlags = ExposureMask | StructureNotifyMask;
    iXInputFlags |= (uFlags & FLAG_KEY_DOWN ? KeyPressMask      : 0);
    iXInputFlags |= (uFlags & FLAG_KEY_UP   ? KeyReleaseMask    : 0);
    iXInputFlags |= (uFlags & FLAG_MOVE     ? PointerMotionMask : 0);
    iXInputFlags |= (uFlags & FLAG_PRESS    ? ButtonPressMask   : 0);
    iXInputFlags |= (uFlags & FLAG_RELEASE  ? ButtonReleaseMask : 0);
    ::XSelectInput(oDisplay.get(), uWindowID, iXInputFlags);
}

void X11Manager::handleEvent() {
    switch (oEvent.type) {
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

Manager* createManager(uint16 uWidth, uint16 uHeight, PixelFormat uFormat, uint16 uFlags) {
    return new X11Manager(uWidth, uHeight, uFormat, uFlags);
}

} // namespace

