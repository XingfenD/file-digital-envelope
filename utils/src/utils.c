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

// int str_find(const char str2find[], const char *strArr[], const int size) {
//     int map[size];
//     for (int i = 0; i < size; i++) {
//         map[size] = i;
//     }
// }

/* end of advanced string function definations */

void random_bytes(uint8_t *bytes2random, int bytes_num) {
    for (int i = 0; i < bytes_num; i++) {
        bytes2random[i] = (uint8_t) (rand() % 256);
    }
}
