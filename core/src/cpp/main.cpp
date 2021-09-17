#include <cstdio>
#include <cstdlib>

#include "machine/interpreter.hpp"
#include "loader/executable.hpp"
#include "standard_test_host.hpp"

using namespace MC64K::Loader;
using namespace MC64K::Machine;
using namespace MC64K::Host;

/**
 * Entry point
 */
int main(int iArgN, const char** aArgV) {

    if (iArgN < 2) {
        std::puts("Missing required parameter");
        std::exit(EXIT_FAILURE);
    }

    try {
        const char* sExecutableName = aArgV[1];

        // Initialise the binary loader
        Binary oMC64KBinary(MC64K::StandardTestHost::instance);
        const Executable* pExecutable = oMC64KBinary.load(sExecutableName);

        Interpreter::setExecutable(pExecutable);

//
//         if (aExports) {
//             const int iDumpState = Interpreter::STATE_FPR|Interpreter::STATE_GPR|Interpreter::STATE_TMP;
//             Interpreter::allocateStack(256);
//             Interpreter::setHostFunction(nativeTest, 0x69);
//             Interpreter::setProgramCounter(aExports[0].pByteCode);
//             Interpreter::dumpState (iDumpState);
//             Interpreter::run();
//             Interpreter::dumpState (iDumpState|Interpreter::STATE_STACK);
//             Interpreter::freeStack();
//         }
        delete pExecutable;
    } catch (MC64K::Loader::Error& oError) {
        std::printf(
            "Unable to load binary file \"%s\", %s.\n",
            oError.sFileName,
            oError.sIssue
        );
        std::exit(EXIT_FAILURE);
    } catch (MC64K::Machine::Error& oError) {
        std::printf(
            "Machine error: %s.\n",
            oError.sIssue
        );
        std::exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
