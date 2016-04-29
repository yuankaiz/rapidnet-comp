set terminal pdf
set output "bandwidth_usage_norm_100n_100pr_1000pk_3plots.pdf"

set title "Bandwidth usage (100 pairs of nodes, 1000 packets per pair, in a 100-node topology)"

set xlabel "Number of nodes"
set ylabel "Average Bandwidth"

set key right top
plot "/home/chen/research/rapidnet-comp/scripts/bandwidth_100n_nocomp_lscale.pl" using 1:2 title "no compression" with lines,\
     "/home/chen/research/rapidnet-comp/scripts/bandwidth_100n_online_lscale.pl" using 1:2 title "online compression" with lines,\
     "/home/chen/research/rapidnet-comp/scripts/bandwidth_100n_strawman_lscale.pl" using 1:2 title "strawman compression" with lines
