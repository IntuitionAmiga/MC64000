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
#include <synth/note.hpp>

namespace MC64K::Synth::Audio {

float32 Note::getFrequency(
    int32   iNumber,
    float32 fBendSemis,
    float32 fScalePerOctave,
    float32 fCentreValue
) {
    float32 fNote  = fScalePerOctave * ((float32)(iNumber - CENTRE_REFERENCE) + fBendSemis);
    return fCentreValue * semisToMultiplier(fNote);
}

/**
 * Simple, dirty, non-table driven note name parser.
 */
uint32 Note::getNumber(char const* sName) {
    if (!sName) {
        return ILLEGAL_NOTE;
    }

    // Incorporate the fact that C0 is 12 into the base semitone
    int iSemitone = 12;

    // First Character
    switch (*sName++) {
        case 'c':
        case 'C':
            iSemitone += 0;
            break;

        case 'd':
        case 'D':
            iSemitone += 2;
            break;

        case 'e':
        case 'E':
            iSemitone += 4;
            break;

        case 'f':
        case 'F':
            iSemitone += 5;
            break;

        case 'g':
        case 'G':
            iSemitone += 7;
            break;

        case 'a':
        case 'A':
            iSemitone += 9;
            break;

        case 'b':
        case 'B':
            iSemitone += 11;
            break;

        default:
            return ILLEGAL_NOTE;
    }

    // modifiers
    switch (*sName) {
        case '#':
            ++iSemitone;
            ++sName;
            break; // sharp

        case 'b':
            --iSemitone;
            ++sName;
            break; // flat

        case 0:
            return ILLEGAL_NOTE;
    }
    int iMultiplier = 12;
    int iNote       = 0;

octave:
    // expected values are -1, 0, 1, 2...9
    switch (*sName) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            iNote = (int)(*sName) - '0';
            break;

        case '-':
            iMultiplier = -12;
            ++sName;
            goto octave;

        default:
            return ILLEGAL_NOTE;
    }
    iNote *= iMultiplier;
    iNote += iSemitone;

    if (iNote >= 0 && iNote < 128) {
        return iNote;
    }
    return ILLEGAL_NOTE;
}


}
