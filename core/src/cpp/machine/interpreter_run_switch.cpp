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
#include <machine/interpreter.hpp>
#include <machine/timing.hpp>
#include <bytecode/opcode.hpp>
#include <machine/inline.hpp>
#include <machine/gnarly.hpp>

namespace MC64K::Machine {

/**
 * @inheritDoc
 */
void Interpreter::run() {

    using namespace MC64K::ByteCode;

    if (!puProgramCounter) {
        return;
    }

    int32 iCallDepth = 1;
    initDisplacement();
    initMIPSReport();
    eStatus = RUNNING;
    while (RUNNING == eStatus) {

        // Fast branch back location for operations that don't change the status. If you invoke monadic() or dyadic()
        // you have to assume the status could change due to a bad EA mode.
        skip_status_check:

        updateMIPS();
        switch (*puProgramCounter++) {

            // Set up the required macros for the handler include
            #define end()       break
            #define status()    break
            #define next()      goto skip_status_check
            #define defOp(NAME) case Opcode::NAME:
            #include <machine/opcode_handlers.hpp>

            // Super undocumented timing opcode ftw
            case 0xF0: {
                aoGPR[14].uQuad = Nanoseconds::mark();
                next();
            }
            default:
                todo();
        }
    }

    outputMIPSReport();

}

}

