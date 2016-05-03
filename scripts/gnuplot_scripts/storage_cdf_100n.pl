set terminal pdf
set output "storage_cdf_norm_100n_1000pr_100pk_3plots.pdf"

set title "100-switch topology with 1000 pairs of hosts, each sending 100 packets"

set xlabel "Storage Size (MB)"
set ylabel "Percentage of nodes (%)"
set logscale x 10

set key right bottom
plot "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_cdf_nocompnew_100n_1000pr_100pkt.dat" using ($1/1000000):2 title "no compression" with lines,\
    "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_cdf_onlinenew_100n_1000pr_100pkt.dat" using ($1/1000000):2 title "online compression" with lines,\
     "/home/chen/research/rapidnet-comp/expr_data/prov_compress/storage_cdf_strawmannew_100n_1000pr_100pkt.dat" using ($1/1000000):2 title "strawman compression" with lines
