-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION popcount" to load this file. \quit

/*
    Computes number of bits set in bit(n) data types.
*/
CREATE FUNCTION popcount(bit) RETURNS int
    AS '$libdir/popcount', 'popcount'
LANGUAGE C immutable strict;

CREATE FUNCTION popcount32(bit) RETURNS int
    AS '$libdir/popcount', 'popcount32'
LANGUAGE C immutable strict;

CREATE FUNCTION popcount64(bit) RETURNS int
    AS '$libdir/popcount', 'popcount64'
LANGUAGE C immutable strict;

CREATE FUNCTION popcountAsm(bit) RETURNS int
    AS '$libdir/popcount', 'popcountAsm'
LANGUAGE C immutable strict;

CREATE FUNCTION popcount256(bit) RETURNS int
    AS '$libdir/popcount', 'popcount256'
LANGUAGE C immutable strict;
