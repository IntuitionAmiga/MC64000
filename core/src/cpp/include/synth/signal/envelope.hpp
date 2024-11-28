#ifndef MC64K_SYNTH_SIGNAL_ENVELOPE_HPP
    #define MC64K_SYNTH_SIGNAL_ENVELOPE_HPP

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

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;

class IEnvelope : public TStreamCommon, public TOutputStream, protected TPacketIndexAware {

    protected:
        float32 fTimeScale        = 1.0f;
        float32 fLevelScale       = 1.0f;
        bool    bParameterChanged = false;

        static constexpr float32 const MIN_TIME_SCALE_DIFF  = 1.e-5f;
        static constexpr float32 const MIN_LEVEL_SCALE_DIFF = 1.e-5f;

        IEnvelope(): TOutputStream{} {
        }

    public:
        static constexpr float32 const MIN_TIME_SCALE = 0.01f;

        /**
         * @inheritDoc
         */
        IEnvelope* reset() noexcept override;

        /**
         * Set a scaling factor for envelope timing. A smaller value results in a faster envelope. Use to simlulate the
         * effects of higher notes decaying faster, etc. This should be set whenever we start a new note.
         *
         * @param  float32 fTimeScale
         * @return this
         */
        IEnvelope* setTimeScale(float32 fTimeScale) noexcept;

        /**
         * Set a scaling factor for envelope levels. A smaller value results in a quieter envelope. Use to simlulate  the
         * effects of higher notes having lower overall energy, or higher velocities having greater, etc. This should be
         * set whenever we start a new note.
         *
         * @param  float32 fLevelScale
         * @return this
         */
        IEnvelope* setLevelScale(float32 fLevelScale) noexcept;

        typedef std::shared_ptr<IEnvelope> Ptr;
        typedef std::shared_ptr<IEnvelope const> ConstPtr;

};

}
#endif
