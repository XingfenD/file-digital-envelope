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
     * @details
     * NOTE: the caller should malloc 17 bytes to @file_ext_name
     * NOTE: the caller should free the @cipher_key and @cipher_text
     * NOTE: the caller should pass address of two size_t @cipher_key_len and @cipher_text_len
     */
    int parse_fde_file(
        const char *fde_file_path,
        char *file_ext_name,
        uint8_t *crypt_alg,
        uint8_t *cipher_key, uint8_t *cipher_text,
        size_t *cipher_key_len, size_t *cipher_text_len
    );

#endif /* !MAIN_CALLEE_H */
