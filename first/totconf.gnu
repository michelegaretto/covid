reset
set pointsize 1
#set size square

set term postscript landscape enhanced color  "Times-Roman" 14
#set term postscript eps enhanced "Times-Roman" 18
#set term X11
#set term png

#set title 'q-LRU - q = 0.001, cost(d) = d, d^* = 1000, lambda gaussian'

set xlabel 'time (days)' 
set xrange [:120]
set yrange [0:]
set xtics 
set ytics 
#set grid ytics
#set grid xtics

#set logscale x
#set grid xtics
#set mxtics 5
#set grid mxtics
#set grid xtics
#set logscale x
#set xtics .5,.5,2
#set xtics 1
#set xtics nomirror
#set ytics nomirror

set ylabel 'number of cases' 
#set yrange [0:200000]
#set y2range [0:10000]
#set grid ytics
#set mxtics 5
#set grid mytics
#set grid ytics
#set ytics 0.25
#set logscale y

#set zrange [6e-5:0.22]
#set label 'e({/Symbol l}/D)' at 0.1,0.0,-0.47
#set grid ytics
#set ytics 0.1
#set mytics 10
#set logscale z
#set xtics 1
#set ytics 0.1
#set zrange [-2.09:-0.49]
#set ztics 0.1

#set surface
#set pm3d; set palette
#set palette gray
#set pm3d map
#set dgrid3d
#set hidden3d

#set zrange [-2:0.0001]
#set ztics 0.1
#set samples 100
#set isosamples 100
#set log cb
#set palette gamma 0.9
#set ticslevel 0
#set view 0,0,1,1
#unset colorbox
#unset clabel 
#unset xtics
#unset border
#unset ytics
#unset ztics

set out 'totconf.ps'  
#set key 0.35,0.5,-0.8
#set key top right

#unset ztics
#unset colorbox

#set surface
#set pm3d
#set palette gray
#set hidden3d
#set view 0,0,1,1
#set ticslevel -1

#set multiplot

#unset border
#unset xtics
#unset ytics

set ytics 1e5
set xtics 10
set yrange [:1.6e6]

set ytics nomirror
set y2tics 
set y2label "daily variation"

#set origin 0.01,-0.295
#set size 1.1,1.625
set key at 110,800000


plot "multi.res" index 0 u 1:5:6 axes x1y1 t "" w filledcurves lc "skyblue" fs transparent solid 0.25,\
     "multi.res" index 1 u 1:5:6 axes x1y2 t "" w filledcurves lc "green" fs transparent solid 0.25,\
     "multi.res" index 0 u 1:2 axes x1y1 t "total cases (model)" w l lc "skyblue" lw 4,\
     "multi.res" index 1 u 1:2 axes x1y2 t "daily variation (model)" w l lc "green" lw 4




