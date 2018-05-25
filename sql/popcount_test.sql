CREATE EXTENSION popcount;

SELECT popcount(15::bit(4));
SELECT popcount(7::bit(4));

SELECT popcount(15::bit(8));
SELECT popcount(7::bit(8));

SELECT popcount(15::bit(12));
SELECT popcount(7::bit(12));

SELECT popcount(15::bit(32));
SELECT popcount(7::bit(32));

SELECT popcount(30::bit(32));
SELECT popcount(127::bit(32));

SELECT sum(popcount(x::bit(512))) from generate_series(1,1000000) x;
SELECT sum(popcount(x::bit(500))) from generate_series(1,1000000) x;
