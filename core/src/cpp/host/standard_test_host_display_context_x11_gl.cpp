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
#include <host/display/x11/raii.hpp>

#include <GL/gl.h>
#include <GL/glx.h>

using MC64K::Machine::Interpreter;
using MC64K::Machine::Nanoseconds;

namespace MC64K::StandardTestHost::Display {

const uint8 aPixelSize[] = {
    1, 4
};

/**
 * X11GL Implementation of the Manager interface.
 */
class X11GLManager : public Manager {
    private:
        X11Context    oContext;
        ::XEvent      oEvent;
        DisplayHandle oDisplay;
        XVisualInfo*  pVisualInfo;
        GLXContext    pGLXContext;
        ::Window      uWindowID;
        int32         aGLAttributes[5];
        uint32        uTextureID;
        float32       fMouseXScale;
        float32       fMouseYScale;

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
        X11GLManager(uint16 uWidth, uint16 uHeight, uint16 uFlags, uint8 uFormat, uint8 uRateHz);
        virtual ~X11GLManager();

        /**
         * @inheritDoc
         */
        Context* getContext();

        /**
         * @inheritDoc
         */
        void runEventLoop();

        /**
         * @inheritDoc
         */
        void updateDisplay();

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

        /**
         * Updates mouse. Since the GL window is resizable, we have to scale the real coordinates
         * back to virtual.
         */
        void updateMousePosition();
};

inline void X11GLManager::updateMousePosition() {
    oContext.uEventRawMask = (uint16) event<::XMotionEvent>().state;
    oContext.uPositionX    = (uint16) ((float32)event<::XMotionEvent>().x * fMouseXScale);
    oContext.uPositionY    = (uint16) ((float32)event<::XMotionEvent>().y * fMouseYScale);
}

/**
 * Constructor. We use RAII here and throw exceptions if a requirement can't be met.
 */
X11GLManager::X11GLManager(uint16 uWidth, uint16 uHeight, uint16 uFlags, uint8 uFormat, uint8 uRateHz):
    oContext(),
    oEvent(),
    oDisplay(),
    pVisualInfo(nullptr),
    pGLXContext(nullptr),
    uWindowID(0),
    uTextureID(0)
{
    ::Display* poDisplay = oDisplay.get();
    std::fprintf(stderr, "X11GLManager: Found Display at %p\n", poDisplay);

    aGLAttributes[0] = GLX_RGBA;
    aGLAttributes[1] = GLX_DEPTH_SIZE;
    aGLAttributes[2] = 24;
    aGLAttributes[3] = GLX_DOUBLEBUFFER;
    aGLAttributes[4] = None;

    if (!(pVisualInfo = ::glXChooseVisual(poDisplay, 0, aGLAttributes))) {
        std::fprintf(stderr, "Failed to allocate Visual\n");
        throw Error();
    }

    std::fprintf(stderr, "X11GLManager: Visual at %p\n", pVisualInfo);

    if (!(pGLXContext = ::glXCreateContext(poDisplay, pVisualInfo, nullptr, GL_TRUE))) {
        std::fprintf(stderr, "Failed to allocate GLXContext\n");
        throw Error();
    }
    std::fprintf(stderr, "X11GLManager: GLXContext at %p\n", pGLXContext);

    ::Screen* pScreen = DefaultScreenOfDisplay(poDisplay);

    int iScaleFactor = pScreen->height / uHeight;
    std::fprintf(stderr, "Using Integer Scale Factor: %d\n", iScaleFactor);

    uWindowID = ::XCreateSimpleWindow(
        poDisplay,
        DefaultRootWindow(poDisplay),
        0, 0,
        iScaleFactor * uWidth, iScaleFactor * uHeight,
        0,
        0,
        BlackPixel(poDisplay, 0)
    );


    std::fprintf(stderr, "X11GLManager: Got window handle %u\n", (unsigned)uWindowID);

    ::XStoreName(poDisplay, uWindowID, "MC64K (GL)");

    oContext.uNumBufferPixels = uWidth * uHeight;
    oContext.uNumBufferBytes  = oContext.uNumBufferPixels * aPixelSize[uFormat];
    oContext.uBufferWidth     = uWidth;
    oContext.uBufferHeight    = uHeight;
    oContext.uFlags           = uFlags;
    oContext.uPixelFormat     = uFormat;
    oContext.uRateHz          = uRateHz < 1 ? 1 : uRateHz;
    oContext.poManager        = this;
    oContext.allocateBuffer();

    ::glXMakeCurrent(poDisplay, uWindowID, pGLXContext);

    // Hackity Hack
    ::glGenTextures(1, &uTextureID);

    std::fprintf(stderr, "Assgined texture ID %u\n", uTextureID);

    ::glBindTexture(GL_TEXTURE_2D, uTextureID);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ::glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        uWidth,
        uHeight,
        0,
        GL_BGRA,
        GL_UNSIGNED_BYTE,
        oContext.puImageBuffer
    );
    ::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    ::glDisable(GL_DEPTH_TEST);
    ::glEnable(GL_TEXTURE_2D);
    ::glViewport(0, 0, iScaleFactor * uWidth, iScaleFactor * uHeight);

    // Open the window
    ::XMapWindow(poDisplay, uWindowID);
    ::XFlush(poDisplay);

