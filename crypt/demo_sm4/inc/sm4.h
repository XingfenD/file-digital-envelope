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

#define SM4_BLOCK_BITS 128 /* bits of AES algoithm block */
#define SM4_BLOCK_SIZE 16  /* bytes of AES algoithm block 16 * 8 = 128 */
#define SM4_KEY_SIZE 16    /* bytes of AES algoithm double key 16 * 8 = 128  */
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
     */
    int sm4_make_enc_subkeys(const uint8_t key[SM4_KEY_SIZE], uint32_t encSubKeys[SM4_ROUNDS]);

    /**
     * @brief Generate decryption subkeys
     * @param[in] key original key
     * @param[out] decSubKeys generated subkeys
     * @return 0 OK
     * @return 1 Failed
     */
    int sm4_make_dec_subkeys(const uint8_t key[SM4_KEY_SIZE], uint32_t decSubKeys[SM4_ROUNDS]);

    /**
     * @brief SM4 encrypt single block
     * @param[in] input plaintext, [length = SM4_BLOCK_SIZE]
     * @param[in] encSubKeys encryption subKeys
     * @param[out] output ciphertext, [length = SM4_BLOCK_SIZE]
     */
    void sm4_encrypt_block(const uint8_t *input, const uint32_t encSubKeys[SM4_ROUNDS], uint8_t *output);

    /**
     * @brief SM4 decrypt single block
     * @param[in] input ciphertext, [length = SM4_BLOCK_SIZE]
     * @param[in] decSubKeys decryption subKeys
     * @param[out] output plaintext, [length = SM4_BLOCK_SIZE]
     */
    void sm4_decrypt_block(const uint8_t *input, const uint32_t decSubKeys[SM4_ROUNDS], uint8_t *output);

#ifdef __cplusplus
}
#endif

#endif // SM4_H
