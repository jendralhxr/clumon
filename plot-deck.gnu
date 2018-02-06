#!/usr/bin/gnuplot

reset
#set terminal pngcairo
#set terminal epslatex color colortext
set terminal postscript eps enhanced color font 'Arial,24' linewidth 2
set zzeroaxis
set zrange [-50:50]
set yrange [-100:100]
set grid
#set view 0,0
set style line 2  lc rgb '#0025ad' # blue
do for [i=450:500]{
    displacement = sprintf('deck%04i',i)
    right = sprintf('deck%04in',i)
	outfile = sprintf('deck%04i.eps',i)
	set output outfile
	#splot displacement with lines ls 2 title sprintf("t=\t%i ms", (i-354)*1000/180)
	splot displacement with lines ls 2 title sprintf("t=%.1f ms    left side",(i-458)*10/2.4) , right with lines ls 1 title sprintf("right side")
}
