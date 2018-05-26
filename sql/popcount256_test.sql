CREATE EXTENSION popcount;

SELECT popcount256(15::bit(4));
SELECT popcount256(7::bit(4));

SELECT popcount256(15::bit(8));
SELECT popcount256(7::bit(8));

SELECT popcount256(15::bit(12));
SELECT popcount256(7::bit(12));

SELECT popcount256(15::bit(32));
SELECT popcount256(7::bit(32));

SELECT popcount256(30::bit(32));
SELECT popcount256(127::bit(32));

SELECT popcount256(15::bit(256));
SELECT popcount256(7::bit(256));

SELECT popcount256(15::bit(512));
SELECT popcount256(7::bit(512));

SELECT popcount256(15::bit(8192));
SELECT popcount256(7::bit(8192));

SELECT popcount256(15::bit(1234));
SELECT popcount256(7::bit(1234));

SELECT popcount256(15::bit(3000));
SELECT popcount256(7::bit(3000));

SELECT popcount256(15::bit(8192));
SELECT popcount256(7::bit(8192));

SELECT sum(popcount256(x::bit(8192))) from generate_series(1,1000000) x;
SELECT sum(popcount256(x::bit(8192))) from generate_series(1,1000000) x;
