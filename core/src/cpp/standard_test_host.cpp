#include <cstdio>
#include <cstdlib>

#include "machine/interpreter.hpp"
#include "loader/binary.hpp"
#include "host.hpp"

using namespace MC64K::Loader;
using namespace MC64K::Machine;

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
 * Declare the host
 */
MC64K::Host standardTestHost(
    "Standard Test Host",
    1, 0, 0,
    aVectors,
    aExports,
    aImports
);

