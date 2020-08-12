#!/bin/sh

#Locate CC65
export CC65_HOME=/usr/share/cc65

echo "Compiling mainline code ..."
cc65 -O2 -t atari5200  -o build/clm1.s main.c 
cc65 -O2 -t atari5200  -DALTERNATE_CONTROLS -o build/clm1_alt.s main.c

#if failed, then terminate processing
if [ $? -ne 0 ]
then
  exit 1
fi

echo "Assembling mainline code ..."
ca65 -t atari5200 -o build/clm1.o build/clm1.s
ca65 -t atari5200 -o build/clm1_alt.o build/clm1_alt.s

#if failed, then terminate processing
if [ $? -ne 0 ]
then
  exit 1
fi

echo "Assembling the supplementary routines ..."
ca65 -t atari5200 -o build/clm_sup.o rmt_sup.asm

#if failed, then terminate processing
if [ $? -ne 0 ]
then
  exit 1
fi

echo "Assembling data..."
ca65 -t atari5200 -o build/data.o data.s
#if failed, then terminate processing
if [ $? -ne 0 ]
then
  exit 1
fi


echo "Linking cartridge image"
ld65 -C linker.cfg --mapfile map.txt -o culomin_umc_5200.bin build/clm_sup.o build/clm1.o build/data.o atari5200.lib
ld65 -C linker.cfg --mapfile map_alt.txt -o culomin_umc_5200_alt.bin build/clm_sup.o build/clm1_alt.o build/data.o atari5200.lib

#if failed, then terminate processing
if [ $? -ne 0 ]
then
  exit 1
fi