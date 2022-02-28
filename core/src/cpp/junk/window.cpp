/**
 * Junk prototype
 *
 * Opens a window and runs a simple event loop that includes a timer interrupt.
 *
 * g++ -Ofast -Wall window.cpp -o window -lX11
 */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/select.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {

    XEvent   oEvent;
    timeval  oTimeVal;

    Display* poDisplay;
    Window   hWindow;
    int      iWindowFD;
    fd_set   iInputFD;

    // Get handle to the main display
    poDisplay = XOpenDisplay(nullptr);

    // Create our window on it
    hWindow   = XCreateSimpleWindow(
        poDisplay,
        RootWindow(poDisplay, 0),
        1, 1,
        640, 480,
        0,
        BlackPixel(poDisplay, 0),
        BlackPixel(poDisplay, 0)
    );

    // Set the Window's IDCMP Flags... wait, what?
    XSelectInput(poDisplay, hWindow,
        ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
        ButtonPressMask | ButtonReleaseMask  | StructureNotifyMask
    );
    XStoreName(poDisplay, hWindow, "Press a key to close");

    // Open the window
    XMapWindow(poDisplay, hWindow);
    XFlush(poDisplay);

    // Get the file descriptor associated with the display
    iWindowFD = ConnectionNumber(poDisplay);

    // Event Loop. Use select() to react to both X11 events and timer.
    int iRunning = 1;
    int iFrame   = 0;
    while (iRunning) {
        // Create a File Description Set containing iWindowFD
        FD_ZERO(&iInputFD);
        FD_SET(iWindowFD, &iInputFD);

        oTimeVal.tv_usec = 100000; // tem times a second
        oTimeVal.tv_sec  = 0;

        int iReadyFD = select(iWindowFD + 1, &iInputFD, nullptr, nullptr, &oTimeVal);
        if (iReadyFD > 0) {
            std::printf("Event %d\r", iFrame);
        } else if (iReadyFD == 0) {
            std::printf("Timer %d\r", iFrame);
        } else {
            std::printf("Error %d\r", iFrame);
        }

        // Handle XEvents and flush the input. If a key is pressed, we'll exit the main loop.
        while (XPending(poDisplay)) {
            XNextEvent(poDisplay, &oEvent);
            if (oEvent.type == KeyPress) {
                iRunning = 0;
                break;
            }
        }
        std::fflush(stdout);
        ++iFrame;
    }
    XCloseDisplay(poDisplay);
    std::puts("\nAdios.");
    return 0;
}

