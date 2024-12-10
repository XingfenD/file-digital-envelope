/**
 * @file ./inc/bin_lib.h
 * @brief state some basic bits operations
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-02
 *
 * @copyright Copyright (c) 2024
 *
 * the operations related to bit operation and conversion are suggested to be stated in this file
 */

#ifndef BIN_LIB_H
#define BIN_LIB_H

    #include <stdint.h>

    /**
     * @brief loop left shift a uint32 for @num_of_bit bits
     * @param[out]  uint32          the four bytes to be left shifted
     * @param[in]   num_of_bit      the num_of_bit to left shift(< 32)
     */
    uint32_t uint32_left_loop(uint32_t uint32, int num_of_bit);

    /**
     * @brief loop right shift a uint32 for a byte
     * @param[out]  uint32          the four bytes to be right shifted
     * @param[in]   num_of_bit      the num_of_bit to right shift(< 32)
     */
    uint32_t uint32_right_loop(uint32_t uint32, int num_of_bit);

    /**
     * @brief compute the xor of a bytes array and a word
     * @param[in]   bytes           the bytes array
     * @param[in]   word            the word
     * @return uint32_t - the xor result
     */
    uint32_t xor_uint8_uint32(const uint8_t bytes[4], const uint32_t word);

    /**
     * @brief transform a bytes array to a word
     * @param[in]   src_bytes        the bytes array
     * @return uint32_t - the transformation result
     */
    uint32_t bytes2word(const uint8_t src_bytes[4]);

    /**
     * @brief transform a word to a bytes array
     * @param[out]  dst_bytes        the destination bytes array
     * @param[in]   word             the src word
     */
    void word2bytes(uint8_t dst_bytes[4], const uint32_t word);

#endif /* !BIN_LIB_H */