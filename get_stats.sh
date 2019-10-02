#!/bin/bash

clients=0
max=0
min=10000000
files=0
bytes=0

#metraw ta bytes kai ta arxeia
while read line
do
    b=$(echo $line | tr -dc '0-9')
    set -- $line
    bytes=$(($bytes+$5)) #pairnw ta bytes apo to arxeio
    files=$(($files+1))
done

#metraw ta logfles, ektupwnw id kai vriskw max-min
for f in ./log_file*
do
    clients=$((clients+1))
    id=$(echo $f | tr -dc '0-9')
    echo client id: $id

    if [ $id -lt $min ]
    then
        min=$id
    fi

    if  [ $id -gt $max ]
    then
        max=$id
    fi
done

#metraw tous clients sto common kai tous afairw apo tous sunolikous
common_clients=0
for cid in ./common/*.id
do
    common_clients=$(($common_clients+1))
done
gone=$(($clients-$common_clients))

echo clients: $clients
echo max id: $max min id: $min
echo number of files sent: $files
echo bytes sent: $bytes
echo clients that left: $gone
