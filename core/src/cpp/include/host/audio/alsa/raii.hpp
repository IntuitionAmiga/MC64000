#ifndef MC64K_STANDARD_TEST_HOST_AUDIO_ALSA_RAII_HPP
    #define MC64K_STANDARD_TEST_HOST_AUDIO_ALSA_RAII_HPP

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

#include <cstdio>
#include <host/audio/output.hpp>
#include <host/audio/config.hpp>
#include <alsa/asoundlib.h>

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Audio::Alsa {

/**
 * RAII Handle for the snd_pcm_t handle.
 */
class PCMHandle {
    private:
        ::snd_pcm_t* poHandle;

    public:
        PCMHandle();
        ~PCMHandle();
        PCMHandle(PCMHandle const&) = delete;
        PCMHandle& operator=(PCMHandle const&) = delete;

        void set(::snd_pcm_t* poHandle);
        ::snd_pcm_t* get() const;

};

inline PCMHandle::PCMHandle() : poHandle(nullptr) {

}

inline PCMHandle::~PCMHandle() {
    if (poHandle) {
        ::snd_pcm_drain(poHandle);
        ::snd_pcm_close(poHandle);
        std::fprintf(stderr, "~PCMHandle(). Closed audio device %p\n", poHandle);
    }
}

inline void PCMHandle::set(::snd_pcm_t* poHandle) {
    this->poHandle = poHandle;
}

inline ::snd_pcm_t* PCMHandle::get() const {
    return poHandle;
}

};
#endif
