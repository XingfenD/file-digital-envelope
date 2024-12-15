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
    if ((p_dot = strchr(origin_file_name, '.')) == NULL) {
        ret = str_malloc_cat(origin_file_name, new_ext_name);
    } else {
        ret = calloc((int) (p_dot - origin_file_name) + 1 + strlen(new_ext_name) + 1, sizeof(char));
        // pr_int("calloc size", (int) (p_dot - origin_file_name) + 1 + strlen(new_ext_name) + 1);
        memcpy(ret, origin_file_name, (p_dot - origin_file_name) + 1);
        strcat(ret, new_ext_name);
    }

    return ret;
}

/* end of advanced string function definations */