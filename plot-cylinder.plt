#!/usr/bin/gnuplot

set terminal pngcairo enhanced font "arial,10" fontscale 1.0 size 600, 600
#set terminal epslatex color colortext
#set terminal postscript eps enhanced color font 'Arial,16' linewidth 2
set output ARG2

set mapping cylindrical
set zzeroaxis
set grid

set xrange [-1:1]
set yrange [-1:1]
set zrange [-.10:410]
set xlabel "x"
set ylabel "y"
set zlabel "z"

set view 60, 30, 1, 1
splot ARG1 with lines ls 2 title 'mode shape'

#set ticslevel 0.0
