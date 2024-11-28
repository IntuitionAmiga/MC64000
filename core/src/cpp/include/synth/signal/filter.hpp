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
#include "operator/single_in_single_out.hpp"

namespace MC64K::Synth::Audio::Signal {
using namespace MC64K::StandardTestHost::Audio::IConfig;

/**
 * IFilter
 */
class IFilter : public Operator::SingleInSingleOut, public TOutputStream  {

    protected:
        IStream::Ptr   oCutoffModulatorPtr;
        IStream::Ptr   oResonanceModulatorPtr;
        IEnvelope::Ptr oCutoffEnvelopePtr;
        IEnvelope::Ptr oResonanceEnvelopePtr;

        IStream*   poCutoffModulator;
        IStream*   poResonanceModulator;
        IEnvelope* poCutoffEnvelope;
        IEnvelope* poResonanceEnvelope;

        float64 fFixedCutoff;
        float64 fFixedResonance;

        /**
         * @inheritDoc
         *
         * Overridden - filter cannot be enabled if there is no input stream set.
         */
        bool    canEnable() const noexcept override;

        virtual void configure() = 0;

        IFilter(IStream& roStream, float32 fCutoff, float32 fResonance):
            Operator::SingleInSingleOut{roStream},
            TOutputStream{},
            poCutoffModulator{nullptr},
            poResonanceModulator{nullptr},
            poCutoffEnvelope{nullptr},
            poResonanceEnvelope{nullptr},
            fFixedCutoff{fCutoff},
            fFixedResonance{fResonance} {

            }


        IFilter(IStream::Ptr const& roStreamPtr, float32 fCutoff, float32 fResonance):
            Operator::SingleInSingleOut{roStreamPtr},
            TOutputStream{},
            poCutoffModulator{nullptr},
            poResonanceModulator{nullptr},
            poCutoffEnvelope{nullptr},
            poResonanceEnvelope{nullptr},
            fFixedCutoff{fCutoff},
            fFixedResonance{fResonance} {

            }

    public:
        // Cutoff range is normalised
        static constexpr float32 const MIN_CUTOFF    = 0.001f;
        static constexpr float32 const DEF_CUTOFF    = 0.5f;
        static constexpr float32 const MAX_CUTOFF    = 1.0f;

        // Resonance range is normalised
        static constexpr float32 const MIN_RESONANCE = 0.0f;
        static constexpr float32 const DEF_RESONANCE = 0.0f;
        static constexpr float32 const MAX_RESONANCE = 1.0f;

        IFilter* reset() noexcept override;

        IFilter* enable() noexcept override;

        IFilter* setInputStream(IStream& roInputStream) noexcept override {
            Operator::SingleInSingleOut::setInputStream(roInputStream);
            return this;
        }

        IFilter* setInputStream(IStream::Ptr const& roInputStreamPtr) noexcept override {
            Operator::SingleInSingleOut::setInputStream(roInputStreamPtr);
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
        IFilter* setCutoff(float32 fCutoff) noexcept {
            fFixedCutoff = fCutoff > MIN_CUTOFF ? fCutoff : MIN_CUTOFF;
            configure();
            return this;
        }

        /**
         * @param  IStream& roCutoffControl
         * @return this
         *
         * Hardwired version
         */
        IFilter* setCutoffModulator(IStream& roNewCutoffModulator) noexcept {
            poCutoffModulator   = &roNewCutoffModulator;
            configure();
            return this;
        }

        /**
         * @param  IStream::Ptr poCutoffControl
         * @return this
         */
        IFilter* setCutoffModulator(IStream::Ptr const& roNewCutoffModulatorPtr) noexcept {
            oCutoffModulatorPtr = roNewCutoffModulatorPtr;
            poCutoffModulator   = roNewCutoffModulatorPtr.get();
            configure();
            return this;
        }

        /**
         * @param IEnvelope& roNewEnvelope
         * return this
         *
         * Hardwired version
         */
        IFilter* setCutoffEnvelope(IEnvelope& roNewEnvelope) noexcept {
            poCutoffEnvelope = &roNewEnvelope;
            configure();
            return this;
        }

        /**
         * @param IEnvelope::Ptr
         * return this
         */
        IFilter* setCutoffEnvelope(IEnvelope::Ptr const& roNewEnvelopePtr) noexcept {
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
        IFilter* setResonance(float32 fResonance) noexcept {
            fFixedResonance = fResonance > MIN_RESONANCE ? fResonance : MIN_RESONANCE;
            configure();
            return this;
        }

        /**
         * Set a control stream (envelope, LFO etc) for the resonance control. Setting null clears any existing control.
         *
         * @param  IStream& roResonanceControl
         * @return this
         *
         * Hardwired version.
         */
        IFilter* setResonanceModulator(IStream& roNewResonanceModulator) noexcept {
            poResonanceModulator = &roNewResonanceModulator;
            configure();
            return this;
        }

        /**
         * Set a control stream (envelope, LFO etc) for the resonance control. Setting null clears any existing control.
         *
         * @param  IStream::Ptr poResonanceControl
         * @return this
         */
        IFilter* setResonanceModulator(IStream::Ptr const& roNewResonanceModulatorPtr) noexcept {
            oResonanceModulatorPtr = roNewResonanceModulatorPtr;
            configure();
            return this;
        }

        /**
         * @param IEnvelope::Ptr
         * return this
         */
        IFilter* setResonanceEnvelope(IEnvelope& roNewEnvelope) noexcept {
            poResonanceEnvelope = &roNewEnvelope;
            configure();
            return this;
        }

        /**
         * @param IEnvelope::Ptr
         * return this
         */
        IFilter* setResonanceEnvelope(IEnvelope::Ptr const& roNewEnvelopePtr) noexcept {
            oResonanceEnvelopePtr = roNewEnvelopePtr;
            configure();
            return this;
        }
};

}
#endif
