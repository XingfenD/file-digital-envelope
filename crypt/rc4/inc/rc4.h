/*
 * @Author: “shoutifeng” “shoutifeng@gmail.com”
 * @Date: 2025-01-08 00:15:53
 * @LastEditors: “shoutifeng” “shoutifeng@gmail.com”
 * @LastEditTime: 2025-01-08 17:29:30
 * @FilePath: \file_digital_envelope\file-digital-envelope\crypt\rc4\inc\rc4.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef RC4_H
#define RC4_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @brief 
 * @param key 
 * @param key_len 
 */
void rc4_init(uint8_t *key,uint32_t key_len);

/**
 * @brief 
 * @param input 
 * @param in_len 
 * @param key 
 * @param output 
 */
void rc4_encrypt(uint8_t *input, size_t in_len, uint8_t *key, uint8_t *output);

/**
 * @brief same with rc4_encrypt
 * @param input 
 * @param in_len 
 * @param key 
 * @param output 
 */
void rc4_decrypt(uint8_t *input, size_t in_len, uint8_t *key, uint8_t *output);

#ifdef __cplusplus
}
#endif

#endif // SM4_H
