#!/bin/bash
# Argument 1: name of the test file
# Argument 2: number of pairs
# Argument 3: number of packets
# Argument 4: the name of the file containing the final result
# Arguemtn 5: short name for the experiment

# Directory for serialized files
DIRECTORY=/localdrive1/chen/prov_storage/*
storeDir=/localdrive1/chen/prov_storage
mkdir -p $storeDir

# The file contains the final results, i.e.
# the storage sequences with elapsed time
outputFile=/localdrive1/chen/calc_prov_storage/$4

# Directory for storage snapshot
snapshotDirPfx=/localdrive1/chen/raw_prov_storage_data/
# Prefix for files for storage snapshot
snapshotPrefix="timed_storage_100n_"
snapshotSuffix=".dat"
snapshotFileName="$snapshotPrefix"$2"prs_"$3"pkts_"$5"$snapshotSuffix"

# A loop that runs experiments for each time interval

# Whenever the experiment finishes, do three things:

# - Compute the average storage based on the collected data
# - Output the computed data into a specific file for graphing

rm -f $outputFile
touch $outputFile
for exprTime in {50..500..50}
do
    rm -f $DIRECTORY
    # Run the experiment
    ./waf --run "$1 --hostPairs=$2 --packetNum=$3 --finishTime=$exprTime"
    # Record the storage data of all serialized files
    cd $storeDir
    allFileNames=$(ls *)
    for file in $allFileNames
    do
	fsize=$(stat --printf="%s" "$file")
	printf "%s %s \n" "$file" "$fsize" >> "$snapshotDir"	
    done
    fileNum=$count
    avg=$((sum/fileNum))
    printf "%s \t %s\n" "$pairNum" "$avg">> "$outputFile"
done
