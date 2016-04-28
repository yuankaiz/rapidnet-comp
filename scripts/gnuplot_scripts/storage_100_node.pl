set terminal pdf
set output "storage_norm_100n_10p_200pk_3plots.pdf"

set title "100-switch topology with 10 pairs of hosts, each sending 1000 packets"

set xlabel "Storage Size (MB)"
set ylabel "Percentage of nodes (%)"

set key right bottom
plot "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_cdf_norm_100n_10p_1000pk_nocomp.dat" using ($1/1000000):2 title "no compression" with lines,\
    "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_cdf_norm_100n_10p_1000pk_online.dat" using ($1/1000000):2 title "online compression" with lines,\
     "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_cdf_norm_100n_10p_1000pk_strawman.dat" using ($1/1000000):2 title "strawman compression" with lines
