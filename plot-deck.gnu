#!/usr/bin/gnuplot

reset
set terminal pngcairo
set zzeroaxis
set zrange [-50:50]
set yrange [-10:40]
set grid
system('mkdir -p animation')
do for [i=0:1999]{
  outfile = sprintf('animation160/deck%04i.png',i)
  infile = sprintf('plot160/time%04i.plt',i)
  set output outfile
  splot infile with lines title sprintf("framenum=%i",i)
}
