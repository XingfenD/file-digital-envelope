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
#include <malloc.h>

void utils_print() {
    // TODO: this function should be removed in the final version of this project
    debug_print("this is function ./utils/src/utils.c:utils_print()\n");
}

size_t pkcs7_padded_len(const uint8_t bytes_to_pad, size_t in_len) {
    /* TODO: finish this function */
}

size_t pkcs7_parsed_len(const uint8_t bytes_to_parse, size_t in_len) {
    /* TODO: finish this function  */
}

char *str_malloc_cpy(const char *src) {
    int len_of_src = strlen(src);
    char *dst;

    dst = (char *) malloc(sizeof(char) * (len_of_src + 1));
    strcpy(dst, src);

    return dst;
}