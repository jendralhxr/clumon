#!/usr/bin/gnuplot

reset
set terminal pngcairo
#set zzeroaxis
set zrange [-50:50]
set yrange [-100:100]
set grid
set view 80,0
set style line 2  lc rgb '#0025ad' # blue
do for [i=0:1800]{
    far = sprintf('deck%04i',i)
	near = sprintf('deck%04in',i)
	outfile = sprintf('deck%04i.png',i)
	set output outfile
	#splot infile with lines ls 1 title sprintf("framenum=%i",i)
	splot near with lines ls 1, far with lines ls 2
}
