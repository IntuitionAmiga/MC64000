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
 * Symbols the host application requires from the binary. Terminated by a null name.
 */
LinkSymbol aImports[] = {
    { "main", {0}, LinkSymbol::EXECUTE },
    { 0, { 0 }, 0 }
};

/**
 * Symbols the host application makes available to the binary. Terminated by a null name.
 */
LinkSymbol aExports[] = {
    { "abadcafe", { &testGlobal }, LinkSymbol::READ|LinkSymbol::WRITE },
    { 0, { 0 }, 0 }
};

/**
 * Declare the Standard Test Host
 */
Definition standardTestHost(
    "Standard Test Host",
    Version(1, 0, 0),
    aVectors,
    aExports,
    aImports
);

