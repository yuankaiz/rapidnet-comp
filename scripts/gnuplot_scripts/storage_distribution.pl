set terminal pdf
set output "storage_distribution.pdf"
set key right top
set xlabel "Node ID"
set ylabel "Storage Size(bytes)"
plot "storage_distribution.dat" with errorbars
