#!/bin/sh
gcc -Ofast -march=native -mavx -Wall -W -Werror interpreter.cpp interpreter_ea.cpp interpreter_run.cpp dummy.cpp -o dummy
./dummy
rm dummy