    fMouseYScale =
    fMouseXScale = 1.0f / (float32)iScaleFactor;

    // Todo - we should be using newer VBO operations and bypass all of this.
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    updateDisplay();

    std::fprintf(stderr, "X11GLManager: RAII Complete, we live at: %p\n", this);
}

/**
 * Destructor.
 */
X11GLManager::~X11GLManager() {
    if (pGLXContext) {
        if (uTextureID) {
            ::glDeleteTextures(1, &uTextureID);
        }
        ::glXMakeCurrent(oDisplay.get(), None, nullptr);
        ::glXDestroyContext(oDisplay.get(), pGLXContext);
        std::fprintf(stderr, "X11GLManager GLXContext destroyed\n");
    }
    std::fprintf(stderr, "X11GLManager destroyed\n");
}

/**
 * Get the context.
 */
Context* X11GLManager::getContext() {
    return &oContext;
}

/**
 * Update the display
 */
void X11GLManager::updateDisplay() {
    // TODO this should use a VBO
    glBegin (GL_QUADS);
    glVertex3i(-1, 1, -1);  glTexCoord2i(1, 0);
    glVertex3i(1, 1, -1);   glTexCoord2i(1, 1);
    glVertex3i(1, -1, -1);  glTexCoord2i(0, 1);
    glVertex3i(-1, -1, -1); glTexCoord2i(0, 0);
    glEnd();
    glXSwapBuffers(oDisplay.get(), uWindowID);
}

/**
 * Run the event loop.
 */
void X11GLManager::runEventLoop() {

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

        // Check if we need to copy the pixel buffer to the offscreen buffer
        if (oContext.uFlags & (FLAG_DRAW_BUFFER_NEXT_FRAME|FLAG_DRAW_BUFFER_ALL_FRAMES)) {
            oContext.updateBuffers();
            // HERE GL Texture Update
            ::glTexSubImage2D(
                GL_TEXTURE_2D,
                0, // level
                0, // x pos
                0, // y pos
                oContext.uBufferWidth,
                oContext.uBufferHeight,
                GL_BGRA,                   // format
                GL_UNSIGNED_BYTE,          // type
                oContext.puImageBuffer     // data
            );
            oContext.uFlags &= (uint16)~FLAG_DRAW_BUFFER_NEXT_FRAME;
        }

        // Check if we need to flip the offscreen buffer
        if (oContext.uFlags & (FLAG_FLIP_NEXT_FRAME|FLAG_FLIP_ALL_FRAMES)) {
            updateDisplay();
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
long X11GLManager::configureInputMask() {
    long int iXInputMask = ExposureMask;
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
void X11GLManager::handleEvent() {
    switch (oEvent.type) {
        case NoExpose:
            break;
        case Expose:
            XWindowAttributes oWinAttr;
            ::XGetWindowAttributes(oDisplay.get(), uWindowID, &oWinAttr);
            ::glViewport(0, 0, oWinAttr.width, oWinAttr.height);
            fMouseXScale = (float32)oContext.uBufferWidth /  (float32) oWinAttr.width;
            fMouseYScale = (float32)oContext.uBufferHeight / (float32) oWinAttr.height;
            break;
        case KeyPress:
            if (oContext.apVMCall[CALL_KEY_PRESS]) {
                oContext.uEventRawCode = (uint16) event<::XKeyEvent>().keycode;
                updateMousePosition();
                invokeVMCallback(oContext.apVMCall[CALL_KEY_PRESS]);
            }
            break;
        case KeyRelease:
            if (oContext.apVMCall[CALL_KEY_RELEASE]) {
                oContext.uEventRawCode = (uint16) event<::XKeyEvent>().keycode;
                updateMousePosition();
                invokeVMCallback(oContext.apVMCall[CALL_KEY_RELEASE]);
            }
            break;
        case MotionNotify:
            if (oContext.apVMCall[CALL_MOVEMENT]) {
                updateMousePosition();
                invokeVMCallback(oContext.apVMCall[CALL_MOVEMENT]);
            }
            break;
        case ButtonPress:
            if (oContext.apVMCall[CALL_BUTTON_PRESS]) {
                oContext.uEventRawCode = (uint16) event<::XButtonEvent>().button;
                updateMousePosition();
                invokeVMCallback(oContext.apVMCall[CALL_BUTTON_PRESS]);
            }
            break;
        case ButtonRelease:
            if (oContext.apVMCall[CALL_BUTTON_RELEASE]) {
                oContext.uEventRawCode = (uint16) event<::XButtonEvent>().button;
                updateMousePosition();
                invokeVMCallback(oContext.apVMCall[CALL_BUTTON_RELEASE]);
            }
            break;
        default:
            std::printf("Other Event %d\n", oEvent.type);
            break;
    }
}

void X11GLManager::invokeVMCallback(Interpreter::VMCodeEntryPoint pBytecode) {
    Interpreter::setProgramCounter(pBytecode);
    Interpreter::gpr<ABI::PTR_REG_0>().pAny = &oContext;
    Interpreter::run();
}

Manager* createManager(uint16 uWidth, uint16 uHeight, uint16 uFlags, uint8 uFormat, uint8 uRateHz) {
    return new X11GLManager(uWidth, uHeight, uFlags, uFormat, uRateHz);
}

} // namespace

