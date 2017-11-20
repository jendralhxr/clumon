#!/usr/bin/gnuplot

reset
#set terminal pngcairo
#set terminal epslatex color colortext
set terminal postscript eps enhanced color font 'Times,24' linewidth 2
set zzeroaxis
set zrange [-50:50]
set yrange [-100:100]
set grid
#set view 0,0
set style line 2  lc rgb '#0025ad' # blue
do for [i=0:2399]{
    far = sprintf('deck%04i',i)
    near = sprintf('deck%04in',i)
	outfile = sprintf('deck%04i.eps',i)
	set output outfile
	#splot far with lines ls 2 title sprintf("framenum=%i",i)
	splot far with lines ls 2 title sprintf("framenum=%i     far",i) , near with lines ls 1
}
