set title 'Site percolation : percolation probability vs. % of percolations'
set xlabel 'site probability'
set ylabel 'percolation found (%)'
plot '.phase_transition.gp' using 1:3 with lp title 'phase transition'
