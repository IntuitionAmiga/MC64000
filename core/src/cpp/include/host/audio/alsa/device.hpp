#ifndef MC64K_STANDARD_TEST_HOST_AUDIO_ALSA_DEVICE_HPP
    #define MC64K_STANDARD_TEST_HOST_AUDIO_ALSA_DEVICe_HPP

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

#include "raii.hpp"

using MC64K::Machine::Interpreter;

namespace MC64K::StandardTestHost::Audio::Alsa {

/**
 * Alsa realisation of the OutputPCMDevice interface
 *
 */
class OutputPCMDevice : public Audio::OutputPCMDevice {

    private:
        static ::snd_pcm_format_t const aSampleFormatMap[];
        static uint8 const              aBytesPerSample[];

        PCMHandle               oPCMHandle;
        ::snd_pcm_hw_params_t*  poParams;
        ::snd_pcm_uframes_t     uFrames;
        uint32                  uPeriod;
        uint32                  uSampleRateHz;
        int32                   iDirection;

        Context                 oContext;

    public:
        OutputPCMDevice(
            uint16 uDesiredSampleRateHz,
            uint16 uDesiredBufferLengthMs,
            uint8  uChannelMode,
            uint8  uSampleFormat
        );
        ~OutputPCMDevice();
        Context* getContext();
        void write(void const* pBuffer, size_t uLength);
};

};
#endif
