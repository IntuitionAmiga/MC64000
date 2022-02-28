/**
 * Junk prototype
 *
 * Opens a window and runs a simple event loop that includes a timer interrupt.
 *
 * g++ -Ofast -Wall -Wextra window.cpp -o window -lX11
 */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <sys/select.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main() {

    XEvent   oEvent;
    fd_set   oInputFD;
    timeval  oTimeVal;

    Display* poDisplay;
    Window   hWindow;
    int      iWindowFD;


    // Get handle to the main display
    poDisplay = XOpenDisplay(nullptr);

    if (Screen* poScreen = XDefaultScreenOfDisplay(poDisplay)) {

        std::printf(
            "Found main display @ %p, Screen %p %d x %d\n",
            poDisplay,
            poScreen,
            poScreen->width,
            poScreen->height
        );

    }
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
    int iEvents  = 0;
    while (iRunning) {
        // Create a File Description Set containing iWindowFD
        FD_ZERO(&oInputFD);
        FD_SET(iWindowFD, &oInputFD);

        oTimeVal.tv_usec = 100000; // ten times a second
        oTimeVal.tv_sec  = 0;

        int iReadyFD = select(iWindowFD + 1, &oInputFD, nullptr, nullptr, &oTimeVal);
        if (iReadyFD > 0) {
            std::printf("Event %d\r", iEvents);
        } else if (iReadyFD == 0) {
            ++iEvents;
            std::printf("Timer %d\r", iEvents);
        } else {
            std::printf("Error %d\r", iEvents);
        }

        // Handle XEvents and flush the input. If a key is pressed, we'll exit the main loop.
        while (XPending(poDisplay)) {
            XNextEvent(poDisplay, &oEvent);
            ++iEvents;
            if (oEvent.type == KeyPress) {
                iRunning = 0;
                break;
            }
        }
        std::fflush(stdout);
    }
    XCloseDisplay(poDisplay);
    std::puts("\nAdios.");
    return 0;
}

