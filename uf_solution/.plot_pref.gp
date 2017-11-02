set title 'CPU time vs. Lattice size for sequential and multithreaded solutions using Union Find.'
set xlabel 'Lattice Size (n*n)'
set ylabel 'CPU time (s)'
plot '.seq.gp' using 1:3 with lp title 'site percolation (sequential)', '.mt.gp' using 1:3 with lp title 'site percolation (multithreaded)'
