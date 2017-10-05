set title 'Bond percolation : percolation probability vs. % of percolations'
set xlabel 'bond probability'
set ylabel 'percolation found (%)'
plot '.phase_transition.gp' using 1:3 with lp
