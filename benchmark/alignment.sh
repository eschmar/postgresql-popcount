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
size=500000

while getopts 's:t:a:uc' flag; do
    case "${flag}" in
        s) strategy=$OPTARG ;;
        t) trials=$OPTARG ;;
        a) size=$OPTARG ;;
        u) units='true' ;;
        c) color='true' ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

case $color in
    'true')
        printf "> Strategy: ${MAGENTA}$strategy${NC}\n"
        printf "> Bit length: ${MAGENTA}$size${NC}\n"
        printf "> Trials: ${MAGENTA}$trials${NC}\n\n"
        ;;
    'false')
        printf "> Strategy: $strategy\n"
        printf "> Bit length: $size\n"
        printf "> Trials: $trials\n\n"
        ;;
esac

for alignments in 2048 4096 8192 14016 15040 15680 16000 16352 16384 32768 65536 131072
do
    case $color in
        'true')
            printf "${CYAN}$alignments${NC} "
            ;;
        'false')
            printf "$alignments "
            ;;
    esac

    table="${base}_${alignments}_$(printf %07d $size)"
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
