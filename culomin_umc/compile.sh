#!/bin/sh

#Compile and assemble main program
cc65 -O2 -t atari --all-cdecl -o build/main.s main.c
ca65 -t atari -o build/main.o build/main.s

#Assemble native routines in assembler
ca65 -t atari -o build/routines.o routines.asm

#Assemble game data
ca65 -t atari -o build/gamedata.o gamedata.asm

#Link binary file
ld65 -C linker.cfg --mapfile map.txt -o culomin_umc.xex build/main.o build/gamedata.o build/routines.o atari.lib

