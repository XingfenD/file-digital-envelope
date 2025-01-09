#ifndef SM4_H
#define SM4_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

#define SM4_BLOCK_BITS 128 /* bits of AES algoithm block */
#define SM4_BLOCK_SIZE 16  /* bytes of AES algoithm block 16 * 8 = 128 */
#define SM4_KEY_SIZE 16    /* bytes of AES algoithm double key 16 * 8 = 128  */
#define SM4_ROUNDS 32


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

    // void sm4_encrypt_cbc(uint8_t *input, size_t length, uint8_t iv[16], const uint32_t encSubkey[SM4_ROUNDS], uint8_t *output);
    // void sm4_decrypt_cbc(uint8_t *input, size_t length, uint8_t iv[16], const uint32_t decSubkey[SM4_ROUNDS], uint8_t *output);

    /**
     * @brief SM4 encrypt arbitrary length
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
    void sm4_padding_encrypt(const uint8_t *input, uint8_t *output, size_t in_len, size_t *out_len, const uint8_t vector[16], const uint8_t key[16]);

    /**
     * @brief SM4 decrypt arbitrary length
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
    void sm4_padding_decrypt(const uint8_t *input, uint8_t *output,size_t in_len, size_t *out_len,const uint8_t vector[16], const uint8_t key[16]);


#ifdef __cplusplus
}
#endif

#endif // SM4_H
