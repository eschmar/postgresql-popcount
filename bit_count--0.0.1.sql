-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION bit_count" to load this file. \quit
CREATE FUNCTION bit_count(bit) RETURNS int
    AS '$libdir/bit_count'
LANGUAGE C IMMUTABLE STRICT;