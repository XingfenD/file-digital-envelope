/**
 * @file ./inc/sm4_sub.h
 * @brief state the sub functions of sm4 top functions
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef SM4_SUB_H
#define SM4_SUB_H

    #include <stdint.h>

    /**
     * @brief s map function
     * @param[in]   bits_8           the byte to be mapped
     * @return uint8_t - the mapped result
     * @details
     * assume the bits_8 = 0xEF
     * the result will be S_Box[0xE][0xF]
     */
    uint8_t s_map(const uint8_t bits_8);

    /**
     * @brief τ replace - the topper layer of s map, consisting of 4 parallel S_Box
     * @param[in]   src_bytes        the word to be mapped
     * @return uint32_t - the mapped result
     */
    uint32_t tao_rep(uint32_t word);

    /**
     * @brief t transform - the topper layer of tao_rep
     * @param[in]   src_word         the word to be transformed
     * @return uint32_t - the transformation result
     */
    // uint32_t t_trans(uint32_t src_word);

    /**
     * @brief the reverse process of t_trans
     * @param[in]   src_word         the word to be transformed
     * @return uint32_t - the transformation result
     */
    uint32_t t_rev_trans(uint32_t src_word);

    /**
     * @brief compute Xor0 ^ t_trans(xor0 ^ xor1 ^ xor2 ^ round_key)
     * @param[in]   Xor0             the outer xor arg
     * @param[in]   xor0             the inner xor arg
     * @param[in]   xor1             the inner xor arg
     * @param[in]   xor2             the inner xor arg
     * @param[in]   round_key        the inner xor arg
     * @return uint32_t 
     */
    uint32_t f_function(
        uint32_t Xor0,
        uint32_t xor0,
        uint32_t xor1,
        uint32_t xor2,
        uint32_t round_key
    );

#endif /* !SM4_SUB_H */
