#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_HPP

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

#include "standard_test_host.hpp"

namespace MC64K::StandardTestHost::Display {

/**
 * Call
 *
 * Enumeration of calls in the Mem namespace
 */
enum Call {
    INIT = 0,
    DONE,
    OPEN,
    CLOSE,
    BEGIN,
    UPDATE
};

enum PixelFormat {
    // 8-bit pixels
    PXL_LUT_8   = 0, // 8-bit pixel, paletted, ARGB-32 colour format
    PXL_HAM_555 = 1, // 5-bit pixel with 3-bit HAM modifier, RGB 555 colour format

    // 16-bit pixels
	PXL_RGB_555 = 2,  // 16-bit pixel, RGB 555 colourspace

    // 32-bit pixels
    PXL_ARGB_32 = 3,  // 32-bit pixel, ARGB-32 colour format

    PXL_MAX
};

enum Dimension {
    WIDTH_MIN  = 160,
    HEIGHT_MIN = 100,
    WIDTH_MAX  = 16384,
    HEIGHT_MAX = 16384,
};

enum Flags {
    /**
     * When set, this flag signals that the direct writable pixel buffer should be copied to the
     * offscreen buffer on the next frame. This is cleared when the operation happens, making this
     * flag useful for one-off updates.
     */
    FLAG_DRAW_BUFFER_NEXT_FRAME = 0x0001,

    /**
     * When set, this flag signals that the direct writable pixel buffer should be copied to the
     * offscreen buffer on every frame, regardless of the state of FLAG_DRAW_BUFFER_NEXT_FRAME.
     */
    FLAG_DRAW_BUFFER_ALL_FRAMES = 0x0002,

    /**
     * When set, this flag signals that the visible and offscreen buffers should be flipped on the next
     * frame. This is cleared when the operation happens, making this flag useful for one-off updates.
     */
    FLAG_FLIP_NEXT_FRAME = 0x0004,

    /**
     * When set, this flag signals that the visible and offscreen buffers should be flipped on every
     * frame, regardless of the state of FLAG_FLIP_NEXT_FRAME.
     */
    FLAG_FLIP_ALL_FRAMES = 0x0008,


    /**
     * Set when calling the BEGIN host vector. The VM signals that it wishes to exit the host native
     * event loop by clearing this flag.
     */
    FLAG_RUNNING         = 0x8000
};

enum ContextCall {
    CALL_FRAME          = 0,
    CALL_KEY_PRESS      = 1,
    CALL_KEY_RELEASE    = 2,
    CALL_BUTTON_PRESS   = 3,
    CALL_BUTTON_RELEASE = 4,
    CALL_MOVEMENT       = 5,
    CALL_MAX
};

/**
 * Error return values
 */
enum Result {
    ERR_NO_DISPLAY     = 1000,
    ERR_INVALID_FMT    = 1001,
    ERR_INVALID_WIDTH  = 1002,
    ERR_INVALID_HEIGHT = 1003,
    ERR_INVALID_ID     = 1010,
};

Interpreter::Status hostVector(uint8 uFunctionID);

} // namespace

#endif
