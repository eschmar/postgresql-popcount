#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

base="bit_count"
database="bit_count"
samples="100 500 1000 2500 5000 7500 10000 25000 50000 75000 100000 250000 500000 750000 1000000"

while getopts 'd:s:' flag; do
    case "${flag}" in
        d) database=$OPTARG ;;
        s) samples=$OPTARG ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

read -a arr <<< "$samples"

for sample in "${arr[@]}"
do
    printf "$sample "
    table="${base}_$(printf %07d $sample)"

    mysql -u root "$database" -e "DROP TABLE IF EXISTS \`$table\`;"
    mysql -u root "$database" -e "CREATE TABLE \`$table\` (\`id\` int(11) unsigned NOT NULL AUTO_INCREMENT, \`bit\` bit(64) DEFAULT NULL, PRIMARY KEY (\`id\`)) ENGINE=InnoDB AUTO_INCREMENT=1000001 DEFAULT CHARSET=utf8;"
    mysql -u root "$database" -e "TRUNCATE TABLE \`$table\`;"

    values=''
    for (( j=0; j<($sample/100); j++))
    do
        values=''
        for (( i=1; i<=100; i++ ))
        do
            values="$values(NULL, $((j*100+i)))"
            if [[ $i -ne 100 ]]; then values="$values," ; fi
        done

        # insert here
        mysql -u root "$database" -e "INSERT INTO $table VALUES $values;"
    done
done
