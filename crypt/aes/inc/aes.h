/**
 * @file ./crypt/aes/inc/aes.h
 * @brief state the top functions of aes
 * @author Fendy (xingfen.star@gmail.com)
 * @version 2.0
 * @date 2024-11-25
 * @copyright Copyright (c) 2025
 */

#ifndef AES_H
#define AES_H

    #include <stdint.h>

    #define AES_BLOCK_BITS 128 /* bits of AES algoithm block */
    #define AES_BLOCK_SIZE 16  /* bytes of AES algoithm block */
    #define AES_KEY_SIZE 16    /* bytes of AES algoithm double key */

    /**
     * @brief the function to show the module:aes is included and compiled correctly
     *
     * TODO: this function should be removed in the final version of this project
     */
    void aes_print();

    /**
     * @brief AES encrypt arbitrary length
     * @param[in]   input       plaintext, [length = @in_len]
     * @param[out]  output      ciphertext, [length >= @in_len]
     * @param[in]   in_len      the length of @input
     * @param[out]  out_len     the length of @*output
     * @param[in]   vector      the first block vi used in xor
     * @param[in]   key         the orgin key, need expand
     * @note
     * NOTE: the caller should free the memory in @*output
     *
     * this function will be used to encrypt a long text,
     * we apply CBC mode and PKCS#7 padding mode
     *
     * if the length of @input is not an integer multiple of 16,
     * then the length of @output won't be the same as @in_len
     *
     * consequently, the caller of this function should pass the pointer of @output,
     * so can the function malloc the memory in it (call the function:./utils::pkcs7_padded_len)
     */
    void aes_padding_encrypt(
        const uint8_t *input, uint8_t **output,
        size_t in_len, size_t *out_len,
        const uint8_t vector[16], const uint8_t key[16]
    );

    /**
     * @brief AES decrypt arbitrary length
     * @param[in]   input       ciphertext, [length = @in_len]
     * @param[out]  output      plaintext, [length = @out_len <= @in_len]
     * @param[in]   in_len      the length of @input
     * @param[out]  out_len     the length of @output
     * @param[in]   vector      the block vi used in xor
     * @param[in]   key         the orgin key, need expand
     * @note
     * TODO: correct the comment
     *
     * NOTE: the caller should malloc the same size of memory to input and output
     *
     * the decrypt result will contain padding bytes, this function should identify these padding bytes
     * and pass the real length of plaintext to pointer @out_len (call the function:./utils::pkcs7_parsed_len)
     */
    void aes_padding_decrypt(
        const uint8_t *input, uint8_t *output,
        size_t in_len, size_t *out_len,
        const uint8_t vector[16], const uint8_t key[16]
    );

#endif // AES_H
