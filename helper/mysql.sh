#!/bin/bash
RED='\033[0;31m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m'

table="bit_count"
size=100

while getopts 'n:t:' flag; do
    case "${flag}" in
        n) size=$OPTARG ;;
        t) table=$OPTARG ;;
        *) error "Unexpected option ${flag}" ;;
    esac
done

table="${table}_${size}"

echo "
CREATE TABLE \`$table\` (
    \`id\` int(11) unsigned NOT NULL AUTO_INCREMENT,
    \`bit\` bit(64) DEFAULT NULL,
    PRIMARY KEY (\`id\`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
"

for (( i=1; i<=$size; i++ ))
do
    printf "INSERT INTO $table VALUES (NULL, $i);\n"
done
