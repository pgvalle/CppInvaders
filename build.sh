#!/bin/bash

mkdir -p deps out

command_exists() {
    if ! command -v "$1" >/dev/null 2>&1; then
        echo "$1 is not installed. Please install it and try again."
        exit 1
    fi
}

command_exists git

if [ ! -d "deps/pico-sdl" ]; then
    git clone https://github.com/fsantanna/pico-sdl deps/pico-sdl
fi

CFLAGS=`pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_gfx SDL2_mixer`
LIBS=`pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_gfx SDL2_mixer`

cd out

command_exists gcc
command_exists g++

pico_src_files=`find ../deps/pico-sdl/src -type f -name "*.c"`
gcc -c $pico_src_files $CFLAGS

src_files=`find ../src/ -type f -name "*.cpp"`
g++ -std=c++11 -c $src_files -I../deps/pico-sdl/src $CFLAGS

obj_files=`find ./ -type f -name "*.o"`
g++ -o ../a.out $obj_files $LIBS