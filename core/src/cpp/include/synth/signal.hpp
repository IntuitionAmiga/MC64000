#ifndef MC64K_SYNTH_SIGNAL_HPP
    #define MC64K_SYNTH_SIGNAL_HPP

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

#include <memory>
#include <misc/scalar.hpp>
#include <host/audio/config.hpp>

#include "signal/packet.hpp"
#include "signal/stream.hpp"
#include "signal/envelope.hpp"
#include "signal/waveform.hpp"
#include "signal/oscillator.hpp"
#include "signal/filter.hpp"
#include "signal/insert.hpp"

namespace MC64K::Synth::Audio::Signal {

}

#endif
