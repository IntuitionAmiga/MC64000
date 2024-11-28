#ifndef MC64K_SYNTH_SIGNAL_INSERT_HPP
    #define MC64K_SYNTH_SIGNAL_INSERT_HPP

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

#include "stream.hpp"
#include "operator/single_in_single_out.hpp"

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;


/**
 * IInsert
 *
 * Interface for stream insertion classes. These consume a stream and apply some form of insertion effect.
 */
class IInsert : public Operator::SingleInSingleOut {

    protected:
        float32        fDryLevel;

    public:
        /**
         * Obtain the InputStream for this Insert. Returns null if not set yet.
         *
         * @return IStream|null
         */
        IStream* getInputStream() const noexcept {
            return poInputStream;
        };

        /**
         * Set the InputStream for this Insert. Until an input stream is set, the insert is disable()d.
         *
         */
        IInsert* setInputStream(IStream::Ptr const& roInputStreamPtr) noexcept {
            Operator::SingleInSingleOut::setInputStream(roInputStreamPtr);
            return this;
        }

        IInsert* setInputStream(IStream& roInputStream) noexcept {
            Operator::SingleInSingleOut::setInputStream(roInputStream);
            return this;
        }

        /**
         * Get the dry signal level for this Insert.
         *
         * @return float32
         */
        virtual float32 getDryLevel() const = 0;

        /**
         * Set the dry signal level for this Insert. Should be in the range 0.0 ... 1.0 but this is not a strict requirement.
         * Values over 1 serve as amplification, values lower than one attenuation. Negative values invert the sign.
         *
         * @param  float32 fDryLevel
         * @return this
         */
        virtual IInsert* setDryLevel(float32 fDryLevel);
};

}
#endif
