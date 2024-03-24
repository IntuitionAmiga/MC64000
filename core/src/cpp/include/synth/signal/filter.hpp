#ifndef MC64K_SYNTH_SIGNAL_FILTER_HPP
    #define MC64K_SYNTH_SIGNAL_FILTER_HPP

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

/**
 * IFilter
 */
class IFilter : public TStreamCommon, protected TPacketIndexAware  {

    protected:
        IStream::Ptr   oInputStreamPtr;
        IStream::Ptr   oCutoffModulatorPtr;
        IStream::Ptr   oResonanceModulatorPtr;
        IEnvelope::Ptr oCutoffEnvelopePtr;
        IEnvelope::Ptr oResonanceEnvelopePtr;

        float64 fFixedCutoff;
        float64 fFixedResonance;

        /**
         * @inheritDoc
         *
         * Overridden - filter cannot be enabled if there is no input stream set.
         */
        bool    canEnable() const;

        virtual void configure() = 0;

        IFilter(IStream::Ptr const& roStreamPtr, float32 fCutoff, float32 fResonance):
            oInputStreamPtr{roStreamPtr},
            fFixedCutoff{fCutoff},
            fFixedResonance{fResonance} { }

    public:
        // Cutoff range is normalised
        static constexpr float32 const MIN_CUTOFF    = 0.001f;
        static constexpr float32 const DEF_CUTOFF    = 0.5f;
        static constexpr float32 const MAX_CUTOFF    = 1.0f;

        // Resonance range is normalised
        static constexpr float32 const MIN_RESONANCE = 0.0f;
        static constexpr float32 const DEF_RESONANCE = 0.0f;
        static constexpr float32 const MAX_RESONANCE = 1.0f;

        IFilter* reset() override;

        IFilter* enable() override;

        IFilter* setInputStream(IStream::Ptr const& roInputStreamPtr) {
            oInputStreamPtr = roInputStreamPtr;
            if (!oInputStreamPtr.get()) {
                disable();
            }
            return this;
        }

        /**
         * Set the baseline cutoff level. In the absence of a cutoff controller, this is the fixed cutoff. Otherwise it is
         * the cutoff value when the control signal level is 1.0. Values sould be in the range MIN_CUTOFF to MAX_CUTOFF.
         * Note that values above MAX_CUTOFF may be tolerated depending on the filter type.
         *
         * @param  float32 fCutoff
         * @return this
         */
        IFilter* setCutoff(float32 fCutoff) {
            fFixedCutoff = fCutoff > MIN_CUTOFF ? fCutoff : MIN_CUTOFF;
            configure();
            return this;
        }

        /**
         * @param  IStream::Ptr poCutoffControl
         * @return this
         */
        IFilter* setCutoffModulator(IStream::Ptr const& roNewCutoffModulatorPtr) {
            oCutoffModulatorPtr = roNewCutoffModulatorPtr;
            configure();
            return this;
        }

        /**
         * @param IEnvelope::Ptr
         * return this
         */
        IFilter* setCutoffEnvelope(IEnvelope::Ptr const& roNewEnvelopePtr) {
            oCutoffEnvelopePtr = roNewEnvelopePtr;
            configure();
            return this;
        }

        /**
         * Set the baseline resonance level. In the absence of a resonance controller, this is the fixed resonance.
         * Otherwise it is the resonance value when the control signal level is 1.0. Values should be in the range
         * MIN_RESONANCE to MAX_RESONANCE. Note that values above MAX_RESONANCE may be tolerated depending on the filter
         * type.
         *
         * @param  float fResonance
         * @return this
         */
        IFilter* setResonance(float32 fResonance) {
            fFixedResonance = fResonance > MIN_RESONANCE ? fResonance : MIN_RESONANCE;
            configure();
            return this;
        }

        /**
         * Set a control stream (envelope, LFO etc) for the resonance control. Setting null clears any existing control.
         *
         * @param  IStream::Ptr poResonanceControl
         * @return this
         */
        IFilter* setResonanceModulator(IStream::Ptr const& roNewResonanceModulatorPtr) {
            oResonanceModulatorPtr = roNewResonanceModulatorPtr;
            configure();
            return this;
        }

        /**
         * @param IEnvelope::Ptr
         * return this
         */
        IFilter* setResonanceEnvelope(IEnvelope::Ptr const& roNewEnvelopePtr) {
            oResonanceEnvelopePtr = roNewEnvelopePtr;
            configure();
            return this;
        }
};

}
#endif
