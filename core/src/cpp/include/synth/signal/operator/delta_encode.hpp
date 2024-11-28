#ifndef MC64K_SYNTH_SIGNAL_DELTAENCODE_HPP
    #define MC64K_SYNTH_SIGNAL_DELTAENCODE_HPP

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
#include "single_in_single_out.hpp"

namespace MC64K::Synth::Audio::Signal::Operator {

/**
 * Performs the differential of the input.
 */
class DeltaEncode : public SingleInSingleOut, public TOutputStream {
    private:
        float32 fPrevious;

    public:
        DeltaEncode(IStream& roSourceInput):
            SingleInSingleOut{roSourceInput},
            TOutputStream{},
            fPrevious{0.0f}
            {}

        DeltaEncode(IStream::Ptr const& roSourceInputPtr):
            SingleInSingleOut{roSourceInputPtr},
            TOutputStream{},
            fPrevious{0.0f}
            {}

        ~DeltaEncode() {}

        /**
         * @inheritDoc
         */
        DeltaEncode* reset() noexcept override {
            SingleInSingleOut::reset();
            oOutputPacketPtr->clear();
            fPrevious = 0.0f;
            return this;
        }

        DeltaEncode* enable() noexcept override {
            SingleInSingleOut::enable();
            return this;
        }

        DeltaEncode* setInputStream(IStream& roNewSource) noexcept override {
            SingleInSingleOut::setInputStream(roNewSource);
            fPrevious = 0.0f;
            return this;
        }

        DeltaEncode* setInputStream(IStream::Ptr const& roNewSourcePtr) noexcept override {
            SingleInSingleOut::setInputStream(roNewSourcePtr);
            fPrevious = 0.0f;
            return this;
        }


    protected:
        /**
         * Generate a new packet. This is called by emit() when it is determined that
         * the packet we are being asked for is not the one we last calculated.
         *
         * @return Packet::ConstPtr
         */
        Packet::ConstPtr emitNew() noexcept;
};

} // namespace

#endif
