CREATE EXTENSION popcount;

SELECT popcount32(15::bit(4));
SELECT popcount32(7::bit(4));

SELECT popcount32(15::bit(8));
SELECT popcount32(7::bit(8));

SELECT popcount32(15::bit(12));
SELECT popcount32(7::bit(12));

SELECT popcount32(15::bit(32));
SELECT popcount32(7::bit(32));

SELECT popcount32(30::bit(32));
SELECT popcount32(127::bit(32));

SELECT sum(popcount32(x::bit(512))) from generate_series(1,1000000) x;
SELECT sum(popcount32(x::bit(500))) from generate_series(1,1000000) x;
