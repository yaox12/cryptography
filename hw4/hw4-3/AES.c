#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AES.h"
#include "operation.h"

int sub_bytes(unsigned char* state, int l) {
    unsigned char table[256] = {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
                                0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                                0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
                                0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                                0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
                                0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                                0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
                                0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                                0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
                                0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                                0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
                                0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                                0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
                                0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                                0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 
                                0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};
    int i;
    for(i = 0; i < l; i ++)
        state[i] = table[state[i]];
    return 1;
}

int shift_rows(unsigned char* state) {
    unsigned char tmp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = tmp;
    tmp = state[2];
    state[2] = state[10];
    state[10] = tmp;
    tmp = state[6];
    state[6] = state[14];
    state[14] = tmp;
    tmp = state[3];
    state[3] = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = tmp;
    return 1;
}

int mix_columns(unsigned char* state) {
	int i, j;
    for(i = 0; i < 4; i ++) {
        unsigned char a[4];
        unsigned char b[4];
        unsigned char h;
        for(j = 0; j < 4; j ++) {
            a[j] = state[i * 4 + j];
            h = (unsigned char)((signed char)a[j] >> 7);
            b[j] = a[j] << 1;
            b[j] ^= 0x1b & h;
        }
        state[i * 4] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1]; // 2 * a0 + a3 + a2 + 3 * a1
        state[i * 4 + 1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2]; // 2 * a1 + a0 + a3 + 3 * a2
        state[i * 4 + 2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3]; // 2 * a2 + a1 + a0 + 3 * a3
        state[i * 4 + 3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0]; // 2 * a3 + a2 + a1 + 3 * a0
    }
}

int add_round_key(unsigned char* state, unsigned char* key) {
    int i;
    for(i = 0; i < 16; i ++)
        state[i] ^= key[i];
    return 1;
}

int AES_key_expansion(unsigned char* subkey, unsigned char* key, int n, int m) {
    int i, j;
    unsigned char rc[] = {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};
    memcpy(subkey, key, n);
    for(i = n; i < m; i ++) {
        char buf[4];
        for(j = 0; j < 4; j ++)
            buf[j] = subkey[(i - 1) * 4 + j];
        if(i % n == 0) {
            // rotword
            char tmp = buf[3];
            for(j = 0; j < 2; j ++)
                buf[j] = buf[j + 1];
            buf[3] = tmp;
            // subbytes
            sub_bytes(buf, 4);
            // addrcon
            for(j = 0; j < 4; j ++)
                buf[j] ^= rc[i / n];
        } else if(n == 8 && i % n == 4) {
            // subbytes
            sub_bytes(buf, 4);
        }
        for(j = 0; j < 4; j ++)
            subkey[i * 4 + j] = subkey[(i - n) * 4 + j] ^ buf[j];
    }
    return 1;
}

int AES_block(unsigned char* state, unsigned char* subkey, unsigned char* result, int r) {
    int i;
    memcpy(result, state, 16);
    add_round_key(result, subkey);
    for(i = 1; i <= r; i ++) {
        sub_bytes(result, 16);
        shift_rows(result);
        if(i != r)
            mix_columns(result);
        add_round_key(result, subkey + i * 16);
    }
    return 1;
}

int AES_128_key_expansion(unsigned char* subkey, unsigned char* key) {
    return AES_key_expansion(subkey, key, 16, 44);
}

int AES_192_key_expansion(unsigned char* subkey, unsigned char* key) {
    return AES_key_expansion(subkey, key, 24, 52);
}

int AES_256_key_expansion(unsigned char* subkey, unsigned char* key) {
    return AES_key_expansion(subkey, key, 32, 60);
}

int AES_128(unsigned char* block, unsigned char* subkey, unsigned char* result) {
    return AES_block(block, subkey, result, 10);
}

int AES_192(unsigned char* block, unsigned char* subkey, unsigned char* result) {
    return AES_block(block, subkey, result, 12);
}

int AES_256(unsigned char* block, unsigned char* subkey, unsigned char* result) {
    return AES_block(block, subkey, result, 14);
}