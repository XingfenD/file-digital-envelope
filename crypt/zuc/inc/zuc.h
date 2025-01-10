#ifndef ZUC_H
#define ZUC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>

#define ZUC_KEY_SIZE 16
#define ZUC_IV_SIZE 16
    /* ZUC_LFSR */
    uint32_t ZUC_LFSR[32];

    /* R1和R2 */
    uint32_t ZUC_R1;
    uint32_t ZUC_R2;

    /**
     * @brief
     * @param key
     * @param iv
     */
    void zuc_init(const uint8_t *key, const uint8_t *iv);

    /**
     * @brief generate keyword
     * @return keyword
     */
    uint32_t zuc_generate_keyword();

    /**
     * @brief generate keystream
     * @param nwords number of words to generate
     * @param keystream generated keystream
     */
    void zuc_generate_keystream(size_t nwords, uint32_t *keystream);

    /**
     * @brief zuc_encrypt
     * @param input input data
     * @param inlen input data length
     * @param key key for encryption
     * @param iv initialization vector
     * @param output output data
     */
    void zuc_encrypt(const uint8_t *input, size_t inlen, const uint8_t *key, const uint8_t *iv, uint8_t *output);

    /**
     * @brief zuc_decrypt
     * @param input input data
     * @param inlen input data length
     * @param key key for decryption
     * @param iv initialization vector
     * @param output output data
     */
    void zuc_decrypt(const uint8_t *input, size_t inlen, const uint8_t *key, const uint8_t *iv, uint8_t *output);

#ifdef __cplusplus
}
#endif

#endif // SM4_H
