#ifndef AES_H
#define AES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

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
     * @brief Generate encryption subkeys
     * @param[in] key original key
     * @param[out] subKeys generated encryption subkeys
     * @return 0 OK
     * @return 1 Failed
     */
    int aes_make_enc_subkeys(const uint8_t key[16], uint8_t subKeys[11][16]);

    /**
     * @brief Generate decryption subkeys
     * @param[in] key original key
     * @param[out] subKeys generated decryption subkeys
     * @return 0 OK
     * @return 1 Failed
     */
    int aes_make_dec_subkeys(const uint8_t key[16], uint8_t subKeys[11][16]);

    /**
     * @brief AES encrypt single block
     * @param[in] input plaintext, [length = AES_BLOCK_SIZE]
     * @param[in] subKeys subKeys
     * @param[out] output ciphertext, [length = AES_BLOCK_SIZE]
     */
    void aes_encrypt_block(const uint8_t *input, uint8_t subKeys[11][16], uint8_t *output);

    /**
     * @brief AES decrypt single block
     * @param[in] input ciphertext, [length = AES_BLOCK_SIZE]
     * @param[in] subKeys subKeys
     * @param[out] output plaintext, [length = AES_BLOCK_SIZE]
     */
    void aes_decrypt_block(const uint8_t *input, uint8_t subKeys[11][16], uint8_t *output);

    void aes_padding_encrypt(const uint8_t *input, uint8_t **output, size_t in_len, size_t *out_len, const uint8_t vector[16], const uint8_t key[16]);
    void aes_padding_decrypt(const uint8_t *input, uint8_t *output, size_t in_len, size_t *out_len, const uint8_t vector[16], const uint8_t key[16]);
#ifdef __cplusplus
}
#endif

#endif // AES_H
