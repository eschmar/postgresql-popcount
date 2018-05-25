CREATE EXTENSION bit_count;

SELECT bit_count_64bit(15::bit(4));
SELECT bit_count_64bit(7::bit(4));

SELECT bit_count_64bit(15::bit(8));
SELECT bit_count_64bit(7::bit(8));

SELECT bit_count_64bit(15::bit(12));
SELECT bit_count_64bit(7::bit(12));

SELECT bit_count_64bit(15::bit(32));
SELECT bit_count_64bit(7::bit(32));

SELECT bit_count_64bit(30::bit(32));
SELECT bit_count_64bit(127::bit(32));

SELECT sum(bit_count_64bit(x::bit(512))) from generate_series(1,1000000) x;
SELECT sum(bit_count_64bit(x::bit(500))) from generate_series(1,1000000) x;
