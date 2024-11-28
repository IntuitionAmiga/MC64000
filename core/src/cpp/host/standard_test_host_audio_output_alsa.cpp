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

#include <host/audio/alsa/device.hpp>

namespace MC64K::StandardTestHost::Audio {

namespace Alsa {

::snd_pcm_format_t const OutputPCMDevice::aSampleFormatMap[] = {
    SND_PCM_FORMAT_S8,
    SND_PCM_FORMAT_S16_LE
};

uint8 const OutputPCMDevice::aBytesPerSample[] = {
    1,
    2
};

OutputPCMDevice::~OutputPCMDevice() {
    if (oContext.oBuffer.piByte) {
        delete[] oContext.oBuffer.piByte;
    }
}

OutputPCMDevice::OutputPCMDevice(
    uint16 uDesiredSampleRateHz,
    uint16 uBufferLengthMs,
    uint8  uChannelMode,
    uint8  uSampleFormat
) :
    oPCMHandle(),
    poParams(nullptr),
    uFrames(IConfig::PACKET_SIZE),
    uSampleRateHz(uDesiredSampleRateHz),
    iDirection(0)
{
    ::snd_pcm_t* poHandle = nullptr;
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

    // Own the handle. From this point on, it's managed by the PCMHandle.
    oPCMHandle.set(poHandle);

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
        aSampleFormatMap[uSampleFormat]
    );
    std::fprintf(
        stderr,
        "\tFormat set to %d\n",
        aSampleFormatMap[uSampleFormat]
    );

    // Set the channel count. Our enum maps directly
    ::snd_pcm_hw_params_set_channels(
        poHandle,
        poParams,
        uChannelMode
    );
    std::fprintf(
        stderr,
        "\tChannels set to %d\n",
        uChannelMode
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

    // Fill in the context structure, which is used by the VM
    oContext.uSamplesSent    = 0;
    oContext.uPacketLength   = (uint16)uFrames;
    oContext.uSampleRateHz   = (uint16)uSampleRateHz;
    oContext.uChannelMode    = uChannelMode;
    oContext.uSampleFormat   = uSampleFormat;
    oContext.uBytesPerSample = (uint8)(aBytesPerSample[uSampleFormat] * uChannelMode);
    oContext.uReserved       = 0;

    size_t uBufferSize = (uBufferLengthMs * uSampleRateHz) / 1000;

    std::fprintf(
        stderr,
        "\tRequired buffer length for %u ms at %uHz is %zu\n",
        (uint32)uBufferLengthMs,
        (uint32)uSampleRateHz,
        uBufferSize
    );

    // Round to frame size
    uBufferSize = uFrames * ((uBufferSize + uFrames - 1) / uFrames);
    oContext.uBufferLength  = (uint32)uBufferSize;
    uBufferSize *= oContext.uBytesPerSample;
    oContext.oBuffer.piByte = new int8[uBufferSize];
    oContext.uBufferSize    = (uint32)uBufferSize;
    oContext.poOutputDevice = this;

    std::fprintf(
        stderr,
        "\tRounded buffer is %u, %u bytes/sample, total %u bytes, allocated at %p\n",
        (uint32)oContext.uBufferLength,
        (uint32)oContext.uBytesPerSample,
        (uint32)(oContext.uBufferLength * oContext.uBytesPerSample),
        oContext.oBuffer.piByte
    );

}

Context* OutputPCMDevice::getContext() {
    return &oContext;
}

void OutputPCMDevice::write(void const* pBuffer, size_t uLength) {
    ::snd_pcm_t* poHandle = oPCMHandle.get();
    int64 iResult = ::snd_pcm_writei(poHandle, pBuffer, uLength);
    if (iResult == -EPIPE) {
        /* EPIPE means underrun */
        std::fprintf(stderr, "AlsaOutputPCMDevice::write(): underrun occurred\n");
        ::snd_pcm_prepare(poHandle);
        return;
    } else if (iResult < 0) {
        std::fprintf(
            stderr,
            "error from writei: %s\n",
            snd_strerror((int)iResult)
        );
        return;
    } else if (iResult != (int64)uLength) {
        std::fprintf(
            stderr,
            "short write, write %ld frames\n",
            iResult
        );
    }
    oContext.uSamplesSent += (uint64)iResult;
}

} // end of Alsa namespace

// Audio namespace
OutputPCMDevice* createOutputPCMDevice(
    uint16 uDesiredSampleRateHz,
    uint16 uDesiredBufferLengthMs,
    Output::ChannelMode iChannelMode,
    Output::Format      iSampleFormat
) {
    return new Alsa::OutputPCMDevice(
        uDesiredSampleRateHz,
        uDesiredBufferLengthMs,
        iChannelMode,
        iSampleFormat
    );
}

} // namespace

