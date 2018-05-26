#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

base="bit_count"
database="bit_count"
alignment=64
samples="100 500 1000 2500 5000 7500 10000 25000 50000 75000 100000 250000 500000 750000 1000000"

while getopts 'd:a:s:' flag; do
    case "${flag}" in
        d) database=$OPTARG ;;
        a) alignment=$OPTARG ;;
        s) samples=$OPTARG ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

read -a arr <<< "$samples"

for sample in "${arr[@]}"
do
    printf "$sample "
    table="${base}_${alignment}_$(printf %07d $sample)"

    # psql -U postgres -d database_name -c 
    psql -q -c "DROP TABLE IF EXISTS \"$table\";"
    psql -q -c "CREATE TABLE \"$table\" (\"id\" SERIAL, \"bit\" bit($alignment), PRIMARY KEY (\"id\"));"
    psql -q -c "TRUNCATE TABLE \"$table\";"

    values=''
    for (( j=0; j<($sample/100); j++))
    do
        values=''
        for (( i=1; i<=100; i++ ))
        do
            values="$values($((j*100+i))::bit($alignment))"
            if [[ $i -ne 100 ]]; then values="$values," ; fi
        done

        # insert here
        psql -q -c "INSERT INTO $table (bit) VALUES $values;"
    done
done
