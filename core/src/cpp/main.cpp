#include <cstdio>
#include <cstdlib>

#include "standard_test_host.hpp"
#include "host/runtime.hpp"
#include "loader/error.hpp"

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

        MC64K::Host::Runtime oRuntime(MC64K::StandardTestHost::instance, sExecutableName);

        oRuntime.invoke(MC64K::StandardTestHost::MAIN);

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
