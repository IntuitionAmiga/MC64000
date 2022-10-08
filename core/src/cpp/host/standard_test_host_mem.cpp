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
#include <cstdlib>
#include <cstring>
#include <host/standard_test_host_mem.hpp>
#include <machine/register.hpp>
#include <host/mem/inline.hpp>

using MC64K::Machine::Interpreter;

using MC64K::Host::Memory::ElementBuffer;

namespace MC64K::StandardTestHost::Mem {

/**
 * Mem::hostVector(uint8 uFunctionID)
 */
Interpreter::Status hostVector(uint8 uFunctionID) {
    Call iOperation = (Call) uFunctionID;
    switch (iOperation) {
        case INIT:
        case DONE:
            break;

        case ALLOC:
            if (uint64 uSize  = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
                void* pBuffer = std::malloc(uSize);
                Interpreter::gpr<ABI::PTR_REG_0>().pAny  = pBuffer;
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = pBuffer ?
                    (uint64)ABI::ERR_NONE :
                    (uint64)ERR_NO_MEM;
            } else {
                Interpreter::gpr<ABI::PTR_REG_0>().pAny  = 0;
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ERR_MEM;
            }
            break;

        case FREE:
            std::free(Interpreter::gpr<ABI::PTR_REG_0>().pAny);
            Interpreter::gpr<ABI::PTR_REG_0>().pAny = 0;
            break;

        case ALLOC_BUFFER: {
            uint32 uParams = Interpreter::gpr<ABI::INT_REG_0>().uLong;
            ElementBuffer* pBuffer = ElementBuffer::allocateBuffer(
                (uint16)(uParams >> 16),
                (uint16)(uParams & 0xFFFF)
            );
            Interpreter::gpr<ABI::PTR_REG_0>().pAny  = pBuffer;
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = pBuffer ?
                (uint64)ABI::ERR_NONE :
                (uint64)ERR_NO_MEM;
            break;
        }

        case FREE_BUFFER: {
            ElementBuffer* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().address<ElementBuffer>();
            if (pBuffer) {
                if ( (ElementBuffer::SUCCESS == ElementBuffer::validate(pBuffer)) ) {
                    ElementBuffer::freeBuffer(pBuffer);
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
                } else {
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = ERR_MEM_INVALID_BUFFER;
                }
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
            }
            break;
        }
        case ALLOC_ELEMENT: {
            ElementBuffer* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().address<ElementBuffer>();
            if (pBuffer) {
                if ( (ElementBuffer::SUCCESS == ElementBuffer::validate(pBuffer)) ) {
                    void *pElement = pBuffer->alloc();
                    Interpreter::gpr<ABI::PTR_REG_0>().pAny  = pElement;
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = pElement ?
                        (uint64)ABI::ERR_NONE :
                        (uint64)ERR_MEM_BUFFER_FULL;
                } else {
                    Interpreter::gpr<ABI::PTR_REG_0>().pAny  = nullptr;
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = ERR_MEM_INVALID_BUFFER;
                }
            } else {
                Interpreter::gpr<ABI::PTR_REG_0>().pAny  = nullptr;
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
            }
            break;
        }

        case FREE_ELEMENT: {
            ElementBuffer* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().address<ElementBuffer>();
            void *pElement = Interpreter::gpr<ABI::PTR_REG_1>().pAny;
            if (pBuffer && pElement) {
                if ( (ElementBuffer::SUCCESS == ElementBuffer::validate(pBuffer)) ) {
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = (ElementBuffer::SUCCESS == pBuffer->free(pElement)) ?
                        (uint64)ABI::ERR_NONE :
                        (uint64)ERR_MEM_INVALID_ELEMENT;
                } else {
                    Interpreter::gpr<ABI::PTR_REG_0>().pAny  = nullptr;
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = ERR_MEM_INVALID_BUFFER;
                }
            } else {
                Interpreter::gpr<ABI::PTR_REG_0>().pAny  = nullptr;
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
            }
            break;
        }

        case COPY:
            if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
                void* pFrom = Interpreter::gpr<ABI::PTR_REG_0>().pAny;
                void* pTo   = Interpreter::gpr<ABI::PTR_REG_1>().pAny;
                if (pFrom && pTo) {
                    Host::Memory::copy(pTo, pFrom, uSize);
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
                } else {
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
                }
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
            }
            break;

        case BSWAP_WORD: swapBlock<uint16>(); break;
        case BSWAP_LONG: swapBlock<uint32>(); break;
        case BSWAP_QUAD: swapBlock<uint64>(); break;
        case AND_BYTE:   andBlock<uint8>();  break;
        case AND_WORD:   andBlock<uint16>(); break;
        case AND_LONG:   andBlock<uint32>(); break;
        case AND_QUAD:   andBlock<uint64>(); break;
        case OR_BYTE:    orBlock<uint8>();   break;
        case OR_WORD:    orBlock<uint16>();  break;
        case OR_LONG:    orBlock<uint32>();  break;
        case OR_QUAD:    orBlock<uint64>();  break;
        case EOR_BYTE:   eorBlock<uint8>();  break;
        case EOR_WORD:   eorBlock<uint16>(); break;
        case EOR_LONG:   eorBlock<uint32>(); break;
        case EOR_QUAD:   eorBlock<uint64>(); break;
        case FILL_BYTE:  fillBlock<uint8>();  break;
        case FILL_WORD:  fillBlock<uint16>(); break;
        case FILL_LONG:  fillBlock<uint32>(); break;
        case FILL_QUAD:  fillBlock<uint64>(); break;
        case FIND_BYTE:  findBlock<uint8>();  break;
        case FIND_WORD:  findBlock<uint16>(); break;
        case FIND_LONG:  findBlock<uint32>(); break;
        case FIND_QUAD:  findBlock<uint64>(); break;

        case STR_LENGTH: {
            char const* sString = Interpreter::gpr<ABI::PTR_REG_0>().sString;
            if (sString) {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = std::strlen(sString);
                Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NONE;
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = 0;
                Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NULL_PTR;
            }
            break;
        }
        case STR_COMPARE: {
            char const* sString1 = Interpreter::gpr<ABI::PTR_REG_0>().sString;
            char const* sString2 = Interpreter::gpr<ABI::PTR_REG_1>().sString;
            if (sString1 && sString2) {
                Interpreter::gpr<ABI::INT_REG_0>().iQuad = std::strcmp(sString1, sString2);
                Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NONE;
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = 0;
                Interpreter::gpr<ABI::INT_REG_1>().uQuad = ABI::ERR_NULL_PTR;
            }
            break;
        }

        default:
            std::fprintf(stderr, "Unknown Mem operation %d\n", iOperation);
            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

} // namespace

