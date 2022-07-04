#ifndef MC64K_STANDARD_TEST_HOST_MEM_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_HPP

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

namespace MC64K::StandardTestHost::Mem {

/**
 * Call
 *
 * Enumeration of calls in the Mem namespace
 */
enum Call {
    INIT = 0,
    DONE,

    /**
     * func mem_alloc(r0/d0 uint64 size) => r8/a0 void* buffer, r0/d0 uint64 error
     *
     * Allocates a contiguous block of memory of at least the reuested size.
     */
    ALLOC,

    /**
     * func mem_free(r8/a0 void* buffer) => r0/d0 uint64 error
     *
     * Releases a previously allocated block of memory.
     */
    FREE,

    /**
     * func mem_alloc_buffer(r0/d0 uint16:uint16 buffer_count:element_size) => r8/a0 Buffer* buffer, r0/d0 uint64 error
     *
     * Creates a fixed count buffer of elements of a set size that can then be independently allocated and released
     * from the buffer. A single buffer can hold a maximum of 65536 elements of up to 65528 bytes in size, allocated
     * as a single contiguous block:
     *
     * The requested element size is rounded to the nearest 8 bytes. There is an overhead of 8 bytes per element.
     * The smallest element size is 24 bytes.
     * The buffer count is rounded to 64.
     * A requested buffer count of 0 is interpreted as 65536
     *
     * The desired element size and buffer count are packed into the lowest 16 bit pair of r0.
     *
     * The primary motivation for this is for the management of frequently recycled structure instances.
     *
     */
    ALLOC_BUFFER,

    /**
     * func mem_alloc_buffer(r8/a0 Buffer* buffer) => r0/d0 uint64 error
     *
     * Frees the entire buffer.
     */
    FREE_BUFFER,

    /**
     * func mem_alloc_element(r8/a0 Buffer* buffer) => r8/a0 void* element, r0/d0 uint64 error
     *
     * Allocates a single element from a buffer.
     */
    ALLOC_ELEMENT,

    /**
     * func mem_free_element(r8/a0 void* element) => r0/d0 uint64 error
     *
     * Frees an element, returning it to it's buffer.
     */
    FREE_ELEMENT,

    /**
     * func mem_copy(r8/a0 void* from, r9/a1 void* to, r0/d0 uint64 size) => r0/d0 uint64 error
     *
     * Block memory copy. Overlapping regions are not supported.
     */
    COPY,

    /**
     * func mem_swap_<itype>(r8/a0 void* from, r9/a1 void* to, r0/d0 uint64 count) => r0/d0 uint64 error
     *
     * Block memory byteswap. Each assumes the address is aligned to the word type. Where this is not the case,
     * the operation proceeds from the next aligned address with one fewer count. Overlapping regions are not
     * supported, except in the case that the swap is happening in place.
     */
    BSWAP_WORD,
    BSWAP_LONG,
    BSWAP_QUAD,

    /**
     * func mem_fill_<itype>(r8/a0 void* buffer, r0/d0 <itype> value, r1/d1 uint64 count) => r0/d0 uint64 error
     *
     * Block memory fill. Each assumes the address is aligned to the word type. Where this is not the case,
     * the operation proceeds from the next aligned address with one fewer count.
     */
    FILL_BYTE,
    FILL_WORD,
    FILL_LONG,
    FILL_QUAD,

    /**
     * func mem_and_<itype>(r8/a0 void* buffer, r0/d0 <itype> value, r1/d1 uint64 count) => r0/d0 uint64 error
     *
     * Applies an AND mask to a range of memory. Each assumes the address is aligned to the word type. Where this is
     * not the case, the operation proceeds from the next aligned address with one fewer count.
     */
    AND_BYTE,
    AND_WORD,
    AND_LONG,
    AND_QUAD,

    /**
     * func mem_or_<itype>(r8/a0 void* buffer, r0/d0 <itype> value, r1/d1 uint64 count) => r0/d0 uint64 error
     *
     * Applies an OR mask to a range of memory. Each assumes the address is aligned to the word type. Where this is
     * not the case, the operation proceeds from the next aligned address with one fewer count.
     */
    OR_BYTE,
    OR_WORD,
    OR_LONG,
    OR_QUAD,

    /**
     * func mem_eor_<itype>(r8/a0 void* buffer, r0/d0 <itype> value, r1/d1 uint64 count) => r0/d0 uint64 error
     *
     * Applies an Exclusive OR mask to a range of memory. Each assumes the address is aligned to the word type. Where
     * this is not the case, the operation proceeds from the next aligned address with one fewer count.
     */
    EOR_BYTE,
    EOR_WORD,
    EOR_LONG,
    EOR_QUAD,

    /**
     * func mem_find_<itype>(r8/a0 void* buffer, r0/d0 <itype> value, r1/d1 uint64 count) => r8/a0 void* result, r0/d0 uint64 error
     *
     * Performs a linear search for the requested value.
     */
    FIND_BYTE,
    FIND_WORD,
    FIND_LONG,
    FIND_QUAD,

    STR_LENGTH,
    STR_COMPARE,
};

/**
 * Error return values
 */
enum Result {
    ERR_NO_MEM = 100,
    ERR_MEM,
    ERR_MEM_INVALID_BUFFER,
    ERR_MEM_BUFFER_FULL,
    ERR_MEM_INVALID_ELEMENT
};

Interpreter::Status hostVector(uint8 uFunctionID);

} // namespace

#endif
