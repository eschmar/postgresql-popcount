CREATE EXTENSION popcount;

SELECT popcount64(15::bit(4));
SELECT popcount64(7::bit(4));

SELECT popcount64(15::bit(8));
SELECT popcount64(7::bit(8));

SELECT popcount64(15::bit(12));
SELECT popcount64(7::bit(12));

SELECT popcount64(15::bit(32));
SELECT popcount64(7::bit(32));

SELECT popcount64(30::bit(32));
SELECT popcount64(127::bit(32));

SELECT sum(popcount64(x::bit(512))) from generate_series(1,1000000) x;
SELECT sum(popcount64(x::bit(500))) from generate_series(1,1000000) x;
