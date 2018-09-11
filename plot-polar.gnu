set terminal pngcairo  enhanced font "arial,10" fontscale 1.0 size 600, 600 
set output 'polar2.png'
set border
set polar
set xzeroaxis
set yzeroaxis
set zzeroaxis

unset xtics
unset ytics
unset label

set rtics  mirror (8, 8.5, 9, 9.5, 10)
set grid polar 0.17452 # pi/18, 72 points
set rrange [8:10.5]
set rlabel "first mode freq (Hz)"

freq = 'data.txt'
plot freq with lines ls 2	    

