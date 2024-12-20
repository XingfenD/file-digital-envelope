/**
 * @file ./utils/inc/main_callee.h
 * @author 2022302181113
 * @brief state the sub functions called by main
 * @version 1.0
 * @date 2024-12-12
 *
 * @copyright Copyright (c) 2024
 *
 * in order to avoid cluttered content within the file:./main.c
 * realize the main subfunctions in this module
 *
 * NOTE: this module shouldn't be included by other modules except for ./main.c
 */

#ifndef MAIN_CALLEE_H
#define MAIN_CALLEE_H

    /**
     * @brief parse the fde file head and output the @cipher_key and @cipher_text
     * @param[in]   fde_file_path                           the path to the fde file
     * @param[out]  file_ext_name                           the file extension encoded in fde file
     * @param[out]  crypt_alg                               the field of crypt algorithm
     * @param[out]  cipher_key[length=@cipher_key_len]      the encrypted symmetric key
     * @param[out]  cipher_text[length=@cipher_text_len]    the encrypted text
     * @param[out]  cipher_key_len
     * @param[out]  cipher_text_len
     * @return int - the status code
     * 0 - returns normally
     *
     * @note
     * NOTE: the caller should malloc 17 bytes to $file_ext_name before calling
     *
     * NOTE: the caller should free the $cipher_key and $cipher_text
     *
     * NOTE: the caller should pass address of two size_t $cipher_key_len and $cipher_text_len
     *
     * we should malloc and asign an address to two (uint8_t*) $cipher_key and $cipher_text,
     * conseqently, we pass the address of (uint8_t*)
     */
    int parse_fde_file(
        const char *fde_file_path, char *file_ext_name,
        uint8_t *crypt_alg,
        uint8_t **cipher_key, uint8_t **cipher_text,
        size_t *cipher_key_len, size_t *cipher_text_len
    );

    /**
     * @brief read the key file and write into a byte array
     * @param[in]   key_file_path                           path to the key file
     * @param[out]  key                                     the pointer to a (uint8_t*)
     * @param[out]  key_len                                 the pointer to key_len
     * @return int - the exit status code
     * @note
     * NOTE: the caller should free the memory of @key
     */
    int read_key_file(const char *key_file_path, uint8_t **key, size_t *key_len);

#endif /* !MAIN_CALLEE_H */
