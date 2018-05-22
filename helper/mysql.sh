#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

trials=50
base="bit_count"
database="bit_count"
drop='true'

while getopts 't:d:x' flag; do
    case "${flag}" in
        t) trials=$OPTARG ;;
        d) database=$OPTARG ;;
        x) drop='false' ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

for samples in 100 500 1000 2500 5000 7500 10000 25000 50000 75000 100000 250000 500000 750000 1000000
do
    printf "$samples "

    table="${base}_$(printf %07d $samples)"
    query="SET profiling = 1; SELECT SUM(count) FROM (SELECT BIT_COUNT(bit) as count FROM $table WHERE 1) as bits; SHOW PROFILES;"

    for (( i=1; i<=$trials; i++ ))
    do
        temp=$(mysql -u root -vvv $database -e "$query" | grep "| SELECT SUM(count)" | grep -Eo '[0-9][.][0-9]+')

        printf "$temp"
        if [ $i -lt $trials ]
        then
            printf " "
        fi
    done

    case $drop in
        'true')
            mysql -u root $database -e "DROP TABLE \`$table\`;"
            ;;
    esac

    echo
done
