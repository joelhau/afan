#!/bin/sh


echo "Compiling Afån Meny..."

vc +aos68k -c99 meny.c ikoner.c grafik.c skarm.c -o meny

if [ $? -eq 0 ]; then
    echo "OK - afoncli compiled!"
else
    echo "ERROR - compilation failed!"
    exit 1
fi
