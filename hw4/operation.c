#include "operation.h"

unsigned leftrotate(unsigned x, int i) {
    return x = (x << i) | x >> (32 - i);
}

unsigned rightrotate(unsigned x, int i) {
    return leftrotate(x, 32 - i);
}

unsigned char* longlong_to_char(unsigned char* str, unsigned long long x) {
    int i;
    for(i = 0; i < 8; i ++)
        str[i] = x >> (8 * (7 - i));
    return str;
}

unsigned char_to_unsigned(unsigned char* str) {
    unsigned x = 0;
    int i;
    for(i = 0; i < 4; i ++) {
        x <<= 8;
        x += (unsigned)str[i];
    }
    return x;
}