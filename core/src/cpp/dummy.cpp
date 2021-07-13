#include <cstdio>
#include "include/mc64k.hpp"


int main() {

    MC64K::Machine::StaticInterpreter::gpr(0).uByte = 1;

    MC64K::Machine::StaticInterpreter::fpr(0).fSingle = 3.141f;


    MC64K::Machine::StaticInterpreter::dumpState();

    return 0;
}
