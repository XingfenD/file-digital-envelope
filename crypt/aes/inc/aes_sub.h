/**
 * @file ./crypt/aes/inc/aes_sub.h
 * @brief state the sub functions of aes top functions
 * @author 2022302181113
 * @version 1.0
 * @date 2024-11-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef AES_SUB_H
#define AES_SUB_H

    /**
     * @brief s change use AES_S_Box1 in encrypt subkey-making
     * @param[out]      src_bytes           the word to be mapped
     */
    void aes_encrypt_word_s_map(unsigned char src_bytes[4]);


    /**
     * @brief s change use AES_S_Box2 in decrypt subkey-making
     * @param[out]      src_bytes           the word to be mapped
     */
    void aes_decrypt_word_s_map(unsigned char src_bytes[4]);

    /**
     * @brief s change use AES_S_Box1 in encrypt
     * @param[out]      src_bytes           the src_bytes to be mapped
     */
    void aes_encrypt_s_map(unsigned char src_bytes[16]);

    /**
     * @brief s change use AES_S_Box2 in decrypt
     * @param[out]      src_bytes           the src_bytes to be mapped
     */
    void aes_decrypt_s_map(unsigned char src_bytes[16]);

    /**
     * @brief left shift rows in matrix used in encryption
     * @param[out]      src_bytes           the src_bytes to be left shifted
     */
    void aes_lef_shift_rows(unsigned char src_bytes[16]);

    /**
     * @brief right shift rows in matrix used in decryption
     * @param[out]      src_bytes           the src_bytes to be right shifted
     */
    void aes_rig_shift_rows(unsigned char src_bytes[16]);

    /**
     * @brief column confusion used in encrypt
     * @param[out]      src_bytes           the src_bytes to be confused
     */
    void aes_encrypt_col_confuse(unsigned char src_bytes[16]);

    /**
     * @brief column confusion used in decrypt
     * @param[out]      src_bytes           the src_bytes to be confused
     */
    void aes_decrypt_col_confuse(unsigned char src_bytes[16]);

    /**
     * @brief just as the function name goes
     * @param[out]  src_bytes               the block to add round key
     * @param[in]   round_key               subkey in the current round
     */
    void aes_add_round_key(unsigned char src_bytes[16], unsigned char round_key[16]);

#endif /* !AES_SUB_H */