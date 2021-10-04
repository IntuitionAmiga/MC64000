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
#include <cmath>
#include <cstdlib>

#include "standard_test_host.hpp"
#include "loader/symbol.hpp"
#include "host/macros.hpp"
#include "machine/register.hpp"

using MC64K::Loader::Symbol;
using MC64K::Machine::Interpreter;
using MC64K::Misc::Version;

namespace MC64K {
namespace StandardTestHost {

/**
 * Basic IO routines
 */
namespace IO {

char const* sDefaultByteFormat   = "%c";
char const* sDefaultWordFormat   = "%" PFS16;
char const* sDefaultLongFormat   = "%" PFS32;
char const* sDefaultQuadFormat   = "%" PFS64;
char const* sDefaultSingleFormat = "%g";
char const* sDefaultDoubleFormat = "%g";


char const* sByteFormat   = sDefaultByteFormat;
char const* sWordFormat   = sDefaultWordFormat;
char const* sLongFormat   = sDefaultLongFormat;
char const* sQuadFormat   = sDefaultQuadFormat;
char const* sSingleFormat = sDefaultSingleFormat;
char const* sDoubleFormat = sDefaultDoubleFormat;

/**
 * Basic IO
 *
 * The VM has put the byte value of the enumeration on the stack and triggered our HCF vector.
 *
 */
Interpreter::Status outputString() {

    int iOperation = (int) *Interpreter::gpr(Machine::GPRegister::SP).piByte;

    switch (iOperation) {
        case INIT:
            break;
        case PRINT_STRING:
            std::fputs(Interpreter::gpr(Machine::GPRegister::R0).sString, stdout);
            break;
        case PRINT_BYTE:
            std::printf(sByteFormat, Interpreter::gpr(Machine::GPRegister::R0).iByte);
            break;
        case PRINT_WORD:
            std::printf(sWordFormat, Interpreter::gpr(Machine::GPRegister::R0).iWord);
            break;
        case PRINT_LONG:
            std::printf(sLongFormat, Interpreter::gpr(Machine::GPRegister::R0).iLong);
            break;
        case PRINT_QUAD:
            std::printf(sQuadFormat,   Interpreter::gpr(Machine::GPRegister::R0).iQuad);
            break;
        case PRINT_SINGLE:
            std::printf(sSingleFormat, (double)Interpreter::fpr(Machine::FPRegister::FP0).fSingle);
            break;
        case PRINT_DOUBLE:
            std::printf(sDoubleFormat, (double)Interpreter::fpr(Machine::FPRegister::FP0).fDouble);
            break;
        case SET_FMT_BYTE:
            sByteFormat = Interpreter::gpr(Machine::GPRegister::R0).sString;
            break;
        case SET_FMT_WORD:
            sWordFormat = Interpreter::gpr(Machine::GPRegister::R0).sString;
            break;
        case SET_FMT_LONG:
            sLongFormat = Interpreter::gpr(Machine::GPRegister::R0).sString;
            break;
        case SET_FMT_QUAD:
            sQuadFormat = Interpreter::gpr(Machine::GPRegister::R0).sString;
            break;
        case SET_FMT_SINGLE:
            sSingleFormat = Interpreter::gpr(Machine::GPRegister::R0).sString;
            break;
        case SET_FMT_DOUBLE:
            sDoubleFormat = Interpreter::gpr(Machine::GPRegister::R0).sString;
            break;
        case CLR_FMT_BYTE:
            sByteFormat = sDefaultByteFormat;
            break;
        case CLR_FMT_WORD:
            sWordFormat = sDefaultWordFormat;
            break;
        case CLR_FMT_LONG:
            sLongFormat = sDefaultLongFormat;
            break;
        case CLR_FMT_QUAD:
            sQuadFormat = sDefaultQuadFormat;
            break;
        case CLR_FMT_SINGLE:
            sSingleFormat = sDefaultSingleFormat;
            break;
        case CLR_FMT_DOUBLE:
            sDoubleFormat = sDefaultDoubleFormat;
            break;

        default:
            std::fprintf(stderr, "\n>>>> Unknown IO operation %d\n", iOperation);

            return Interpreter::UNKNOWN_HOST_CALL;
            break;
    }

    return Interpreter::RUNNING;
}

}

/**
 *  hcf allocate r0 => r0
 */
Machine::Interpreter::Status allocate() {
    uint64 uSize = Interpreter::gpr(0).uQuad;
    void *p = std::malloc(uSize);
    if (!p) {
        throw MC64K::OutOfMemoryException();
    }
    Interpreter::gpr(0).pAny = p;
    return Interpreter::RUNNING;
}

/**
 *  hcf free r0
 */
Machine::Interpreter::Status release() {
    void* p = Interpreter::gpr(0).pAny;
    std::free(p);
    return Interpreter::RUNNING;
}

/**
 * Example host provided global data
 */
uint64        testGlobalU = 0xABADCAFE;
float64 const testConstPi = M_PI;
char const*   testString  = "Hello";

/**
 * Declare the Standard Test Host
 */
Host::Definition instance(

    // Host name and version
    "Standard Test Host",
    Version(1, 0, 0),

    // Host Vectors
    {
        IO::outputString,
        allocate,
        release
    },

    // Symbols this host exports to the virtual code.
    {
        EXPORT_SYMBOL("abadcafe", Symbol::READ|Symbol::WRITE, &testGlobalU),
        EXPORT_SYMBOL("M_PI_f64", Symbol::READ, &testConstPi),
        EXPORT_SYMBOL("my_external_reference", Symbol::READ, testString)
    },

    // Symbols this host expects to be able to access from the virtual code
    {
        IMPORT_SYMBOL("main", Symbol::EXECUTE),
        IMPORT_SYMBOL("exit", Symbol::EXECUTE),
    }
);

}}
