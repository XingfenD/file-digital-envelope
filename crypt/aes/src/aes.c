/**
 * @file ./crypt/aes/src/aes.c
 * @brief realize the function stated in file:./crypt/aes/inc/aes.h
 * @author 2022302181113
 * @version 1.0
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <string.h>
#include <bin_lib.h>
#include <aes_sub.h>
#include <table_defs.h>
#include <debug.h>

void aes_print() {
    printf("this is function ./crypt/aes/src/aes.c:aes_print()\n");
}

int aes_make_enc_subkeys(const unsigned char key[16], unsigned char subKeys[11][16]) {
    memcpy(subKeys[0], key, 16);

    for (int i = 1; i < 11; i++) {
        // the first column in a 4 * 4 bytes block
        memcpy(&subKeys[i][0], &subKeys[i - 1][12], 4);
        word_left_loop_byte((uint32_t *) &subKeys[i][0], 1);
        aes_encrypt_word_s_map(&subKeys[i][0]);

        *((uint32_t *) &subKeys[i][0]) =
            *((uint32_t *) &subKeys[i - 1][0]) ^
            *((uint32_t *) &subKeys[i][0]) ^
            AES_R_Con[i - 1];

        // the following three columns in the same 4 * 4 bytes block
        for (int j = 1; j < 4; j++) {
            *((uint32_t *) &subKeys[i][4 * j]) =
                *((uint32_t *) &subKeys[i][4 * j - 4]) ^
                *((uint32_t *) &subKeys[i - 1][4 * j]);
        }
    }

    return 0;
}

int aes_make_dec_subkeys(const unsigned char key[16], unsigned char subKeys[11][16]) {
    aes_make_enc_subkeys(key, subKeys);

    /**
     * the dec subKeys is defined like this
     * perform decrypt_col_confuse operation on the subKeys except for the first and last one
     */
    for (int i = 1; i < 10; i++) {
        aes_decrypt_col_confuse(subKeys[i]);
    }

    return 0;
}

void aes_encrypt_block(const unsigned char *input, unsigned char subKeys[11][16], unsigned char *output) {
    // initialize the output
    memcpy(output, input, 16);
    aes_add_round_key(output, subKeys[0]);

    for (int i = 1; i < 10; i++) {
        aes_encrypt_s_map(output);
        aes_lef_shift_rows(output);
        aes_encrypt_col_confuse(output);
        aes_add_round_key(output, subKeys[i]);
    }

    aes_encrypt_s_map(output);
    aes_lef_shift_rows(output);
    aes_add_round_key(output, subKeys[10]);
}

void aes_decrypt_block(const unsigned char *input, unsigned char subKeys[11][16], unsigned char *output) {
    
    memcpy(output, input, 16);
    aes_add_round_key(output, subKeys[10]);
    for (int i = 9; i >= 1; i--) {
        aes_decrypt_s_map(output);
        aes_rig_shift_rows(output);
        aes_decrypt_col_confuse(output);
        aes_add_round_key(output, subKeys[i]);
    }
    aes_decrypt_s_map(output);
    aes_rig_shift_rows(output);
    aes_add_round_key(output, subKeys[0]);
}