#ifndef MC64K_SYNTH_SIGNAL_ENV_SHAPE_HPP
    #define MC64K_SYNTH_SIGNAL_ENV_SHAPE_HPP

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
#include <initializer_list>
#include <synth/signal.hpp>

namespace MC64K::Synth::Audio::Signal::Envelope {

class Shape : public IEnvelope {

    public:
        static constexpr float32 const MIN_TIME = 0.001f;
        static constexpr float32 const MAX_TIME = 100.0f;

        /**
         * Basic envelope point structure. A level that is reached after a length of time.
         */
        struct Point {
            float32 fLevel;
            float32 fTime;
        };

        Shape(float32 fInitial, Point const* poPoints, size_t uNumPoints);

        /**
         * Convenience constructor that allows brace initialised input
         */
        Shape(float32 fInitial, std::initializer_list<Point> const& roPoints):
            Shape(fInitial, roPoints.begin(), roPoints.size()) {}
        ~Shape();

        /**
         * @inheritDoc
         */
        Packet::ConstPtr emit(size_t uIndex) noexcept override;

        Shape* reset() noexcept override;

    private:
        /**
         * Internal representation of envelope point, expressed in terms of output sample position
         * and higher precision level (after taking into account both time and level scale properties.
         */
        struct ProcessPoint {
            size_t  uSamplePosition;
            float64 fLevel;
        };

        Packet::Ptr   oFinalPacketPtr;

        std::unique_ptr<Point[]>        poPoints;
        std::unique_ptr<ProcessPoint[]> aoProcessPoints;

        ProcessPoint* pNextProcessPoint;
        size_t        uNumPoints;
        size_t        uFinalSamplePosition;
        size_t        uXOffset;
        float64       fYOffset;
        float64       fGradient;

        void processPointList(float32 fInitial, Point const* aoInputPoints, size_t uNumPoints) noexcept;
        void recalculate() noexcept;
        void updateInterpolants() noexcept;
};

} // namespace

#endif
