/**
 * @file ./crypt/aes/src/aes_sub.c
 * @brief realize the sub functions stated in file:./crypt/aes/inc/aes_sub.h
 * @author 2022302181113
 * @version 1.0
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <table_defs.h>
#include <stdint.h>
#include <string.h>
#include <bin_lib.h>



static void s_map(unsigned char * src_bytes, const unsigned char s_table[16][16], int len) {
    for (int i = 0; i < len; i++) {
        src_bytes[i] = s_table[src_bytes[i] >> 4][src_bytes[i] & 0x0F];
    }
}

void aes_encrypt_word_s_map(unsigned char src_bytes[4]) {
    s_map(src_bytes, AES_S_Box1, 4);
}

void aes_decrypt_word_s_map(unsigned char src_bytes[4]) {
    s_map(src_bytes, AES_S_Box2, 4);
}

void aes_encrypt_s_map(unsigned char src_bytes[16]) {
    s_map(src_bytes, AES_S_Box1, 16);
}

void aes_decrypt_s_map(unsigned char src_bytes[16]) {
    s_map(src_bytes, AES_S_Box2, 16);
}

void aes_lef_shift_rows(unsigned char src_bytes[16]) {
    /**
     * array layout in a block(numbers refer to the index in the array):
     *
     * input:
     * 0x00, 0x04, 0x08, 0x12
     * 0x01, 0x05, 0x09, 0x13
     * 0x02, 0x06, 0x10, 0x14
     * 0x03, 0x07, 0x11, 0x15
     *
     * output:
     * 0x00, 0x04, 0x08, 0x12
     * 0x05, 0x09, 0x13, 0x01
     * 0x10, 0x14, 0x02, 0x06
     * 0x15, 0x03, 0x07, 0x11
     */

    unsigned char temp[2];

    // row 0
    // nothing to do

    // row 1
    temp[0] = src_bytes[1];
    src_bytes[1] = src_bytes[5];
    src_bytes[5] = src_bytes[9];
    src_bytes[9] = src_bytes[13];
    src_bytes[13] = temp[0];

    // row 2
    temp[0] = src_bytes[2];
    temp[1] = src_bytes[6];
    src_bytes[2] = src_bytes[10];
    src_bytes[6] = src_bytes[14];
    src_bytes[10] = temp[0];
    src_bytes[14] = temp[1];

    // row 3
    temp[0] = src_bytes[15];
    src_bytes[15] = src_bytes[11];
    src_bytes[11] = src_bytes[7];
    src_bytes[7] = src_bytes[3];
    src_bytes[3] = temp[0];
}

void aes_rig_shift_rows(unsigned char src_bytes[16]) {
    /**
     * array layout in a block(numbers refer to the index in the array):
     *
     * input:
     * 0x00, 0x04, 0x08, 0x12
     * 0x01, 0x05, 0x09, 0x13
     * 0x02, 0x06, 0x10, 0x14
     * 0x03, 0x07, 0x11, 0x15
     *
     * output:
     * 0x00, 0x04, 0x08, 0x12
     * 0x13, 0x01, 0x05, 0x09
     * 0x10, 0x14, 0x02, 0x06
     * 0x07, 0x11, 0x15, 0x03
     */

    unsigned char temp[2];

    // row 0
    // nothing to do

    // row 1
    temp[0] = src_bytes[13];
    src_bytes[13] = src_bytes[9];
    src_bytes[9] = src_bytes[5];
    src_bytes[5] = src_bytes[1];
    src_bytes[1] = temp[0];

    // row 2
    temp[0] = src_bytes[2];
    temp[1] = src_bytes[6];
    src_bytes[2] = src_bytes[10];
    src_bytes[6] = src_bytes[14];
    src_bytes[10] = temp[0];
    src_bytes[14] = temp[1];

    // row 3
    temp[0] = src_bytes[3];
    src_bytes[3] = src_bytes[7];
    src_bytes[7] = src_bytes[11];
    src_bytes[11] = src_bytes[15];
    src_bytes[15] = temp[0];
}

void aes_encrypt_col_confuse(unsigned char src_bytes[16]) {
    unsigned char tmp[16];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tmp[i * 4 + j] =
                AES_MAT_MUL((int) AES_MixCol_Mat[j][0], (int) src_bytes[i * 4 + 0]) ^
                AES_MAT_MUL((int) AES_MixCol_Mat[j][1], (int) src_bytes[i * 4 + 1]) ^
                AES_MAT_MUL((int) AES_MixCol_Mat[j][2], (int) src_bytes[i * 4 + 2]) ^
                AES_MAT_MUL((int) AES_MixCol_Mat[j][3], (int) src_bytes[i * 4 + 3]);
        }
    }

    memcpy(src_bytes, tmp, 16);
}

void aes_decrypt_col_confuse(unsigned char src_bytes[16]) {
    unsigned char tmp[16];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            tmp[i * 4 + j] =
                AES_MAT_MUL((int) AES_deMixCol_Mat[j][0], (int) src_bytes[i * 4 + 0]) ^
                AES_MAT_MUL((int) AES_deMixCol_Mat[j][1], (int) src_bytes[i * 4 + 1]) ^
                AES_MAT_MUL((int) AES_deMixCol_Mat[j][2], (int) src_bytes[i * 4 + 2]) ^
                AES_MAT_MUL((int) AES_deMixCol_Mat[j][3], (int) src_bytes[i * 4 + 3]);
        }
    }

    memcpy(src_bytes, tmp, 16);
}

void aes_add_round_key(unsigned char src_bytes[16], unsigned char round_key[16]) {
    *((uint32_t *) &src_bytes[0]) =
        *((uint32_t *) &src_bytes[0]) ^
        *((uint32_t *) &round_key[0]);

    *((uint32_t *) &src_bytes[4]) =
        *((uint32_t *) &src_bytes[4]) ^
        *((uint32_t *) &round_key[4]);

    *((uint32_t *) &src_bytes[8]) =
        *((uint32_t *) &src_bytes[8]) ^
        *((uint32_t *) &round_key[8]);

    *((uint32_t *) &src_bytes[12]) =
        *((uint32_t *) &src_bytes[12]) ^
        *((uint32_t *) &round_key[12]);
}