#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

strategy='bit_count'
base='bit_count'
trials=10
bitlength=512
units='false'
color='false'

while getopts 's:t:l:uc' flag; do
    case "${flag}" in
        s) strategy=$OPTARG ;;
        t) trials=$OPTARG ;;
        l) bitlength=$OPTARG ;;
        u) units='true' ;;
        c) color='true' ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

case $color in
    'true')
        printf "> Strategy: ${MAGENTA}$strategy${NC}\n"
        printf "> Bit length: ${MAGENTA}$bitlength${NC}\n"
        printf "> Trials: ${MAGENTA}$trials${NC}\n\n"
        ;;
    'false')
        printf "> Strategy: $strategy\n"
        printf "> Bit length: $bitlength\n"
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

    table="${base}_${samples}"

    # psql -U postgres -d database_name -c 
    psql -q -c "DROP TABLE IF EXISTS \"$table\";"
    psql -q -c "CREATE TABLE \"$table\" (\"id\" SERIAL, \"bit\" bit($samples), PRIMARY KEY (\"id\"));"
    psql -q -c "TRUNCATE TABLE \"$table\";"

    for (( i=1; i<=$samples; i++ ))
    do
        psql -q -c "INSERT INTO $table (bit) VALUES ($i::bit($samples));"
    done

    query="SELECT sum($strategy(bit)) FROM $table WHERE True;"
    count=0

    for (( i=1; i<=$trials; i++ ))
    do
        case $units in
            'true')
                temp=$(echo "\\timing on \\\\ $query" | psql | grep "Time:" | sed s/"Time: "//)
                ;;
            'false')
                temp=$(echo "\\timing on \\\\ $query" | psql | grep "Time:" | grep -Eo '[0-9]+([.][0-9]+)?')
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
