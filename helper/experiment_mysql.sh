#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

trials=10

while getopts 't:' flag; do
    case "${flag}" in
        t) trials=$OPTARG ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

for samples in 100 500 1000 2500 5000 7500 10000 25000 50000 75000 100000 250000 500000 750000 1000000
do
    printf "$samples "

    query="SELECT SUM(count) FROM (SELECT BIT_COUNT(bit) as count FROM bit_count_1000000 WHERE id < $samples) as bits;"

    for (( i=1; i<=$trials; i++ ))
    do
        temp=$(mysql -u root -vvv temp -e "$query" | grep "row in set" | grep -Eo '\([0-9]+([.][0-9]+)?' | sed s/\(//)

        printf "$temp"
        if [ $i -lt $trials ]
        then
            printf " "
        fi
    done

    echo
done
