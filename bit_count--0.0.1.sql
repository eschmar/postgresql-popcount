-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION tada" to load this file. \quit
CREATE FUNCTION bit_count_encode(integer) RETURNS int
    AS '$libdir/bit_count'
LANGUAGE C IMMUTABLE STRICT;