#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_X11_DEVICE_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_X11_DEVICE_HPP

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

#include "raii.hpp"

namespace MC64K::StandardTestHost::Display::x11 {

/**
 * X11 Implementation of the Device interface.
 */
class Device : public Display::Device {
    private:
        X11Context    oContext;
        ::XEvent      oEvent;
        DisplayHandle oDisplay;
        XImageHandle  oImage;
        ::Window      uWindowID;
        ::Pixmap      uPixmapID;
        GC            pGC;
        unsigned int  uWidth, uHeight;
;
    public:
        /**
         * Constructor. Follows RAII principle.
         *
         * @param  Display::OpenParams const& roOpenParams
         * @throws Error
         * @throws std::bad_alloc
         */
        Device(Display::OpenParams const& roOpenParams);
        virtual ~Device();

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
};

} // namespace

#endif
