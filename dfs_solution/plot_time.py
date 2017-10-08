#!/usr/bin/python3

'''''''''
Runs the program on 4*4, 8*8, 16*16, 32*32, 64*64, ... ,1024*1024, 2048*2048
and 4096*4096 lattice sizes 5 times for each to estimate the time difference
between the sequential and multithreaded solutions and plot the time difference
using gnuplot.
'''''''''

import sys;
import subprocess;
import pathlib;
import math;

#plots the generated graph..
def plot_graph():
    subprocess.call(['gnuplot', '--persist', '.plot_pref.gp'], shell=False)


# generate a .dat file containing the output from the percolation porgram.
# the output is then plotted using gnu-plot.
# to plot the graph use gnuplot:
# gnuplot '.plot_pref.gp' --persist
def generate_data(perc_type, prob):
        plotPref = open('.plot_pref.gp', 'w')
        plotDataSeq = open('.seq.gp', 'w')
        plotDataMt = open('.mt.gp', 'w')
        plotPref.write("set title 'CPU time vs. Lattice size for sequential and multithreaded solutions using Depth First Search.'\n")
        plotPref.write("set xlabel 'Lattice Size (n*n)'\n")
        plotPref.write("set ylabel 'CPU time (s)'\n")
        if(perc_type == 's'):
            plotPref.write("plot '.seq.gp' using 1:3 with lp title 'site percolation (sequential)', '.mt.gp' using 1:3 with lp title 'site percolation (multithreaded)'\n")
        else:
            plotPref.write("plot '.seq.gp' using 1:3 with lp title 'bond percolation (sequential)', '.mt.gp' using 1:3 with lp title 'bond percolation (multithreaded)'\n")
        plotPref.close()
        for i in range(4, 13):
            latsiz = int(math.pow(2, i))
            avg = 0.0
            avg_mt = 0.0
            for j in range(0, 5):
                exe_seq = "./percol -l "+ str(latsiz)+ " -p "  + str(prob) +" -" + str(perc_type)
                exe_mt =  "./percol -l "+ str(int(math.pow(2, i)))+ " -p "  + str(prob) +" -" + str(perc_type) +" -m"
                out =  str(subprocess.check_output(exe_seq, shell=True))
                out_mt = str(subprocess.check_output(exe_mt, shell=True))
                time = out.split('\\n')[-2]
                time_mt = out_mt.split('\\n')[-2]
                avg+=float(time)
                avg_mt+=float(time_mt)
            avg = (avg/5.0)
            avg_mt = (avg_mt/5.0)
            outstring_seq = str(latsiz) + " , " + str(avg)+"\n"
            outstring_mt = str(latsiz) + " , " +str(avg_mt)+"\n"
            plotDataSeq.write(outstring_seq)
            plotDataMt.write(outstring_mt)
        plotDataMt.close()
        plotDataSeq.close()
        plot_graph()


if __name__ == '__main__':
    exefile = pathlib.Path("./percol")
    if(not exefile.is_file()):
        sys.stderr.write("ERROR: exe not found.. Please build the project using the Makefile.\n")
        sys.exit(1);
    percol_type = input("Enter percolation type site or bond.. [s/b]?\n")
    if(not ((percol_type == "s") or (percol_type == "b"))):
        sys.stderr.write("ERROR: wrong input! must be s or b\n")
        sys.exit(1);
    if(percol_type == 's'):
        prob = input("Enter site probability...\n")
    else:
        prob = input("Enter bond probability...\n")
    generate_data(percol_type, prob)
