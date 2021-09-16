#include <cstdio>
#include <cmath>
#include <cstdlib>

#include "machine/interpreter.hpp"
#include "loader/symbol.hpp"
#include "host/definition.hpp"

using namespace MC64K::Loader;
using namespace MC64K::Machine;
using namespace MC64K::Host;
using namespace MC64K::Misc;

/**
 * Example host vectors
 */
Interpreter::Status nativeFunctionTest1() {
    std::puts("Native Call 1");
    return Interpreter::RUNNING;
}

Interpreter::Status nativeFunctionTest2() {
    std::puts("Native Call 2");
    return Interpreter::RUNNING;
}

/**
 * Example host provided global data
 */
uint64        testGlobalU = 0xABADCAFE;
const float64 testConstPi = M_PI;
const char*   testString  = "Hello";

/**
 * Declare the Standard Test Host
 */
Definition standardTestHost(

    // Host name and version
    "Standard Test Host",
    Version(1, 0, 0),

    // Host Vectors
    {
        nativeFunctionTest1,
        nativeFunctionTest2
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
        //IMPORT_SYMBOL("exit", Symbol::EXECUTE),
    }
);

