-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION bit_count" to load this file. \quit

/*
    Computes number of bits set in bit(n) data type.
*/
CREATE FUNCTION bit_count(bit) RETURNS int
    AS '$libdir/bit_count', 'bit_count'
LANGUAGE C immutable strict;

CREATE FUNCTION bit_count_32bit(bit) RETURNS int
    AS '$libdir/bit_count', 'bit_count_32bit'
LANGUAGE C immutable strict;

CREATE FUNCTION bit_count_64bit(bit) RETURNS int
    AS '$libdir/bit_count', 'bit_count_64bit'
LANGUAGE C immutable strict;
