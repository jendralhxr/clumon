#!/usr/bin/gnuplot

set terminal pngcairo  enhanced font "arial,10" fontscale 1.0 size 600, 600 
set output ARG2
set border
set polar
set xzeroaxis
set yzeroaxis
set zzeroaxis

unset xtics
unset ytics
unset label

set rrange [ARG3:ARG4]
set rtics  mirror ARG3,ARG5,ARG4 

set grid polar 0.17452 # pi/18, 72 points
set rlabel "first mode freq (Hz)"

freq = ARG1
plot freq with lines ls 2	    

