#!/bin/sh
rm bin/interpreter
g++ -std=c++17 -Iinclude -Ofast -march=native -mavx -Wall -W -Werror interpreter.cpp interpreter_ea.cpp interpreter_run.cpp loader.cpp main.cpp -o bin/interpreter
strip bin/interpreter
ls -l bin/interpreter
./bin/interpreter ../../../assembler/test_projects/calltest/bin/calltest.mc64k

