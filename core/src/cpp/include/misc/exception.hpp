#ifndef __MC64K_EXCEPTION_HPP__
    #define __MC64K_EXCEPTION_HPP__

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

namespace MC64K {

/**
 * Root exception class for all MC64K runtime error
 */
class Exception {
    private:
        const char* sMessage;
    public:
        Exception(const char* sMessage = "Exception") : sMessage(sMessage) {}

        const char* getMessage() const {
            return sMessage;
        }
};

/**
 * Sundry top level exceptions.
 */
class OutOfRangeException  : public Exception {
    public:
        OutOfRangeException(const char* sMessage = "Value out of range") : Exception(sMessage) {}
};

class OutOfMemoryException : public Exception {
    public:
        OutOfMemoryException(const char* sMessage = "Insufficent memory") : Exception(sMessage) {}
};

}// namespace

#endif
