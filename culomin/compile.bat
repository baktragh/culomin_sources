@ECHO OFF
SET PATH=C:\UTILS\A8\CC65\BIN
SET CC65_HOME=C:\UTILS\A8\CC65

REM Compile and assemble main program
cc65 -O -t atari --all-cdecl -o build/main.s main.c
ca65 -t atari -o build/main.o build/main.s

REM Assemble native routines in assembler
ca65 -t atari -o build/routines.o routines.asm

REM Assemble game data
ca65 -t atari -o build/gamedata.o gamedata.asm

REM Link binary file
ld65 -C linker.cfg --mapfile map.txt -o culomin.xex build/main.o build/gamedata.o build/routines.o atari.lib

PAUSE
