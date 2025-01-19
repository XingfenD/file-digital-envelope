/**
 * @file ./utils/src/bin_lib.c
 * @brief realize the functions stated in file:./utils/inc/bin_lib.h
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-02
 *
 * @copyright Copyright (c) 2024
 *
 * the operations related to bit operation and conversion are suggested to be defined in this file
 */

#include <bin_lib.h>

inline uint32_t uint32_left_loop(uint32_t uint32, int num_of_bit) {
    // num_of_bit %= 32; /* this function shouldn't and won't be called with @num_of_byte greater than 4 */
    return (uint32 << num_of_bit) | (uint32 >> (32 - num_of_bit));
}

inline uint32_t uint32_right_loop(uint32_t uint32, int num_of_bit) {
    // num_of_bit %= 32; /* this function shouldn't and won't be called with @num_of_byte greater than 4 */
    return (uint32 >> num_of_bit) | (uint32 << (32 - num_of_bit));
}

inline void word_left_loop_byte(uint32_t * word, int num_of_byte) {
    /**
     * assume the byte array is arr[4] = {0x01, 0x23, 0x45, 0x67}
     * the value of pointer $word is the same as pointer $arr.
     *
     * if we read this address as uint32_t,
     * it will be 0x67452301 instead of 0x01234567
     * the address points to value 0x01
     *
     * to realize the left shift in byte array, we should right shift the uint32_t for 8 bits
     * the result will be 0x01674523, the address points to value 0x23
     *
     * then we read this address as uint8_t[4]
     * it will be {0x23, 0x45, 0x67, 0x01}, so we realize a left shift in byte array
     *
     */

    // num_of_byte %= 4; /* this function shouldn't and won't be called with $num_of_byte greater than 4 */
    *word = (*word >> 8 * num_of_byte) | (*word << (32 - 8 * num_of_byte));
}

inline void word_right_loop_byte(uint32_t * word, int num_of_byte) {
    /**
     * similar with the function above: word_left_loop_byte
     * no further elaboration will be provided
     *
     */

    // num_of_byte %= 4; /* this function shouldn't and won't be called with $num_of_byte greater than 4 */
    *word = (*word << 8 * num_of_byte) | (*word >> (32 - 8 * num_of_byte));
}

inline uint32_t xor_uint8_uint32(const uint8_t bytes[4], const uint32_t word) {
    /**
     * assume the bytes array is bytes[4] = { 0x01, 0x23, 0x45, 0x67 }
     * and the word = 0x01234567
     *
     * if we read the pointer @bytes as (uint32_t *)
     * the result will be 0x67452301
     *
     * and compute the xor with @word directly,
     * the returned value will be not as expected
     *
     * consequently, we should compute the xor of the higher byte in @word and lower index in bytes array
     * and then join them together
     */
    return ((word & 0x000000FF) ^ (uint32_t) bytes[3]) |
        ((((word & 0x0000FF00) >>  8) ^ (uint32_t) bytes[2]) <<  8) |
        ((((word & 0x00FF0000) >> 16) ^ (uint32_t) bytes[1]) << 16) |
        ((((word & 0xFF000000) >> 24) ^ (uint32_t) bytes[0]) << 24)
    ;
}

inline uint32_t bytes2word(const uint8_t src_bytes[4]) {
    /**
     * just as the comments in the function above explains
     * we can't read the pointer @src_bytes as uint32_t directly
     */
    return (((uint32_t) src_bytes[3]) << 0) |
        (((uint32_t) src_bytes[2]) <<  8) |
        (((uint32_t) src_bytes[1]) << 16) |
        (((uint32_t) src_bytes[0]) << 24);
}

inline void word2bytes(uint8_t dst_bytes[4], const uint32_t word) {
    /**
     * the reverse process of byte2word
     */
    dst_bytes[0] = (uint8_t) ((word & 0xFF000000) >> 24);
    dst_bytes[1] = (uint8_t) ((word & 0x00FF0000) >> 16);
    dst_bytes[2] = (uint8_t) ((word & 0x0000FF00) >>  8);
    dst_bytes[3] = (uint8_t) ((word & 0x000000FF) >>  0);
}

// inline void bytes_rev(uint8_t bytes[4]) {
//     uint8_t tmp = bytes[0];
//     bytes[0] = bytes[4];
//     bytes[3] = tmp;
//     tmp = bytes[1];
//     bytes[1] = bytes[2];
//     bytes[2] = tmp;
// }
