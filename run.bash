#!/bin/bash
build_and_run()
{
gcc -lSDL2 -lSDL2_mixer -Wall $1 main.c -o boodeelneecheck && ./boodeelneecheck
}


if [ "$1" == "d" ]
then
    build_and_run "-D_DEBUG"
else
    build_and_run
fi
