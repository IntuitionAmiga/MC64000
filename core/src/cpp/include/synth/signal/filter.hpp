#ifndef MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_FILTER_HPP
    #define MC64K_STANDARD_TEST_HOST_SYNTH_SIGNAL_FILTER_HPP

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

class IFilter : public IStream {

    public:
        // Cutoff range is normalised
        static constexpr float32 const MIN_CUTOFF    = 0.001f;
        static constexpr float32 const DEF_CUTOFF    = 0.5f;
        static constexpr float32 const MAX_CUTOFF    = 1.0f;

        // Resonance range is normalised
        static constexpr float32 const MIN_RESONANCE = 0.0f;
        static constexpr float32 const DEF_RESONANCE = 0.0f;
        static constexpr float32 const MAX_RESONANCE = 1.0f;

        /**
         * Set the baseline cutoff level. In the absence of a cutoff controller, this is the fixed cutoff. Otherwise it is
         * the cutoff value when the control signal level is 1.0. Values sould be in the range MIN_CUTOFF to MAX_CUTOFF.
         * Note that values above MAX_CUTOFF may be tolerated depending on the filter type.
         *
         * @param  float32 fCutoff
         * @return this
         */
        virtual IFilter* setCutoff(float32 fCutoff) = 0;

        /**
         * Set a control stream (envelope, LFO etc) for the cutoff control. Setting null clears any existing control.
         *
         * @param  IStream|null oCutoffControl
         * @return this
         */
        virtual IFilter* setCutoffControl(IStream* oCutoffControl) = 0;

        /**
         * Set the baseline resonance level. In the absence of a resonance controller, this is the fixed resonance.
         * Otherwise it is the resonance value when the control signal level is 1.0. Values should be in the range
         * MIN_RESONANCE to MAX_RESONANCE. Note that values above MAX_RESONANCe may be tolerated depending on the filter
         * type.
         *
         * @param  float fResonance
         * @return this
         */
        virtual IFilter* setResonance(float32 fResonance) = 0;

        /**
         * Set a control stream (envelope, LFO etc) for the resonance control. Setting null clears any existing control.
         *
         * @param  IStream|null oResonanceControl
         * @return this
         */
        virtual IFilter* setResonanceControl(IStream* oResonanceControl) = 0;
};

}
#endif
