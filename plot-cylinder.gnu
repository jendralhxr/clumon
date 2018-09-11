#!/usr/bin/gnuplot
set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 600
#set terminal epslatex color colortext
#set terminal postscript eps enhanced color font 'Arial,16' linewidth 2
set output 'mode.png'

set mapping cylindrical
set zzeroaxis
set grid

#set xrange [-10:410]
#set zrange [-40:40]
#set yrange [-80:80]
set xlabel "x (cm)"
set ylabel "y (cm)"
set zlabel "z (cm)"

modes = 'modes.txt'
splot modes with lines ls 2

#set ticslevel 0.0
#set view 30,56,0.98
