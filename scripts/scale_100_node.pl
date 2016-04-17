set terminal pdf
set output "scale_norm_100n_1000pk_3plots.pdf"

set title "Storage growth with the number of communicating pairs (100-node topology, 1000 packets)"

set xlabel "Number of nodes"
set ylabel "Average storage (MB)"

set key left top
plot "/home/chen/research/rapidnet-comp/scripts/scale_100node_1000pk_storage_nocomp.dat" using 2:($1/1000000) title "no compression" with lines,\
     "/home/chen/research/rapidnet-comp/scripts/scale_100node_1000pk_storage_online.dat" using 2:($1/1000000) title "online compression" with lines,\
     "/home/chen/research/rapidnet-comp/scripts/scale_100node_1000pk_storage_strawman.dat" using 2:($1/1000000) title "strawman compression" with lines
