#!/bin/sh
rm dummy
g++ -std=c++17 -Iinclude -Ofast -march=native -mavx -Wall -W -Werror interpreter.cpp interpreter_ea.cpp interpreter_run.cpp loader.cpp dummy.cpp -o dummy
strip dummy
ls -l dummy
./dummy ../../../assembler/test_projects/calltest/bin/calltest.mc64k

