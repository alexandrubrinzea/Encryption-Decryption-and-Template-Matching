#include "xorshift.h"

unsigned int* xorshift32(unsigned int seed, unsigned int size) {
    unsigned int *output = (unsigned int*)malloc(sizeof(unsigned int)*(size + 1));
    output[0] = seed;
    unsigned int r = seed;

    int k;

    for (k = 0; k < size; k++) {
        r = r ^ r << 13;
        r = r ^ r >> 17;
        r = r ^ r << 5;
        output[k + 1] = r;
    }
    return output;
}
