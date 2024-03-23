#ifndef MC64K_SYNTH_MACHINE_VOICE_HPP
    #define MC64K_SYNTH_MACHINE_VOICE_HPP

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

namespace MC64K::Synth::Audio::Machine {

/**
 * Simple enumeration for voice number
 */
enum Voice {
    V0  = 0,
    V1  = 1,
    V2  = 2,
    V3  = 3,
    V4  = 4,
    V5  = 5,
    V6  = 6,
    V7  = 7,
    V8  = 8,
    V9  = 9,
    V10 = 10,
    V11 = 11,
    V12 = 12,
    V13 = 13,
    V14 = 14,
    V15 = 15
};

enum Polyphony {
    MONO           = 1,
    ONE_VOICE      = 1,
    TWO_VOICE      = 2,
    THREE_VOICE    = 3,
    FOUR_VOICE     = 4,
    FIVE_VOICE     = 5,
    SIX_VOICE      = 6,
    SEVEN_VOICE    = 7,
    EIGHT_VOICE    = 8,
    NINE_VOICE     = 9,
    TEN_VOICE      = 10,
    ELEVEN_VOICE   = 11,
    TWELVE_VOICE   = 12,
    THIRTEEN_VOICE = 13,
    FOURTEEN_VOICE = 14,
    FIFTEEN_VOICE  = 15,
    SIXTEEN_VOICE  = 16,
    MAX_VOICES     = 16
};

}

#endif
