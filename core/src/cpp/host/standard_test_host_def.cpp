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

#include "host/standard_test_host_io.hpp"
#include "host/standard_test_host_mem.hpp"
#include "loader/symbol.hpp"
#include "host/macros.hpp"
#include "machine/register.hpp"

using MC64K::Loader::Symbol;
using MC64K::Machine::Interpreter;
using MC64K::Misc::Version;

namespace MC64K {
namespace StandardTestHost {

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

    // Host ABI Vectors
    {
        IO::hostVector,
        Mem::hostVector,
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
