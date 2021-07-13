#!/bin/sh
gcc -Wall -W -Werror interpreter.cpp dummy.cpp -o dummy
./dummy
rm dummy
