/**
 * @file ./utils/src/main_callee.c
 * @author 2022302181113
 * @brief realize the sub functions stated in file:./utils/inc/main_callee.h
 * @version 1.0
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 * NOTE: the functions in this file shouldn't be called by other modules except for ./main.c
 */

#include <debug.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <utils.h>
#include <string.h>
#include <defs.h>

long get_file_size(FILE *stream) {
	long file_size = -1;
	long cur_offset = ftell(stream);	// 获取当前偏移位置
	if (cur_offset == -1) {
		printf("ftell failed\n");
		return -1;
	}
	if (fseek(stream, 0, SEEK_END) != 0) {	// 移动文件指针到文件末尾
		printf("fseek failed");
		return -1;
	}
	file_size = ftell(stream);	// 获取此时偏移值，即文件大小
	if (file_size == -1) {
		printf("ftell failed");
	}
	if (fseek(stream, cur_offset, SEEK_SET) != 0) {	// 将文件指针恢复初始位置
		printf("fseek failed\n");
		return -1;
	}
	return file_size;
}

int parse_fde_file(
    const char *fde_file_path,
	char *file_ext_name,
	uint8_t *crypt_alg,
    uint8_t *cipher_key, uint8_t *cipher_text,
    size_t *cipher_key_len, size_t *cipher_text_len
) {
    FDE_HEAD fde_head;
    FILE *pfde = fopen(fde_file_path, "rb");
    size_t file_size = (size_t) get_file_size(pfde);

    if (file_size <= (sizeof(fde_head))) {
		return ERR_PARSE_FAIL;
    }
    fread(&fde_head, sizeof(fde_head), 1, pfde);
    memcpy(file_ext_name, &(fde_head.origin_ext), 16);
	*cipher_key_len = (size_t) fde_head.sym_key_len;
}