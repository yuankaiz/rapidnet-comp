set terminal pdf
set output "storage_growth_100n_500pr_100to1000pkts.pdf"

set title "Storage growth with increasing number of packets (100-node topology, 500 pairs)"

set xlabel "Number of packets"
set ylabel "Average storage (MB)"

set key left top
plot "/home/chen/research/rapidnet-comp/expr_data/prov_compress/analysis_data/storage_growth_nocomp_100n_500pr_100to1000pkts.dat" using 1:($2/1000000) title "no compression" with lines,\
     "/home/chen/research/rapidnet-comp/expr_data/prov_compress/analysis_data/storage_growth_online_100n_500pr_100to1000pkts.dat" using 1:($2/1000000) title "online compression" with lines,\
     "/home/chen/research/rapidnet-comp/expr_data/prov_compress/analysis_data/storage_growth_strawman_100n_500pr_100to1000pkts.dat" using 1:($2/1000000) title "strawman compression" with lines

