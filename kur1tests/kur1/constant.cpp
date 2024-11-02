#include "constant.h"

const int M = 24;
const int N = 10;

const int TILE_SIZE = 34;

int field[M][N] = { 0 };

int figures[7][4] = {
    1, 3, 5, 7,
    2, 4, 5, 7,
    3, 5, 4, 6,
    3, 5, 4, 7,
    2, 3, 5, 7,
    3, 5, 7, 6,
    2, 3, 4, 5
};

Point a[4], b[4];