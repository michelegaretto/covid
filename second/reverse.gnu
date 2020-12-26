reset
set pointsize 1
set size square

set term postscript landscape enhanced color  "Times-Roman" 14
#set term postscript eps enhanced "Times-Roman" 18
#set term X11
#set term png

#set title 'q-LRU - q = 0.001, cost(d) = d, d^* = 1000, lambda gaussian'

set xlabel 'time (days)' 
#set xrange [0:150]
#set yrange [0:0.2]
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

set y2label 'mobility (Apple)' 

set out 'reverse.ps'  
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

set ytics nomirror
set y2tics 
set ylabel "{/Symbol m}(t)"
set y2range [:180]
set yrange [:5]
set ytics 0.25
#set xrange[0:]
set xtics 30
set grid xtics


#set origin 0.01,-0.295
#set size 1.1,1.625
set key top right

# plot "reverse.res" index 0 u ($1-15):2 axes x1y1 t "reverse rho" w l lt 1 lw 2,\
 
plot "factor.res" index 0 u ($1-160):2 axes x1y1 t "{/Symbol m}(t) - 1st wave" w l lc "black" lw 4,\
     "reverse.res" index 0 u ($1-30):3 axes x1y1 t "reverse {/Symbol m}(t) - 2nd wave" w l lc "blue" lw 4,\
     "mobility_apple.res" index 0 u($1-201):(($2+$3+$4)/3) axes x1y2 t "average mobility - Apple maps" w l lc "orange" lw 2


#     "mobility_apple.res" index 0 u($1-201):3 axes x1y1 t "transit" w l lt 4 lw 3,\
#     "mobility_apple.res" index 0 u($1-201):4 axes x1y1 t "walking" w l lt 5 lw 3


