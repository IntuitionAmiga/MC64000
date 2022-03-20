#ifndef MC64K_LOADER_ERROR_HPP
    #define MC64K_LOADER_ERROR_HPP

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

namespace MC64K::Loader {

/**
 * Error
 *
 * Exception class for any sort of file IO related loading errors
 */
class Error {
    public:
        char const* sFileName;
        char const* sIssue;
        uint64 uChunkID;

        /**
         * Constructor
         *
         * @param char const*  sFileName
         * @param char const*  sIssue
         * @param uint64 const uChunkID
         */
        Error(char const* sFileName, char const* sIssue, uint64 const uChunkID = 0) :
            sFileName(sFileName),
            sIssue(sIssue),
            uChunkID(uChunkID) {}
};

/**
 * LinkError
 */
class LinkError {

};

}// namespace
#endif
