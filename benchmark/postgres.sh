#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

strategy='popcount'
base='bit_count'
trials=50
domain=500000
alignments="2048 4096 8192 15040 16384 32768 65536 131072"
units='false'

while getopts 's:t:a:x:uc' flag; do
    case "${flag}" in
        s) strategy=$OPTARG ;;
        t) trials=$OPTARG ;;
        d) domain=$OPTARG ;;
        a) alignments=$OPTARG ;;
        u) units='true' ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

printf "> Strategy: ${MAGENTA}$strategy${NC}\n"
printf "> Bit length: ${MAGENTA}$alignments${NC}\n"
printf "> Trials: ${MAGENTA}$trials${NC}\n\n"

read -a arr <<< "$alignments"

for alignment in "${arr[@]}"
do
    printf "${CYAN}$alignment${NC} "

    table="${base}_${alignment}_$(printf %07d $domain)"
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
