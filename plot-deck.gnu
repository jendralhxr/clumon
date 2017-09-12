#!/usr/bin/gnuplot

reset
set terminal pngcairo
set zzeroaxis
set zrange [-50:50]
set yrange [-10:40]
set grid
do for [i=0:1500]{
    infile = sprintf('deck%04i',i)
	outfile = sprintf('deck%04i.png',i)
	set output outfile
	splot infile with lines title sprintf("framenum=%i",i)
}
