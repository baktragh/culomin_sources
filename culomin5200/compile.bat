@ECHO OFF

SET CC65_HOME=C:\utils\a8\cc65
SET PATH=C:\utils\a8\cc65\bin

cc65 -O -t atari5200  -o build/clm1.s main.c 
cc65 -O -t atari5200  -o build/clm1_alt.s main_alt.c


ca65 -t atari5200 -o build/clm1.o build/clm1.s
ca65 -t atari5200 -o build/clm1_alt.o build/clm1_alt.s


echo "Assembling the supplementary routines ..."
ca65 -t atari5200 -o build/clm_sup.o rmt_sup.asm

echo "Assembling data..."
ca65 -t atari5200 -o build/data.o data.s


ld65 -C linker.cfg  -o culomin5200.rom build/clm_sup.o build/clm1.o build/data.o atari5200.lib
ld65 -C linker.cfg  -o culomin5200_alt.rom build/clm_sup.o build/clm1_alt.o build/data.o atari5200.lib

PAUSE