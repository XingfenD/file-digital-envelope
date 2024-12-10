/**
 * @file ./crypt/sm4/src/sm4.c
 * @brief realize the functions stated in file:./crypt/sm4/inc/sm4.h
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdint.h>
#include <table_defs.h>
#include <bin_lib.h>
#include <sm4_sub.h>
#include <debug.h>

// #define NOT_LOOP_UNROLL

void sm4_print() {
    // TODO: this function should be removed in the final version of this project
    debug_print("this is function ./crypt/demo_sm4/src/sm4.c:sm4_print()\n");
}

int sm4_make_enc_subkeys(const uint8_t key[16], uint32_t encSubKeys[32]) {
    uint32_t SM4_fk[4];

    for (int i = 0; i < 4; i++) {
        SM4_fk[i] = xor_uint8_uint32(key + i * 4, SM4_FK[i]);
        // pr_uint32_hex(fk[i]); /* passed */
    }

    {
        encSubKeys[0]=
            SM4_fk[0] ^
            sm4_t_rev(
                SM4_fk[1] ^
                SM4_fk[2] ^
                SM4_fk[3] ^
                SM4_CK[0]
            )
        ;
        // pr_uint32_hex(encSubKeys[0]);
        encSubKeys[1] =
            SM4_fk[1] ^
            sm4_t_rev(
                SM4_fk[2] ^
                SM4_fk[3] ^
                encSubKeys[0] ^
                SM4_CK[1]
            )
        ;
        encSubKeys[2] =
            SM4_fk[2] ^
            sm4_t_rev(
                SM4_fk[3] ^
                encSubKeys[0] ^
                encSubKeys[1] ^
                SM4_CK[2]
            )
        ;
        encSubKeys[3] =
            SM4_fk[3] ^
            sm4_t_rev(
                encSubKeys[0] ^
                encSubKeys[1] ^
                encSubKeys[2] ^
                SM4_CK[3]
            )
        ;
    }

    for (int i = 4; i < 32; i++) {
        encSubKeys[i] =
            encSubKeys[i - 4] ^
            sm4_t_rev(
                encSubKeys[i - 3] ^
                encSubKeys[i - 2] ^
                encSubKeys[i - 1] ^
                SM4_CK[i]
            )
        ;
    }

    return 0;
}

int sm4_make_dec_subkeys(const uint8_t key[16], uint32_t decSubKeys[32]) {
    return sm4_make_enc_subkeys(key, decSubKeys);
}

void sm4_encrypt_block(const uint8_t *input, const uint32_t encSubKeys[32], uint8_t *output) {
    uint32_t tmp_words[4] = {
        bytes2word(input),
        bytes2word(input + 4),
        bytes2word(input + 8),
        bytes2word(input + 12)
    };

    #ifdef NOT_LOOP_UNROLL
        for (int i = 0; i < 32; i++) {
        tmp_words[i % 4] = sm4_f_function(
            tmp_words[i % 4],
            tmp_words[(i + 1) % 4],
            tmp_words[(i + 2) % 4],
            tmp_words[(i + 3) % 4],
            encSubKeys[i]
        );
    }
    #else /* NOT_LOOP_UNROLL */
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], encSubKeys[0]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], encSubKeys[1]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], encSubKeys[2]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], encSubKeys[3]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], encSubKeys[4]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], encSubKeys[5]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], encSubKeys[6]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], encSubKeys[7]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], encSubKeys[8]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], encSubKeys[9]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], encSubKeys[10]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], encSubKeys[11]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], encSubKeys[12]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], encSubKeys[13]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], encSubKeys[14]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], encSubKeys[15]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], encSubKeys[16]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], encSubKeys[17]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], encSubKeys[18]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], encSubKeys[19]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], encSubKeys[20]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], encSubKeys[21]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], encSubKeys[22]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], encSubKeys[23]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], encSubKeys[24]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], encSubKeys[25]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], encSubKeys[26]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], encSubKeys[27]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], encSubKeys[28]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], encSubKeys[29]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], encSubKeys[30]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], encSubKeys[31]);
    #endif /* !NOT_LOOP_UNROLL */

    for (int i = 0; i < 4; i++) {
        word2bytes(&output[i * 4], tmp_words[3 - i]);
    }
}

void sm4_decrypt_block(const uint8_t *input, const uint32_t decSubKeys[32], uint8_t *output) {
    uint32_t tmp_words[4] = {
        bytes2word(input),
        bytes2word(input + 4),
        bytes2word(input + 8),
        bytes2word(input + 12)
    };
    #ifdef NOT_LOOP_UNROLL
        for (int i = 0; i < 32; i++) {
        tmp_words[i % 4] = sm4_f_function(
            tmp_words[i % 4],
            tmp_words[(i + 1) % 4],
            tmp_words[(i + 2) % 4],
            tmp_words[(i + 3) % 4],
            decSubKeys[31 - i]
        );
    }
    #else /* NOT_LOOP_UNROLL */
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], decSubKeys[31]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], decSubKeys[30]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], decSubKeys[29]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], decSubKeys[28]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], decSubKeys[27]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], decSubKeys[26]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], decSubKeys[25]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], decSubKeys[24]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], decSubKeys[23]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], decSubKeys[22]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], decSubKeys[21]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], decSubKeys[20]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], decSubKeys[19]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], decSubKeys[18]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], decSubKeys[17]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], decSubKeys[16]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], decSubKeys[15]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], decSubKeys[14]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], decSubKeys[13]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], decSubKeys[12]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], decSubKeys[11]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], decSubKeys[10]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], decSubKeys[9]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], decSubKeys[8]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], decSubKeys[7]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], decSubKeys[6]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], decSubKeys[5]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], decSubKeys[4]);
        tmp_words[0] = sm4_f_function(tmp_words[0], tmp_words[1], tmp_words[2], tmp_words[3], decSubKeys[3]);
        tmp_words[1] = sm4_f_function(tmp_words[1], tmp_words[2], tmp_words[3], tmp_words[0], decSubKeys[2]);
        tmp_words[2] = sm4_f_function(tmp_words[2], tmp_words[3], tmp_words[0], tmp_words[1], decSubKeys[1]);
        tmp_words[3] = sm4_f_function(tmp_words[3], tmp_words[0], tmp_words[1], tmp_words[2], decSubKeys[0]);
    #endif /* !NOT_LOOP_UNROLL*/

    for (int i = 0; i < 4; i++) {
        word2bytes(&output[i * 4], tmp_words[3 - i]);
    }
}
