#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

strategy='bit_count'
trials=10
bitlength=512
units='false'

while getopts 's:t:l:u' flag; do
    case "${flag}" in
        s) strategy=$OPTARG ;;
        t) trials=$OPTARG ;;
        l) bitlength=$OPTARG ;;
        u) units='true' ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

printf "> Strategy: ${MAGENTA}$strategy${NC}\n"
printf "> Bit length: ${MAGENTA}$bitlength${NC}\n"
printf "> Trials: ${MAGENTA}$trials${NC}\n\n"

for samples in 100 500 1000 2500 5000 7500 10000 25000 50000 75000 100000 250000 500000 750000 1000000
do
    printf "${CYAN}$samples${NC} "

    query="SELECT sum($strategy(x::bit($bitlength))) FROM generate_series(1,$samples) x;"
    count=0

    for (( i=1; i<=$trials; i++ ))
    do
        case $units in
            'true')
                temp=$(echo "\\\timing on \\\\\ $query" | psql | grep "Time:" | sed s/"Time: "//)
                ;;
            'false')
                temp=$(echo "\\\timing on \\\\\ $query" | psql | grep "Time:" | grep -Eo '[0-9]+([.][0-9]+)?')
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
