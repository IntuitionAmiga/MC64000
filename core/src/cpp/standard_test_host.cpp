#include <cstdio>
#include <cstdlib>

#include "machine/interpreter.hpp"
#include "loader/linksymbol.hpp"
#include "host/definition.hpp"

using namespace MC64K::Loader;
using namespace MC64K::Machine;
using namespace MC64K::Host;
using namespace MC64K::Misc;

Interpreter::Status nativeTest() {
    std::puts("Native Call");
    return Interpreter::RUNNING;
}

uint64 testGlobal = 0xABADCAFE;

/**
 * Null terminated array of host vectors that are directly invokable via HCF
 */
Interpreter::HostCall aVectors[] = {
    nativeTest,
    0
};

/**
 * Declare the Standard Test Host
 */
Definition standardTestHost(

    // Host name and version
    "Standard Test Host",
    Version(1, 0, 0),

    // Set of host vectors
    aVectors,

    // Symbols this host exports to the virtual code
    StaticLinkSymbolSet({
        EXPORT_SYMBOL("abadcafe", LinkSymbol::READ|LinkSymbol::WRITE, &testGlobal),
        END_SYMBOL
    }),

    // Symbols this host expects to be able to access in the virtual code
    StaticLinkSymbolSet({
        IMPORT_SYMBOL("main", LinkSymbol::EXECUTE),
        END_SYMBOL
    })
);

