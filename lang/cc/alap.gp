set term eps
#set term postscript eps enhanced
set output "_akt.eps"
set title "Barabasi | m=_m | lepes=_lepes | degree"
#set key off
set logscale
set format "10^{%L}"
set linestyle 1 lt 1 lw 1 lc 2
#set linestyle 1 lt 1 lw 1
set linestyle 2 lt 2 lw 1

plot "_akt" using 1:2 pt 12 ps 0.2 title "simulation", \
"_akt" using 1:3 w l ls 2 title "recursion"