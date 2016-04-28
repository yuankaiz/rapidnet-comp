set terminal pdf
set output "storage_growth_norm_100n_1000pk_3plots.pdf"

set title "Storage growth with the number of communicating pairs (100-node topology, 1000 packets)"

set xlabel "Number of nodes"
set ylabel "Average storage (MB)"

set key left top
plot "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_growth_nocomp_100n_1000pkts_10to90.dat" using 1:($2/1000000) title "no compression" with lines,\
     "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_growth_onlinenew_100n_1000pkts_10to90.dat" using 1:($2/1000000) title "online compression" with lines,\
     "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_growth_strawmannew_100n_1000pkts_10to90.dat" using 1:($2/1000000) title "strawman compression" with lines

