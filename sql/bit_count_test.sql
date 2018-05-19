CREATE EXTENSION bit_count;
SELECT bit_count_encode(0);
SELECT bit_count_encode(1);
SELECT bit_count_encode(10);
SELECT bit_count_encode(35);
SELECT bit_count_encode(36);