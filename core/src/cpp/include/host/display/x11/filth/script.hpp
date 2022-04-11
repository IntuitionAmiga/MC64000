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

namespace MC64K::StandardTestHost::Display::x11 {

template<typename T>
inline size_t setValue(uint8* puCode, uint8* puBase) {
    // Next 16-bit value is the script offst in bytes
    uint16 uOffset = *((uint16*)puCode);

    // Next sizeof(T) value is the word
    *((T*)(puBase + uOffset)) = *((T*)(puCode + sizeof(uint16)));
    return sizeof(T) + sizeof(uint16);
}

template<typename T>
inline size_t addValue(uint8* puCode, uint8* puBase) {
    // Next 16-bit value is the script offst in bytes
    uint16 uOffset = *((uint16*)puCode);

    // Next sizeof(T) value is the word
    *((T*)(puBase + uOffset)) += *((T*)(puCode + sizeof(uint16)));
    return sizeof(T) + sizeof(uint16);
}

template<typename T>
inline size_t subValue(uint8* puCode, uint8* puBase) {
    // Next 16-bit value is the script offst in bytes
    uint16 uOffset = *((uint16*)puCode);

    // Next sizeof(T) value is the word
    *((T*)(puBase + uOffset)) -= *((T*)(puCode + sizeof(uint16)));
    return sizeof(T) + sizeof(uint16);
}

void* updateLUT8Filth(Context& roContext) {
    if (uint32* puPalette = roContext.puPalette) {
        uint32* pDst        = (uint32*)roContext.puImageBuffer;
        uint8* pSrc         = roContext.oDisplayBuffer.puByte;
        uint8* puCode       = roContext.puFilthScript;
        uint16 uViewXOffset = roContext.uViewXOffset;
        uint16 uViewYOffset = roContext.uViewYOffset;

        for (uint32 yDst = 0; yDst < roContext.uViewHeight; ++yDst) {
            for (uint32 xDst = 0; xDst < roContext.uViewWidth; ++xDst) {

                // Determine beam position
                uint32 uBeamPos = yDst << 16 | xDst;

                // Do filth
                if (*(uint32*)puCode == uBeamPos) {
                    puCode += sizeof(uint32);
                    while (uint8 uCommand = *puCode++) {
                        switch (uCommand) {
                            case FC_END: // technically unreachable
                                goto filth_end;
                                break;

                            case FC_WAIT:
                                goto filth_end;
                                break; // Assume puCode now points at next beam position

                            case FC_SET_PALETTE: {
                                uint8 uIndex = *puCode++;
                                puPalette[uIndex] = *(uint32*)puCode;
                                puCode += sizeof(uint32);
                                break;
                            }

                            case FC_ADD_PALETTE_RGB: {
                                // todo - this sucks
                                uint8 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[0] += puCode[0];
                                ((uint8*)(&puPalette[uIndex]))[1] += puCode[1];
                                ((uint8*)(&puPalette[uIndex]))[2] += puCode[2];
                                puCode += sizeof(uint32);
                                break;
                            }

                            case FC_SUB_PALETTE_RGB: {
                                // todo - this sucks
                                uint8 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[0] -= puCode[0];
                                ((uint8*)(&puPalette[uIndex]))[1] -= puCode[1];
                                ((uint8*)(&puPalette[uIndex]))[2] -= puCode[2];
                                puCode += sizeof(uint32);
                                break;
                            }

                            case FC_SET_PALETTE_R: {
                                uint32 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[2] = *puCode++;
                                break;
                            }

                            case FC_ADD_PALETTE_R: {
                                uint32 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[2] += *puCode++;
                                break;
                            }

                            case FC_SUB_PALETTE_R: {
                                uint32 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[2] -= *puCode++;
                                break;
                            }

                            case FC_SET_PALETTE_G: {
                                uint32 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[1] = *puCode++;
                                break;
                            }

                            case FC_ADD_PALETTE_G: {
                                uint32 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[1] += *puCode++;
                                break;
                            }

                            case FC_SUB_PALETTE_G: {
                                uint32 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[1] -= *puCode++;
                                break;
                            }

                            case FC_SET_PALETTE_B: {
                                uint32 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[0] = *puCode++;
                                break;
                            }

                            case FC_ADD_PALETTE_B: {
                                uint32 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[0] += *puCode++;
                                break;
                            }

                            case FC_SUB_PALETTE_B: {
                                uint32 uIndex = *puCode++;
                                ((uint8*)(&puPalette[uIndex]))[0] -= *puCode++;
                                break;
                            }

                            case FC_SWP_PALETTE: {
                                uint32 uIndexA = *puCode++;
                                uint32 uIndexB = *puCode++;
                                uint32 uRGB = puPalette[uIndexA];
                                puPalette[uIndexA] = puPalette[uIndexB];
                                puPalette[uIndexB] = uRGB;
                                break;
                            }

                            case FC_SET_VIEW_X:
                                uViewXOffset = *((uint16*)puCode);// % roContext.uBufferWidth;
                                puCode += sizeof(uint16);
                                break;

                            case FC_ADD_VIEW_X:
                                uViewXOffset += *((uint16*)puCode);// % roContext.uBufferWidth;
                                puCode += sizeof(uint16);
                                break;

                            case FC_SUB_VIEW_X:
                                uViewXOffset -= *((uint16*)puCode);// % roContext.uBufferWidth;
                                puCode += sizeof(uint16);
                                break;

                            case FC_SET_VIEW_Y:
                                uViewYOffset = *((uint16*)puCode);// % roContext.uBufferHeight;
                                puCode += sizeof(uint16);
                                break;

                            case FC_ADD_VIEW_Y:
                                uViewYOffset += *((uint16*)puCode);// % roContext.uBufferWidth;
                                puCode += sizeof(uint16);
                                break;

                            case FC_SUB_VIEW_Y:
                                uViewYOffset -= *((uint16*)puCode);// % roContext.uBufferWidth;
                                puCode += sizeof(uint16);
                                break;

                            case FC_SET_BYTE:
                                puCode += setValue<uint8>(puCode, roContext.puFilthScript);
                                break;

                            case FC_SET_WORD:
                                puCode += setValue<uint16>(puCode, roContext.puFilthScript);
                                break;

                            case FC_SET_LONG:
                                puCode += setValue<uint32>(puCode, roContext.puFilthScript);
                                break;

                            case FC_ADD_BYTE:
                                puCode += addValue<uint8>(puCode, roContext.puFilthScript);
                                break;

                            case FC_ADD_WORD:
                                puCode += addValue<uint16>(puCode, roContext.puFilthScript);
                                break;

                            case FC_ADD_LONG:
                                puCode += addValue<uint32>(puCode, roContext.puFilthScript);
                                break;

                            case FC_SUB_BYTE:
                                puCode += subValue<uint8>(puCode, roContext.puFilthScript);
                                break;

                            case FC_SUB_WORD:
                                puCode += subValue<uint16>(puCode, roContext.puFilthScript);
                                break;

                            case FC_SUB_LONG:
                                puCode += subValue<uint32>(puCode, roContext.puFilthScript);
                                break;

                            default:
                                break;
                        }
                    }
                }
                filth_end:
                // Calculate buffer location
                uint32 xSrc = xDst + uViewXOffset;
                if (xSrc > roContext.uBufferWidth) {
                    xSrc %= roContext.uBufferWidth;
                }
                uint32 ySrc = yDst + uViewYOffset;
                if (ySrc > roContext.uBufferHeight) {
                    ySrc %= roContext.uBufferHeight;
                }
                *pDst++ = puPalette[pSrc[ySrc * roContext.uBufferWidth + xSrc]];
            }
        }
        roContext.uViewXOffset = uViewXOffset;
        roContext.uViewYOffset = uViewYOffset;
    }
    return roContext.puImageBuffer;
}



void* updateARGB32Filth(Context& roContext) {
    uint32* pDst        = (uint32*)roContext.puImageBuffer;
    uint32 const* pSrc  = roContext.oDisplayBuffer.puLong;
    uint8* puCode       = roContext.puFilthScript;
    uint16 uViewXOffset = roContext.uViewXOffset;
    uint16 uViewYOffset = roContext.uViewYOffset;

    for (uint32 yDst = 0; yDst < roContext.uViewHeight; ++yDst) {
        for (uint32 xDst = 0; xDst < roContext.uViewWidth; ++xDst) {

            // Determine beam position
            uint32 uBeamPos = yDst << 16 | xDst;

            // Do filth
            if (*(uint32*)puCode == uBeamPos) {
                puCode += sizeof(uint32);
                while (uint8 uCommand = *puCode++) {
                    switch (uCommand) {
                        case FC_END: // technically unreachable
                            goto filth_end;
                            break;

                        case FC_WAIT:
                            goto filth_end;
                            break; // Assume puCode now points at next beam position

                        case FC_SET_VIEW_X:
                            // Next 16-bit value is the new View X
                            uViewXOffset = *((uint16*)puCode);// % roContext.uBufferWidth;
                            puCode += sizeof(uint16);
                            break;

                        case FC_ADD_VIEW_X:
                            // Next 16-bit value is the new View X
                            uViewXOffset += *((uint16*)puCode);// % roContext.uBufferWidth;
                            puCode += sizeof(uint16);
                            break;

                        case FC_SUB_VIEW_X:
                            // Next 16-bit value is the new View X
                            uViewXOffset -= *((uint16*)puCode);// % roContext.uBufferWidth;
                            puCode += sizeof(uint16);
                            break;

                        case FC_SET_VIEW_Y:
                            // Next 16-bit value is the new View Y
                            uViewYOffset = *((uint16*)puCode);// % roContext.uBufferHeight;
                            puCode += sizeof(uint16);
                            break;

                        case FC_ADD_VIEW_Y:
                            // Next 16-bit value is the new View Y
                            uViewYOffset += *((uint16*)puCode);// % roContext.uBufferWidth;
                            puCode += sizeof(uint16);
                            break;

                        case FC_SUB_VIEW_Y:
                            // Next 16-bit value is the new View Y
                            uViewYOffset -= *((uint16*)puCode);// % roContext.uBufferWidth;
                            puCode += sizeof(uint16);
                            break;

                        case FC_SET_BYTE:
                            puCode += setValue<uint8>(puCode, roContext.puFilthScript);
                            break;

                        case FC_SET_WORD:
                            puCode += setValue<uint16>(puCode, roContext.puFilthScript);
                            break;

                        case FC_SET_LONG:
                            puCode += setValue<uint32>(puCode, roContext.puFilthScript);
                            break;

                        case FC_ADD_BYTE:
                            puCode += addValue<uint8>(puCode, roContext.puFilthScript);
                            break;

                        case FC_ADD_WORD:
                            puCode += addValue<uint16>(puCode, roContext.puFilthScript);
                            break;

                        case FC_ADD_LONG:
                            puCode += addValue<uint32>(puCode, roContext.puFilthScript);
                            break;

                        case FC_SUB_BYTE:
                            puCode += subValue<uint8>(puCode, roContext.puFilthScript);
                            break;

                        case FC_SUB_WORD:
                            puCode += subValue<uint16>(puCode, roContext.puFilthScript);
                            break;

                        case FC_SUB_LONG:
                            puCode += subValue<uint32>(puCode, roContext.puFilthScript);
                            break;

                        default:
                            break;
                    }
                }
            }
            filth_end:
            // Calculate buffer location
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
