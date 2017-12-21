#!/usr/bin/gnuplot

reset
set terminal pngcairo
#set terminal epslatex color colortext
#set terminal postscript eps enhanced color font 'Arial,24' linewidth 2
set zzeroaxis
set zrange [-50:50]
set yrange [-100:100]
set grid
#set view 0,0
set style line 2  lc rgb '#0025ad' # blue
do for [i=0:1799]{
    displacement = sprintf('deck%04i',i)
 #  right = sprintf('deck%04in',i)
	outfile = sprintf('deck%04i.png',i)
	set output outfile
	splot displacement with lines ls 2 title sprintf("t=\t%i ms", (i-354)*1000/180)
	#splot displacement with lines ls 2 title sprintf("t=%i ms    displacement",(i-334)*1000/240) , right with lines ls 1
}
