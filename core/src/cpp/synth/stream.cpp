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
#include <cstdio>
#include <synth/signal.hpp>

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace MC64K::Synth::Audio::Signal {

IStream::Ptr IStream::NONE{nullptr};

/**
 * @inheritDoc
 */
bool TStreamCommon::canEnable() const noexcept {
    return true;
}

/**
 * @inheritDoc
 */
IStream* TStreamCommon::enable() noexcept {
    if (!bEnabled) {
        if (canEnable()) {
            bEnabled = true;
            std::fprintf(stderr, "\tEnabled stream %p\n", this);
        } else {
            std::fprintf(stderr, "\tCould not enable stream %p\n", this);
        }
    } else {
        std::fprintf(stderr, "\tStream %p already enabled\n", this);
    }
    return this;
};

/**
 * @inheritDoc
 */
IStream* TStreamCommon::disable() noexcept {
    if (bEnabled) {
        bEnabled = false;
        //std::fprintf(stderr, "\tDisabled stream %p\n", this);
    } else {
        //std::fprintf(stderr, "\tStream %p already disabled\n", this);
    }
    return this;
}

} // namespace


