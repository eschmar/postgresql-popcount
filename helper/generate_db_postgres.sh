#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

base="bit_count"
database="bit_count"

domain=500000
alignments="2048 4096 8192 15040 16384 32768 65536 131072"

while getopts 'd:a:s:' flag; do
    case "${flag}" in
        d) domain=$OPTARG ;;
        a) alignments=$OPTARG ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

read -a arr <<< "$alignments"

for alignment in "${arr[@]}"
do
    table="${base}_${alignment}_$(printf %07d $domain)"
    printf "> Generating table: ${MAGENTA}$table${NC}\n"

    # psql -U postgres -d database_name -c 
    psql -q -c "DROP TABLE IF EXISTS \"$table\";"
    psql -q -c "CREATE TABLE \"$table\" (\"id\" SERIAL, \"bit\" bit($alignment), PRIMARY KEY (\"id\"));"
    psql -q -c "TRUNCATE TABLE \"$table\";"

    hexLength=$(($alignment/8))
    batchCount=20

    for (( j=0; j<($domain/20); j++))
    do
        echo -ne ">>> Adding rows ($(($j * $batchCount))/$domain).\r"

        sql=''
        for (( i=1; i<=$batchCount; i++ ))
        do
            randomhex=$(openssl rand -hex $hexLength)
            sql="$sql(x'$randomhex'::bit($alignment))"
            if [[ $i -ne $batchCount ]]; then sql="$sql," ; fi
        done

        # insert here
        psql -q -c "INSERT INTO $table (bit) VALUES $sql;"
    done

    echo -ne "\n"
done
