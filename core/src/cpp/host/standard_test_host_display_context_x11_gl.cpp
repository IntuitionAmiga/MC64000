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
#include <host/display/glx/device.hpp>

#include <GL/gl.h>
#include <GL/glx.h>

#include "standard_test_host_display_context_x11_common.cpp"

using MC64K::Machine::Interpreter;
using MC64K::Machine::Nanoseconds;

namespace MC64K::StandardTestHost::Display {

namespace xGL {

inline void Device::updateMousePosition() {
    oContext.uEventRawMask = (uint16) event<::XMotionEvent>().state;
    oContext.uPositionX    = (uint16) ((float32)event<::XMotionEvent>().x * fMouseXScale);
    oContext.uPositionY    = (uint16) ((float32)event<::XMotionEvent>().y * fMouseYScale);
}

// Maps PXL_x to glTexImage2D() internalformat parameter
GLint const aInternalFormat[] = {
    GL_RGBA,
    GL_RGB5,
    GL_RGB5,
    GL_RGBA,
};

// Maps PXL_x to glTexImage2D() format parameter
GLenum const aFormat[] = {
    GL_BGRA,
    GL_BGRA,
    GL_BGRA,
    GL_BGRA,
};

// Maps PXL_x to glTexImage2D() type parameter
GLenum const aType[] = {
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_BYTE,
};

char const* aFormatDescriptors[] = {
    "8 BPP, ARGB32 Palette",
    "8 BPP, HAM, RGB555 Palette",
    "16 BPP, RGB555",
    "32 BPP, ARGB32",
};

/**
 * Constructor. We use RAII here and throw exceptions if a requirement can't be met.
 */
Device::Device(Display::OpenParams const& roOpenParams):
    oContext(),
    oEvent(),
    oDisplay(),
    pVisualInfo(nullptr),
    pGLXContext(nullptr),
    uWindowID(0),
    uTextureID(0)
{
    ::Display* poDisplay = oDisplay.get();
    std::fprintf(stderr, "xGL::Device: Found Display at %p\n", poDisplay);

    aGLAttributes[0] = GLX_RGBA;
    aGLAttributes[1] = GLX_DEPTH_SIZE;
    aGLAttributes[2] = 24;
    aGLAttributes[3] = GLX_DOUBLEBUFFER;
    aGLAttributes[4] = None;

    if (!(pVisualInfo = ::glXChooseVisual(poDisplay, 0, aGLAttributes))) {
        std::fprintf(stderr, "Failed to allocate Visual\n");
        throw Error();
    }

    std::fprintf(stderr, "xGL::Device: Visual at %p\n", pVisualInfo);

    if (!(pGLXContext = ::glXCreateContext(poDisplay, pVisualInfo, nullptr, GL_TRUE))) {
        std::fprintf(stderr, "Failed to allocate GLXContext\n");
        throw Error();
    }
    std::fprintf(stderr, "xGL::Device: GLXContext at %p\n", pGLXContext);

    ::Screen* pScreen = DefaultScreenOfDisplay(poDisplay);

    int iScaleFactor = pScreen->height / roOpenParams.uViewHeight;
    std::fprintf(stderr, "Using Integer Scale Factor: %d\n", iScaleFactor);

    uWindowID = ::XCreateSimpleWindow(
        poDisplay,
        DefaultRootWindow(poDisplay),
        0, 0,
        iScaleFactor * roOpenParams.uViewWidth, iScaleFactor * roOpenParams.uViewHeight,
        0,
        0,
        BlackPixel(poDisplay, 0)
    );


    std::fprintf(stderr, "xGL::Device: Got window handle %u\n", (unsigned)uWindowID);

    oContext.uViewWidth       = roOpenParams.uViewWidth;
    oContext.uViewHeight      = roOpenParams.uViewHeight;
    oContext.uBufferWidth     = roOpenParams.uBufferWidth;
    oContext.uBufferHeight    = roOpenParams.uBufferHeight;
    oContext.uFlags           = roOpenParams.uFlags;
    oContext.uPixelFormat     = roOpenParams.uPixelFormat;
    oContext.uRateHz          = roOpenParams.uRateHz < 1 ? 1 : roOpenParams.uRateHz;
    oContext.poDevice        = this;
    oContext.allocateBuffer();

    std::snprintf(
        sTitleBuffer, sizeof(sTitleBuffer)-1,
        "MC64K (GL) %d x %d [Format:%d, %s] @ %dHz, Scaling: %dx",
        (int)oContext.uViewWidth,
        (int)oContext.uViewHeight,
        (int)oContext.uPixelFormat,
        aFormatDescriptors[oContext.uPixelFormat],
        (int)oContext.uRateHz,
        (int)iScaleFactor
    );

    ::XStoreName(poDisplay, uWindowID, sTitleBuffer);

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
        aInternalFormat[roOpenParams.uPixelFormat], //GL_RGBA,
        roOpenParams.uViewWidth,
        roOpenParams.uViewHeight,
        0,
        aFormat[roOpenParams.uPixelFormat],//GL_BGRA,
        aType[roOpenParams.uPixelFormat],//GL_UNSIGNED_BYTE,
        oContext.puImageBuffer
    );
    ::glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    ::glDisable(GL_DEPTH_TEST);
    ::glEnable(GL_TEXTURE_2D);
    ::glViewport(
        0,
        0,
        iScaleFactor * roOpenParams.uViewWidth,
        iScaleFactor * roOpenParams.uViewHeight
    );

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

    std::fprintf(stderr, "xGL::Device: RAII Complete, we live at: %p\n", this);
}

/**
 * Destructor.
 */
Device::~Device() {
    if (pGLXContext) {
        if (uTextureID) {
            ::glDeleteTextures(1, &uTextureID);
        }
        ::glXMakeCurrent(oDisplay.get(), None, nullptr);
        ::glXDestroyContext(oDisplay.get(), pGLXContext);
        std::fprintf(stderr, "xGL::Device GLXContext destroyed\n");
    }
    std::fprintf(stderr, "X11GLDevice destroyed\n");
}

/**
 * Get the context.
 */
Context* Device::getContext() {
    return &oContext;
}

/**
 * Update the display
 */
void Device::updateDisplay() {
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
void Device::runEventLoop() {

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

            //Nanoseconds::Value uTime = Nanoseconds::mark();
            void* pData = oContext.updateBuffers();


            // HERE GL Texture Update
            ::glTexSubImage2D(
                GL_TEXTURE_2D,
                0, // level
                0, // x pos
                0, // y pos
                oContext.uViewWidth,
                oContext.uViewHeight,
                GL_BGRA,          // format
                GL_UNSIGNED_BYTE, // type
                pData             // data
            );

            //uTime = Nanoseconds::mark() - uTime;

            //std::printf("%lu,%lu\n", uFrames, uTime);
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
long Device::configureInputMask() {
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
void Device::handleEvent() {
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

void Device::invokeVMCallback(Interpreter::VMCodeEntryPoint pBytecode) {
    Interpreter::setProgramCounter(pBytecode);
    Interpreter::gpr<ABI::PTR_REG_0>().pAny = &oContext;
    Interpreter::run();
}

}

Device* createDevice(OpenParams const& roOpenParams) {
    return new xGL::Device(roOpenParams);
}

} // namespace

