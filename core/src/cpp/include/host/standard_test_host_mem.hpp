#ifndef __MC64K_STANDARD_TEST_HOST_MEM_HPP__
    #define __MC64K_STANDARD_TEST_HOST_MEM_HPP__

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

#include "standard_test_host.hpp"

namespace MC64K {
namespace StandardTestHost {

/**
 * Mem namespace. Gathers together all parts of the Standard Test Host Memory functionality.
 *
 */
namespace Mem {

/**
 * Call
 *
 * Enumeration of calls in the Mem namespace
 */
typedef enum {
    INIT = 0,
    DONE,

    /**
     * func mem_alloc(r0/d0 uint64 size) => r8/a0 void* buffer, r0/d0 uint64 error
     */
    ALLOC,

    /**
     * func mem_free(r8/a0 void* buffer) => r0/d0 uint64 error
     */
    FREE,

    /**
     * func mem_copy(r8/a0 void* from, r9/a1 void* to, r0/d0 uint64 size) => r0/d0 uint64 error
     */
    COPY,

    /**
     * func mem_fill_<byte:word:long:quad>(r8/a0 void* buffer, r0/d0 uint64 size, r1/d1 uint64 value) => r0/d0 uint64 error
     */
    FILL_BYTE,
    FILL_WORD,
    FILL_LONG,
    FILL_QUAD
} Call;

/**
 * Error return values
 */
typedef enum {
    ERR_NO_MEM = 100,
    ERR_MEM
} Result;

Interpreter::Status hostVector();

}}} // namespace

#endif
