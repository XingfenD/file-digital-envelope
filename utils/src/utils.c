/**
 * @file ./utils/src/utils.c
 * @brief realize the functions stated in file:./utils/inc/utils.h
 * @author 202230218113
 * @version 1.0
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <debug.h>
#include <utils.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <defs.h>

void utils_print() {
    // TODO: this function should be removed in the final version of this project
    debug_print("this is function ./utils/src/utils.c:utils_print()\n");
}

/* start of pkcs7 function definations */

size_t pkcs7_padded_len(size_t in_len) {
    /* find the smallest multiple of 16 greater than @in_len */
    if (in_len % 16 == 0) {
        return in_len + 16;
    } else {
        return ((in_len + 15) >> 4) << 4;
    }
}

size_t pkcs7_parsed_len(const uint8_t *bytes_to_parse, size_t in_len) {
    /* read the last byte in the array */
    int padded_bytes_num = bytes_to_parse[(int) in_len - 1];
    for (int i = in_len - 1; i >= (int) (in_len - padded_bytes_num); i--) {
        if ((int) bytes_to_parse[i] != padded_bytes_num) {
            return in_len;
        }
    }

    return in_len - padded_bytes_num;
}

void pkcs7_padding(const uint8_t *input, const size_t in_len, uint8_t *output) {
    memcpy(output, input, in_len);
    int padded_len = pkcs7_padded_len(in_len);
    uint8_t pad_byte =  padded_len - in_len;
    for (int i = in_len; i < padded_len; i++) {
        output[i] = pad_byte;
    }
}

/* end of pkcs7 function definations */

/* start of advanced string function definations */

static
char* str_malloc_cat(const char *dst, const char *src) {
    int dst_len = strlen(dst);
    char *ret = malloc(sizeof(char) * (dst_len + strlen(src) + 1));

    strcpy(ret, dst);
    strcpy(ret + dst_len, src);

    return ret;
}

char *str_malloc_cpy(const char *src) {
    int len_of_src = strlen(src);
    char *dst;

    dst = (char *) malloc(sizeof(char) * (len_of_src + 1));
    strcpy(dst, src);

    return dst;
}

char *str_rep_ext(const char *origin_file_name, const char *new_ext_name) {
    char *p_dot = NULL;
    char *ret = NULL;
    if ((p_dot = strrchr(origin_file_name, '.')) == NULL) {
        /* if there are not extension in the filename */
        ret = str_malloc_cat(origin_file_name, new_ext_name);
    } else {
        ret = calloc((int) (p_dot - origin_file_name) + 1 + strlen(new_ext_name) + 1, sizeof(char));
        memcpy(ret, origin_file_name, (p_dot - origin_file_name) + 1);
        strcat(ret, new_ext_name);
    }

    return ret;
}

/* end of advanced string function definations */

void random_bytes(uint8_t *bytes2random, int bytes_num) {
    for (int i = 0; i < bytes_num; i++) {
        bytes2random[i] = (uint8_t) (rand() % 256);
    }
}

void cbc_padding_encrypt(CBC_ENC block_enc, const size_t in_len, size_t *out_len, const uint8_t *input, void* subKeys, uint8_t **output, uint8_t *vector) {
    /* init the variables and malloc memory */
    *out_len = pkcs7_padded_len(in_len);
    uint8_t *padded_input = malloc(sizeof(uint8_t) * *out_len);
    uint8_t xor_arg[16];
    uint8_t *encblock[16];
    *output = malloc(sizeof(uint8_t) * *out_len);

    /* pad the input bytes */
    pkcs7_padding(input, in_len, padded_input);

    /* TODO: execute sm4_cbc_encrypt process below */
    /**
     * related varibles
     * @param[in]   padded_input        the bytes to encrypt in cbc mode
     * @param[out]  *output             the encrypt result of cbc
     * @param[in]   *out_len            both the length of @padded_input and @*output
     * @param[in]   vector              the origin xor arg vector (iv)
     */

    memcpy(xor_arg, vector, 16);
    size_t block_num = *out_len / 16;
    for (size_t i = 0; i < block_num; i++) {
        // fread(encblock, sizeof(unsigned char), 16, bin_input_file);
        memcpy(encblock, padded_input + block_num * 16, 16);
        ((uint32_t *) encblock)[0] ^= ((uint32_t *) xor_arg)[0];
        ((uint32_t *) encblock)[1] ^= ((uint32_t *) xor_arg)[1];
        ((uint32_t *) encblock)[2] ^= ((uint32_t *) xor_arg)[2];
        ((uint32_t *) encblock)[3] ^= ((uint32_t *) xor_arg)[3];
        block_enc(encblock, subKeys, xor_arg);
        // fwrite(xor_arg, sizeof(unsigned char), 16, bin_output_file);
        memcpy(*output + 16 * i, xor_arg, 16);
    }

    free(padded_input);
}

void cbc_padding_decrypt(CBC_ENC block_dec, const size_t in_len, size_t *out_len, const uint8_t *input, void* subKeys, uint8_t **output, uint8_t *vector) {
    /* TODO: execute sm4_cbc_decrypt process below */
    /**
     * related varibles
     * @param[in]   input               the bytes to decrypt in cbc mode
     * @param[out]  output              the decrypt result of cbc
     * @param[in]   in_len              both the length of @input and @output
     * @param[in]   vector              the origin xor arg vector (iv)
     */

    *out_len = pkcs7_parsed_len(*output, in_len);
}
