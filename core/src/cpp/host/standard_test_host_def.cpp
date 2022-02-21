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
#include "host/standard_test_host_vector_math.hpp"
#include "loader/symbol.hpp"
#include "host/macros.hpp"
#include "machine/register.hpp"

using MC64K::Loader::Symbol;
using MC64K::Machine::Interpreter;
using MC64K::Misc::Version;

namespace MC64K::StandardTestHost {

char const*  sHostInfo         = "Standard Test Host";
char const** pHostCLIParams    = 0;
uint64       uHostNumCLIParams = 0;

/**
 * @inheritDoc
 */
void setCLIParameters(unsigned const uArgC, char const** pArgV) {
    uHostNumCLIParams = uArgC;
    pHostCLIParams    = pArgV;
}


/**
 * Declare the Standard Test Host
 */
Host::Definition instance(

    // Host name and version
    sHostInfo,
    Version(1, 0, 0),

    // Host ABI Vectors
    {
        IO::hostVector,
        Mem::hostVector,
        VectorMath::hostVector
    },

    // Symbols this host exports to the virtual code.
    {
        EXPORT_SYMBOL("host_info", Symbol::READ, sHostInfo),
        EXPORT_SYMBOL("host_cli_num_params", Symbol::READ, &uHostNumCLIParams),
        EXPORT_SYMBOL("host_cli_params",  Symbol::READ, &pHostCLIParams)
    },

    // Symbols this host expects to be able to access from the virtual code
    {
        IMPORT_SYMBOL("main", Symbol::EXECUTE),
        IMPORT_SYMBOL("exit", Symbol::EXECUTE),
    }
);

}
