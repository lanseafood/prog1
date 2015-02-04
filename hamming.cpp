// Implement your solutions in this file
#include "hamming.h"

unsigned int hamming(uint64_t x, uint64_t y)
{
    uint64_t hamming_dist = 0;
    uint64_t temp = x^y;

    while (temp!=0) {
    	temp &= (temp-1);
    	hamming_dist++;
    }


    return hamming_dist;

    //return 0;
}
