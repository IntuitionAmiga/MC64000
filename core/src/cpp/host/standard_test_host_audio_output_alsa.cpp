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

namespace MC64K::StandardTestHost::Audio {

::snd_pcm_format_t aSampleFormatMap[] = {
    SND_PCM_FORMAT_S8,
    SND_PCM_FORMAT_S16_LE
};

class AlsaOutputPCMDevice : public OutputPCMDevice {

    private:
        ::snd_pcm_t*            poHandle;
        ::snd_pcm_hw_params_t*  poParams;
        ::snd_pcm_uframes_t     uFrames;
        uint32                  uPeriod;
        uint32                  uSampleRateHz;
        int32                   iDirection;

    public:
        AlsaOutputPCMDevice(ChannelMode iChannelMode, Resolution iResolution, uint16 fRateHz);
        ~AlsaOutputPCMDevice();
        void write(void const* pBuffer, size_t uLength);

};

AlsaOutputPCMDevice::~AlsaOutputPCMDevice() {
    if (poHandle) {
        ::snd_pcm_drain(poHandle);
        ::snd_pcm_close(poHandle);
        std::fprintf(stderr, "Closed audio device %p\n", poHandle);
    }
}

AlsaOutputPCMDevice::AlsaOutputPCMDevice(
    ChannelMode iChannelMode,
    Resolution iResolution,
    uint16 uRateHz
) :
    poHandle(nullptr),
    poParams(nullptr),
    uFrames(IConfig::PACKET_SIZE),
    uSampleRateHz(uRateHz),
    iDirection(0)
{
    // Open PCM device for playback.
    int iResult = ::snd_pcm_open(
        &poHandle,
        "default",
        SND_PCM_STREAM_PLAYBACK,
        0
    );
    if (iResult < 0) {
        std::fprintf(
            stderr,
            "AlsaOutputPCMDevice: Unable to open device: %s\n",
            ::snd_strerror(iResult)
        );
        throw Error();
    }

    std::fprintf(
        stderr,
        "AlsaOutputPCMDevice: obtained handle at %p\n", poHandle
    );

    // this is a macro
    snd_pcm_hw_params_alloca(&poParams);
    ::snd_pcm_hw_params_any(poHandle, poParams);

    ::snd_pcm_hw_params_set_access(
        poHandle,
        poParams,
        SND_PCM_ACCESS_RW_INTERLEAVED
    );

    // Set the format. Our resolution is mapped to an appropriate format.
    ::snd_pcm_hw_params_set_format(
        poHandle,
        poParams,
        aSampleFormatMap[iResolution]
    );
    std::fprintf(
        stderr,
        "\tFormat set to %d\n",
        aSampleFormatMap[iResolution]
    );

    // Set the channel count. Our enum maps directly
    ::snd_pcm_hw_params_set_channels(
        poHandle,
        poParams,
        iChannelMode
    );
    std::fprintf(
        stderr,
        "\tChannels set to %d\n",
        iChannelMode
    );

    // Set our target rate. This may be changed by the driver to an appropriate nearby value that
    // is better suited.
    ::snd_pcm_hw_params_set_rate_near(
        poHandle,
        poParams,
        &uSampleRateHz,
        &iDirection
    );

    std::fprintf(
        stderr,
        "\tSample rate set to %u, direction %d\n",
        uSampleRateHz,
        iDirection
    );

    // Set the period size, in frames. This may be changed by the driver to an appropriate nearby
    // value that is better suited.
    ::snd_pcm_hw_params_set_period_size_near(
        poHandle,
        poParams,
        &uFrames,
        &iDirection
    );

    std::fprintf(
        stderr,
        "\tPeriod frame length set to %lu\n",
        uFrames
    );

    // Set the period size, in frames. This may be changed by the driver to an appropriate nearby
    // value that is better suited.
    ::snd_pcm_hw_params_get_period_time(
        poParams,
        &uPeriod,
        &iDirection
    );

    std::fprintf(
        stderr,
        "\tPeriod frame time determined as %u\n",
        uPeriod
    );

    iResult = ::snd_pcm_hw_params(poHandle, poParams);
    if (iResult < 0) {
        std::fprintf(
            stderr,
            "unable to set hw parameters: %s\n",
            snd_strerror(iResult)
        );
        ::snd_pcm_close(poHandle);
        throw Error();
    }

    std::fprintf(
        stderr,
        "\tHardware initialisation result %d\n",
        iResult
    );

    ::snd_pcm_hw_params_get_period_size(
        poParams,
        &uFrames,
        &iDirection
    );

    std::fprintf(
        stderr,
        "\tFinal hardware mandated frame size %lu, direction %d\n",
        uFrames,
        iDirection
    );

    std::fprintf(
        stderr,
        "AlsaOutputPCMDevice: Initialisation of handle %p complete\n",
        poHandle
    );

}

void AlsaOutputPCMDevice::write(void const* pBuffer, size_t uLength) {
    int64 iResult = ::snd_pcm_writei(poHandle, pBuffer, uLength);
    if (iResult == -EPIPE) {
        /* EPIPE means underrun */
        std::fprintf(stderr, "AlsaOutputPCMDevice::write(): underrun occurred\n");
        ::snd_pcm_prepare(poHandle);
    } else if (iResult < 0) {
        std::fprintf(
            stderr,
            "error from writei: %s\n",
            snd_strerror((int)iResult)
        );
    } else if (iResult != (int64)uLength) {
        std::fprintf(
            stderr,
            "short write, write %ld frames\n",
            iResult
        );
    }
}

OutputPCMDevice* createOutputPCMDevice(
    OutputPCMDevice::ChannelMode iChannelMode,
    OutputPCMDevice::Resolution iResolution,
    uint16 uRateHz
) {
    return new AlsaOutputPCMDevice(iChannelMode, iResolution, uRateHz);
}


} // namespace

