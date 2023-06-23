#ifndef MC64K_STANDARD_TEST_HOST_DISPLAY_X11_FILTH_SCRIPT_HPP
    #define MC64K_STANDARD_TEST_HOST_DISPLAY_X11_FILTH_SCRIPT_HPP

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
#include <host/display/x11/raii.hpp>

#include "conversion.hpp"

namespace MC64K::StandardTestHost::Display::x11 {

/**
 * Gets the immediate value at the script position
 */
template<typename T>
inline T getImmediate(uint8 const* puAny) {
    return *((T*)puAny);
}

/**
 * Replaces the immediate value indicated by the 16-bit offset value at the current script position.
 * This is dirty, evil and fun.
 */
template<typename T>
inline size_t setImmediate(uint8 const* puCode, uint8* puBase) {
    // Next 16-bit value is the script offst in bytes
    uint16 uOffset = *((uint16*)puCode);

    // Next sizeof(T) value is the word
    *((T*)(puBase + uOffset)) = *((T*)(puCode + sizeof(uint16)));
    return sizeof(T) + sizeof(uint16);
}

/**
 * Adds to the immediate value indicated by the 16-bit offset value at the current script position the
 * immediate value following the offset.
 * This is dirty, evil and fun.
 */
template<typename T>
inline size_t addToImmediate(uint8 const* puCode, uint8* puBase) {
    // Next 16-bit value is the script offst in bytes
    uint16 uOffset = *((uint16*)puCode);

    // Next sizeof(T) value is the word
    *((T*)(puBase + uOffset)) += *((T*)(puCode + sizeof(uint16)));
    return sizeof(T) + sizeof(uint16);
}

/**
 * Subtracts from the immediate value indicated by the 16-bit offset value at the current script position the
 * immediate value following the offset.
 * This is dirty, evil and fun.
 */
template<typename T>
inline size_t subFromImmediate(uint8 const* puCode, uint8* puBase) {
    // Next 16-bit value is the script offst in bytes
    uint16 uOffset = *((uint16*)puCode);

    // Next sizeof(T) value is the word
    *((T*)(puBase + uOffset)) -= *((T*)(puCode + sizeof(uint16)));
    return sizeof(T) + sizeof(uint16);
}

/**
 * Updates the visible portion of an 8-bit surface, using the FILTH script to modify palette,
 * offsets, etc. according to the beam location.
 */
template<typename Conversion>
void* updatePalettedScripted(Context& roContext) {

    if (typename Conversion::Pixel* puPalette = roContext.oPaletteData.as<typename Conversion::Pixel>()) {
        typename Conversion::Pixel* pDst      = (typename Conversion::Pixel*)roContext.puImageBuffer;
        uint8* pSrc         = roContext.oDisplayBuffer.puByte;
        uint8* puCode       = roContext.puFilthScript;
        uint16 uViewXOffset = roContext.uViewXOffset;
        uint16 uViewYOffset = roContext.uViewYOffset;

        Conversion oConversion; // this is inlined

        for (uint32 yDst = 0; yDst < roContext.uViewHeight; ++yDst) {
            for (uint32 xDst = 0; xDst < roContext.uViewWidth; ++xDst) {

                // Determine beam position as 32-bit YYYY:XXXX
                uint32 uBeamPos = yDst << 16 | xDst;

                // Do FILTH. Each operation happens at a defined beam position
                if (getImmediate<uint32>(puCode) == uBeamPos) {
                    puCode += sizeof(uint32);
                    while (uint8 uCommand = *puCode++) {
                        switch (uCommand) {
                            case FC_END: // technically unreachable
                                goto filth_end;
                                break;

                            case FC_WAIT:
                                goto filth_end;
                                break; // Assume puCode now points at next beam position

                            #define  FILTH_COMMAND_PALLETE
                            #include "commands/palette.hpp"

                            #define  FILTH_COMMAND_VIEW
                            #include "commands/view.hpp"

                            #define  FILTH_COMMAND_SELFMOD
                            #include "commands/selfmod.hpp"

                            default:
                                break;
                        }
                    }
                }
                filth_end:
                // Calculate buffer location
                // Yucky modulo to get the coordinates back into a sensible place.
                uint32 xSrc = xDst + uViewXOffset;
                if (xSrc > roContext.uBufferWidth) {
                    xSrc %= roContext.uBufferWidth;
                }
                uint32 ySrc = yDst + uViewYOffset;
                if (ySrc > roContext.uBufferHeight) {
                    ySrc %= roContext.uBufferHeight;
                }
                *pDst++ = oConversion.convert(puPalette, pSrc[ySrc * roContext.uBufferWidth + xSrc]);
            }
        }
        roContext.uViewXOffset = uViewXOffset;
        roContext.uViewYOffset = uViewYOffset;
    }
    return roContext.puImageBuffer;
}


/**
 * Updates the visible portion of a 32-bit surface, using the FILTH script to modify palette,
 * offsets, etc. according to the beam location.
 */
template<typename Format>
void* updateRGBScripted(Context& roContext) {
    typename Format::Pixel*       pDst = (typename Format::Pixel*)roContext.puImageBuffer;
    typename Format::Pixel const* pSrc = roContext.oDisplayBuffer.as<typename Format::Pixel const>();
    uint8* puCode       = roContext.puFilthScript;
    uint16 uViewXOffset = roContext.uViewXOffset;
    uint16 uViewYOffset = roContext.uViewYOffset;

    for (uint32 yDst = 0; yDst < roContext.uViewHeight; ++yDst) {
        for (uint32 xDst = 0; xDst < roContext.uViewWidth; ++xDst) {

            // Determine beam position
            uint32 uBeamPos = yDst << 16 | xDst;

            // Do filth
            if (getImmediate<uint32>(puCode) == uBeamPos) {
                puCode += sizeof(uint32);
                while (uint8 uCommand = *puCode++) {
                    switch (uCommand) {
                        case FC_END: // technically unreachable
                            goto filth_end;
                            break;

                        case FC_WAIT:
                            goto filth_end;
                            break; // Assume puCode now points at next beam position

                            // Palette commands not recognised in RGB mode. They may be
                            // misinterpreted as other data if present.

                            #define  FILTH_COMMAND_VIEW
                            #include "commands/view.hpp"

                            #define  FILTH_COMMAND_SELFMOD
                            #include "commands/selfmod.hpp"

                        default:
                            break;
                    }
                }
            }
            filth_end:
            // Calculate buffer location
            // Yucky modulo to get the coordinates back into a sensible place.
            uint32 xSrc = xDst + uViewXOffset;
            if (xSrc > roContext.uBufferWidth) {
                xSrc %= roContext.uBufferWidth;
            }
            uint32 ySrc = yDst + uViewYOffset;
            if (ySrc > roContext.uBufferHeight) {
                ySrc %= roContext.uBufferHeight;
            }
            *pDst++ = pSrc[ySrc * roContext.uBufferWidth + xSrc];
        }
    }
    roContext.uViewXOffset = uViewXOffset;
    roContext.uViewYOffset = uViewYOffset;
    return roContext.puImageBuffer;
}

}

#endif
