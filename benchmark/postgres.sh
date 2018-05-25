#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

strategy='popcount'
base='bit_count'
trials=50
units='false'
color='false'
alignment=64

while getopts 's:t:a:uc' flag; do
    case "${flag}" in
        s) strategy=$OPTARG ;;
        t) trials=$OPTARG ;;
        a) alignment=$OPTARG ;;
        u) units='true' ;;
        c) color='true' ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

case $color in
    'true')
        printf "> Strategy: ${MAGENTA}$strategy${NC}\n"
        printf "> Bit length: ${MAGENTA}$alignment${NC}\n"
        printf "> Trials: ${MAGENTA}$trials${NC}\n\n"
        ;;
    'false')
        printf "> Strategy: $strategy\n"
        printf "> Bit length: $alignment\n"
        printf "> Trials: $trials\n\n"
        ;;
esac

for samples in 100 500 1000 2500 5000 7500 10000 25000 50000 75000 100000 250000 500000 750000 1000000
do
    case $color in
        'true')
            printf "${CYAN}$samples${NC} "
            ;;
        'false')
            printf "$samples "
            ;;
    esac

    table="${base}_${alignment}_$(printf %07d $samples)"
    query="SELECT sum(count) FROM (SELECT $strategy(bit) as count FROM $table WHERE True) as bits;"

    for (( i=1; i<=$trials; i++ ))
    do
        case $units in
            'true')
                temp=$(echo "\\timing on \\\\ $query" | psql | grep "Time:" | sed s/"Time: "//)
                ;;
            'false')
                temp=$(echo "\\timing on \\\\ $query" | psql | grep "Time:" | grep -Eo -m 1 '[0-9]+([.][0-9]+)?' | head -1)
                ;;
        esac

        printf "$temp"
        if [ $i -lt $trials ]
        then
            printf " "
        fi
    done

    echo
done