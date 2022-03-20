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
 * PixelPointer. Simple union to avoid casts.
 */
union SamplePointer {
    void*  pAny;
    int8*  poByte;
    int16* piWord;
    SamplePointer(): pAny(nullptr) {}
};

class OutputPCMDevice;

struct Context {
    SamplePointer    oBuffer;
    uint32           uBufferLength;
    uint16           uRateHz;
    uint8            uChannels;
    uint8            uBits;
    OutputPCMDevice* poOutput;
};

/**
 * Super minimalist output device. As soon as the device is acquired, it's open and ready for input.
 * Destroying it closes resource.
 *
 * The output rate can be specified, but the implementation is allowed to resample etc.
 */
class OutputPCMDevice {

    public:
        enum ChannelMode {
            CH_MONO   = 1,
            CH_STEREO = 2
        };

        enum Resolution {
            RES_8_BIT  = 0,
            RES_16_BIT = 1
        };

        virtual ~OutputPCMDevice() {};
        virtual void write(void const* pBuffer, size_t uLength) = 0;

};

OutputPCMDevice* createOutputPCMDevice(
    OutputPCMDevice::ChannelMode iChannelMode,
    OutputPCMDevice::Resolution iResolution,
    uint16 fRateHz
);

};

class Error {

};

#endif
