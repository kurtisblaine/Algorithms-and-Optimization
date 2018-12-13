reset
set title "Fibrecur Time(x)" font ",25" 
set xlabel "N, 64 bit integers" font ",15"
set ylabel "Time in nanoseconds" font ",15"
set xtics font ",15"
set xtics font ",15"
set grid
set term png size 1200,800

f(x) = A*x***3
fit f(x) 'fibrecur.dat' via A
set logscale y .05
set logscale x 10
plot 'fibloop.txt' with linespoints

