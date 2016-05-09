set terminal pdf
set output "storage_growth_100n_1000pkts_100to1000_3plots.pdf"

set title "Storage growth with the number of communicating pairs (100-node topology, 1000 packets)"

set xlabel "Number of node pairs"
set ylabel "Average storage (MB)"

set key left top
plot "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_growth_nocomp_100n_1000pkts_100to1000.dat" using 1:($2/1000000) title "no compression" with lines,\
     "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_growth_onlinenew_100n_1000pkts_100to1000.dat" using 1:($2/1000000) title "online compression" with lines,\
     "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_growth_strawman_100n_1000pkts_100to1000.dat" using 1:($2/1000000) title "strawman compression" with lines
