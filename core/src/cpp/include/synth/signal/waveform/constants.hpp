#ifndef MC64K_SYNTH_SIGNAL_WAVE_CONSTANTS_HPP
    #define MC64K_SYNTH_SIGNAL_WAVE_CONSTANTS_HPP

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

#include <cmath>
#include <synth/signal.hpp>

namespace MC64K::Synth::Audio::Signal::Waveform {

// 32-bit integer representation of commonly used float values
constexpr int32 const ONE_IEEE_32 = 0x3F800000;
constexpr int32 const TWO_IEEE_32 = 0x40000000;
constexpr int32 const PI_IEEE_32  = 0x40490FDB;

constexpr float32 const HALF   = 0.5f;
constexpr float32 const ONE    = 1.0f;
constexpr float32 const TWO    = 2.0f;
constexpr float32 const TWO_PI = (float32)(2.0 * M_PI);

}

#endif
