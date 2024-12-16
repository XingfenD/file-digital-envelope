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
#define ROTL(value, shift) ((uint32_t)value << shift) | ((uint32_t)value >> (32 - shift))

    /* System parameter */
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

    void sm4_encrypt_cbc(uint8_t *input, size_t length, uint8_t iv[16], const uint32_t encSubkey[SM4_ROUNDS], uint8_t *output);
    void sm4_decrypt_cbc(uint8_t *input, size_t length, uint8_t iv[16], const uint32_t decSubkey[SM4_ROUNDS], uint8_t *output);

    int sm4_padding_encrypt_cbc(uint8_t *input, size_t inlen, uint8_t iv[16], const uint32_t encSubkey[SM4_ROUNDS], uint8_t *output, size_t *outlen);
    int sm4_padding_decrypt_cbc(uint8_t *input, size_t inlen, uint8_t iv[16], const uint32_t decSubkey[SM4_ROUNDS], uint8_t *output, size_t *outlen);

#ifdef __cplusplus
}
#endif

#endif // SM4_H
