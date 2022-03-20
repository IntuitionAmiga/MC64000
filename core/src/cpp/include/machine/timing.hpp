#ifndef MC64K_MACHINE_TIMING_HPP
    #define MC64K_MACHINE_TIMING_HPP

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

#include <time.h>
#include <mc64k.hpp>

namespace MC64K::Machine {

/**
 * Nanosecond measurement
 */
class Nanoseconds {
    public:
        typedef uint64 Value;

        /**
         * @returnValue
         */
        static Value mark() {
            timespec oCurrent;
            clock_gettime(CLOCK_MONOTONIC, &oCurrent);
            Value  uMark = 1000000000UL * oCurrent.tv_sec ;
            return uMark + oCurrent.tv_nsec;
        }

        static Value sleep(Value uTime) {
            timespec oCurrent;
            oCurrent.tv_sec  = 0;
            oCurrent.tv_nsec = uTime;
            nanosleep(&oCurrent, &oCurrent);
            return oCurrent.tv_nsec;
        }
};

} // namespace
#endif
