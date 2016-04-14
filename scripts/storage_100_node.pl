set terminal pdf
set output "prov_storage.pdf"

set title "100-switch topology with 20 communicating pairs"

set xlabel "Storage Size (MB)"
set ylabel "Percentage of nodes (%)"

set key right bottom
plot [0:30] "/home/chen/research/rapidnet-comp/scripts/storage_cdf_100_20_nocomp.dat" using ($1/1000000):2 title "no compression" with lines,\
     "/home/chen/research/rapidnet-comp/scripts/storage_cdf_100_20_online.dat" using ($1/1000000):2 title "online compression" with lines
