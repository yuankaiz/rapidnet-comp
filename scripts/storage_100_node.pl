set terminal pdf
set output "storage_100n_10p_200pk_3plots.pdf"

set title "100-switch topology with 20 pairs of hosts, each sending 200 packets"

set xlabel "Storage Size (MB)"
set ylabel "Percentage of nodes (%)"

set key right bottom
plot [0:30] "/home/chen/research/rapidnet-comp/scripts/storage_cdf_100n_10p_200pk_nocomp.dat" using ($1/1000000):2 title "no compression" with lines,\
     "/home/chen/research/rapidnet-comp/scripts/storage_cdf_100n_10p_200pk_online.dat" using ($1/1000000):2 title "online compression" with lines,\
     "/home/chen/research/rapidnet-comp/scripts/storage_cdf_100n_10p_200pk_strawman.dat" using ($1/1000000):2 title "strawman compression" with lines
