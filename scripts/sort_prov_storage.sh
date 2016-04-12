#!/bin/bash

DIRECTORY="/home/chen/research/rapidnet-comp/examples/*"
count=0

# Collect sizes of all files
for file in $DIRECTORY
do
    fsize=$(stat --printf="%s" "$file")
    fileSizes[$count]=$fsize
    ((count+=1))
done
fileNum=$count

# Sort the size array
sortRes=( $(printf "%s\n" "${fileSizes[@]}" | sort -g))

# Create the cumulative result
outputFile="storage_cdf.dat"
for ((i=0;i<fileNum;i++))
do
    fileIdx=$((i+1))
    percentage=$(($fileIdx*100/$fileNum))
    printf "%s %s \n" "${sortRes[$i]}" "$percentage" >> "$outputFile"
done