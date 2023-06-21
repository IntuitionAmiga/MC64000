#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_CONTEXT_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_CONTEXT_HPP

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

#include <host/standard_test_host_display.hpp>

namespace MC64K::StandardTestHost::Display {

/**
 * PixelPointer. Simple union to avoid casts.
 */
union PixelPointer {
    void*   puAny;
    uint8*  puByte;
    uint16* puWord;
    uint32* puLong;

    template<typename T>
    inline T* as() const {
        static_assert(std::is_integral<T>::value, "Invalid template type for pixel access");
        return (T*)puAny;
    }

    PixelPointer(): puAny(nullptr) {}
};

class Device;

class Error {

};

enum {
    WIDTH_ALIGN      = 32,
    WIDTH_ALIGN_MASK = WIDTH_ALIGN - 1
};

enum FilthCommand {
    FC_END             = 0x00, // End of script
    FC_WAIT            = 0x01, // Wait until beam position

    // Palette commands
    FC_SET_PALETTE     = 0x02, // Set a full RGB value for a palette entry
    FC_ADD_PALETTE_RGB = 0x03, // Adds an RGB value to a palette entry
    FC_SUB_PALETTE_RGB = 0x04, // Subtracts an RGB value from a palette entry

    FC_SET_PALETTE_R   = 0x05,
    FC_ADD_PALETTE_R   = 0x06, // Increments the red component of a palette entry
    FC_SUB_PALETTE_R   = 0x07, // Decrements the red component of a palette entry

    FC_SET_PALETTE_G   = 0x08,
    FC_ADD_PALETTE_G   = 0x09, // Increments the green component of a palette entry
    FC_SUB_PALETTE_G   = 0x0A, // Decrements the green component of a palette entry

    FC_SET_PALETTE_B   = 0x0B,
    FC_ADD_PALETTE_B   = 0x0C, // Increments the blue component of a palette entry
    FC_SUB_PALETTE_B   = 0x0D, // Decrements the blue component of a palette entry

    FC_SWP_PALETTE     = 0x0E, // Swaps a pair of palette entries

    // View offset commands
    FC_SET_VIEW_X      = 0x0F, // Sets the View X offset
    FC_ADD_VIEW_X      = 0x10, // Increases the View X offset
    FC_SUB_VIEW_X      = 0x11, // Decreases the View X offset
    FC_SET_VIEW_Y      = 0x12, // Sets the View Y offset
    FC_ADD_VIEW_Y      = 0x13, // Increases the View Y offset
    FC_SUB_VIEW_Y      = 0x14, // Decreases the View Y offset

    // Really, really dirty. These allow self-modification
    FC_SET_BYTE        = 0x15, // Replace script byte at offset
    FC_SET_WORD        = 0x16, // Replace script word at offset
    FC_SET_LONG        = 0x17, // Replace script long at offset
    FC_ADD_BYTE        = 0x18, // Increment script byte at offset
    FC_ADD_WORD        = 0x19, // Increment script word at offset
    FC_ADD_LONG        = 0x1A, // Increment script long at offset
    FC_SUB_BYTE        = 0x1B, // Decrement script byte at offset
    FC_SUB_WORD        = 0x1C, // Decrement script word at offset
    FC_SUB_LONG        = 0x1D, // Decrement script long at offset

};

/**
 * Context Handle
 *
 * This is the structure that is returned to the MC64 code on successful opening of a display.
 */
struct Context {
    /**
     * VM frame entry callback. Populated by the VM
     */
    Interpreter::VMCodeEntryPoint apVMCall[CALL_MAX];

    /**
     * Address of direct accessible pixel buffer
     */
    PixelPointer oDisplayBuffer;
    PixelPointer oPaletteData;
    uint32       uNumBufferPixels;
    uint32       uNumBufferBytes;
    uint16       uBufferWidth;
    uint16       uBufferHeight;

    uint32       uNumViewPixels;
    uint16       uViewWidth;
    uint16       uViewHeight;
    uint16       uViewXOffset;
    uint16       uViewYOffset;
    uint8*       puFilthScript;

    uint16       uFlags;
    uint8        uPixelFormat;
    uint8        uRateHz;

    /**
     * Read by VM code
     * For keyboard events, contains the enumeration of the key pressed or released.
     * For mouse events, contains the enumeration of the button pressed or released.
     */
    uint16 uEventRawCode;

    /**
     * Read by VM code
     * For all events, contains a mask of the currently active mouse buttons.
     */
    uint16 uEventRawMask;

    /**
     * Read by VM code
     * For all events, contains the current mouse position.
     */
    uint16 uPositionX;
    uint16 uPositionY;

    /**
     * Points to the device of this context.
     */
    Device* poDevice;

};

/**
 * Parameters required for display opening
 */
struct OpenParams {
    uint16 uViewWidth;
    uint16 uViewHeight;
    uint16 uBufferWidth;
    uint16 uBufferHeight;
    uint16 uViewXOffset;
    uint16 uViewYOffset;
    uint16 uFlags;
    uint8  uPixelFormat;
    uint8  uRateHz;
};

/**
 * Root interface for different display device. Successful instantiation of a derived class is expected
 * to create a viable display instance.
 *
 * To close the display, destroy the instance.
 */
class Device {
    public:
        virtual ~Device() {};
        virtual Context* getContext()    = 0;
        virtual void     runEventLoop()  = 0;
        virtual void     updateDisplay() = 0;
};


Device* createDevice(OpenParams const& roOpenParams);

}

#endif
