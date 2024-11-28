#ifndef MC64K_STANDARD_TEST_HOST_AUDIO_OUTPUT_HPP
    #define MC64K_STANDARD_TEST_HOST_AUDIO_OUTPUT_HPP

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

#include <host/standard_test_host_audio.hpp>

/**
 * PDE to the rescue...
 */
namespace MC64K::StandardTestHost::Audio {


/**
 * SamplePointer. Simple union to avoid casts.
 */
union SamplePointer {
    void*  pAny;
    int8*  piByte;
    int16* piWord;
    SamplePointer(): pAny(nullptr) {}
};

class OutputPCMDevice;

struct Context;

/**
 * Super minimalist output device. As soon as the device is acquired, it's open and ready for input.
 * Destroying it closes resource.
 *
 * The output rate can be specified, but the implementation is allowed to resample etc.
 */
class OutputPCMDevice {

    public:
        virtual ~OutputPCMDevice() {};
        virtual Context* getContext() = 0;
        virtual void     write(void const* pBuffer, size_t uLength) = 0;
};

struct Context {
    SamplePointer    oBuffer;         // This is allocated for us
    uint32           uBufferLength;   // The actual length of the buffer (will be a multiple of packet length)
    uint32           uSubmitLength;   // The number of sample frames to send in the next call to write
    uint32           uBufferSize;     // The buffer size in bytes
    uint16           uPacketLength;   // The smallest unit of audio. The buffer will be allocated as a multiple.
    uint16           uSampleRateHz;   // The sample rate. This may be adjusted from what was requested.
    uint64           uSamplesSent;    // The total number of sample frames sent so far
    uint8            uChannelMode;    // The channel mode, see Output::ChannelMode
    uint8            uSampleFormat;   // The per sample format, see Output::Format
    uint8            uBytesPerSample; // The size of a single sample frame, in bytes
    uint8            uReserved;       // Reserved
    OutputPCMDevice* poOutputDevice;
};

OutputPCMDevice* createOutputPCMDevice(
    uint16 uDesiredSampleRateHz,
    uint16 uDesiredBufferLengthMs,
    Output::ChannelMode iChannelMode,
    Output::Format      iSampleFormat
);

};

class Error {

};

#endif
