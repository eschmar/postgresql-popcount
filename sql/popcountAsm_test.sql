CREATE EXTENSION popcount;

SELECT popcountAsm(15::bit(4));
SELECT popcountAsm(7::bit(4));

SELECT popcountAsm(15::bit(8));
SELECT popcountAsm(7::bit(8));

SELECT popcountAsm(15::bit(12));
SELECT popcountAsm(7::bit(12));

SELECT popcountAsm(15::bit(32));
SELECT popcountAsm(7::bit(32));

SELECT popcountAsm(30::bit(32));
SELECT popcountAsm(127::bit(32));

SELECT sum(popcountAsm(x::bit(512))) from generate_series(1,1000000) x;
SELECT sum(popcountAsm(x::bit(500))) from generate_series(1,1000000) x;
