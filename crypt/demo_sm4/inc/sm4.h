/**
 * @file ./crypt/sm4/inc/sm4.h
 * @brief state the top functions of sm4
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 * TODO:reconstruct the sm4 algorithm, hide the given functions from global domain
 *      add padding encryption function instead
 *
 * TODO:rename the folder of demo_sm4 to sm4
 */

#ifndef SM4_H
#define SM4_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#define SM4_BLOCK_BITS 128 /* bits of sm4 algoithm block */
#define SM4_BLOCK_SIZE 16  /* bytes of sm4 algoithm block 16 * 8 = 128 */
#define SM4_KEY_SIZE 16    /* bytes of sm4 algoithm double key 16 * 8 = 128  */
#define SM4_ROUNDS 32

    /**
     * @brief the function to show the module:sm4 is included and compiled correctly
     *
     * TODO: this function should be removed in the final version of this project
     */
    void sm4_print();

    /**
     * @brief Generate encryption subkeys
     * @param[in] key original key
     * @param[out] encSubKeys generated subkeys
     * @return 0 OK
     * @return 1 Failed
     *
     * TODO: hide this function from the global domain and add static keyword in the defination
     * that is, remove this statement from this head file and add static in the source file
     */
    int sm4_make_enc_subkeys(const uint8_t key[SM4_KEY_SIZE], uint32_t encSubKeys[SM4_ROUNDS]);

    /**
     * @brief Generate decryption subkeys
     * @param[in] key original key
     * @param[out] decSubKeys generated subkeys
     * @return 0 OK
     * @return 1 Failed
     *
     * TODO: hide this function from the global domain and add static keyword in the defination
     * that is, remove this statement from this head file and add static in the source file
     */
    int sm4_make_dec_subkeys(const uint8_t key[SM4_KEY_SIZE], uint32_t decSubKeys[SM4_ROUNDS]);

    /**
     * @brief SM4 encrypt single block
     * @param[in] input plaintext, [length = SM4_BLOCK_SIZE]
     * @param[in] encSubKeys encryption subKeys
     * @param[out] output ciphertext, [length = SM4_BLOCK_SIZE]
     *
     * TODO: hide this function from the global domain and add static keyword in the defination
     * that is, remove this statement from this head file and add static in the source file
     */
    void sm4_encrypt_block(const uint8_t *input, const uint32_t encSubKeys[SM4_ROUNDS], uint8_t *output);

    /**
     * @brief SM4 decrypt single block
     * @param[in] input ciphertext, [length = SM4_BLOCK_SIZE]
     * @param[in] decSubKeys decryption subKeys
     * @param[out] output plaintext, [length = SM4_BLOCK_SIZE]
     *
     * TODO: hide this function from the global domain and add static keyword in the defination
     * that is, remove this statement from this head file and add static in the source file
     */
    void sm4_decrypt_block(const uint8_t *input, const uint32_t decSubKeys[SM4_ROUNDS], uint8_t *output);

    /**
     * @brief SM4 encrypt arbitrary length
     * @param[in] input plaintext, [length = @num_of_bytes]
     * @param[out] output ciphertext, [length >= @num_of_bytes]
     * @param[in] num_of_bytes the length of @input
     * @param[in] key the orgin key, need expand
     *
     * this function will be used to encrypt a long text,
     * we apply CBC mode and PKCS#7 padding mode
     *
     * if the length of @input is not an integer multiple of 16
     * then the length of @output won't be the same as @num_of_bytes
     *
     * consequently, the caller of this function should calculate the size of output
     * and malloc memory to varibles (call the function:./utils::pkcs7_padded_len)
     */
    void sm4_padding_encrypt(const uint8_t *input, const uint8_t *output, size_t num_of_bytes, const uint8_t key[SM4_KEY_SIZE]);

    /**
     * @brief SM4 decrypt arbitrary length
     * @param[in] input ciphertext, [length = @in_len]
     * @param[out] output plaintext, [length = @out_len <= @in_len]
     * @param[in] in_len the length of @input
     * @param[out] out_len the length of @output
     * @param[in] key the orgin key, need expand
     *
     * the caller should malloc the same size of memory to input and output
     *
     * the decrypt result will contain padding bytes, this function should identify these padding bytes
     * and pass the real length of plaintext to pointer @out_len (call the function:./utils::pkcs7_parsed_len)
     */
    void sm4_padding_decrypt(const uint8_t *input, const uint8_t *output, size_t in_len, size_t *out_len, const uint8_t key[SM4_KEY_SIZE]);

#ifdef __cplusplus
}
#endif

#endif // SM4_H
