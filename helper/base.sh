#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

trials=10
database="bit_count"
system="mysql"

while getopts 't:d:s:' flag; do
    case "${flag}" in
        t) trials=$OPTARG ;;
        d) database=$OPTARG ;;
        s) system=$OPTARG ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

case $system in
    'mysql')
        query="SET profiling = 1; SELECT ''; SHOW PROFILES;"
        for (( i=1; i<=$trials; i++ ))
        do
            echo $(mysql -u root -vvv $database -e "$query" | grep "| SELECT ''" | grep -Eo '[0-9][.][0-9]+')
        done
        ;;
    'postgres')
        for (( i=1; i<=$trials; i++ ))
        do
            echo "\\timing on \\\\ SELECT '';" | psql | grep "Time:" | sed s/"Time: "//
            # echo '\\timing on \\\\ SELECT "";' | psql | grep "Time:" | sed s/"Time: "//
        done
        ;;
esac
