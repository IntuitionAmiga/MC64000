#!/bin/sh
gcc -Wall -W -Werror interpreter.cpp interpreter_ea.cpp interpreter_run.cpp dummy.cpp -o dummy
./dummy
rm dummy
