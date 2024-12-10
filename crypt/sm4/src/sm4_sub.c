/**
 * @file ./lib/sm4_sub.c
 * @brief realize the sub functions stated in file:./inc/aes_sub.h
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <sm4_sub.h>
#include <bin_lib.h>
#include <table_defs.h>

inline uint8_t s_map(const uint8_t bits_8) {
    return SM4_S_Box[bits_8 >> 4][bits_8 & 0x0F];
}

inline uint32_t tao_rep(uint32_t word) {
    uint8_t * pword = (uint8_t *) &word;
    for (int i = 0; i < 4; i++) {
        pword[i] = s_map(pword[i]);
    }
    return word;
}

// inline uint32_t t_trans(uint32_t src_word) {
//     uint32_t rst_of_tao_rep = tao_rep(src_word);

//     return rst_of_tao_rep ^
//         uint32_left_loop(rst_of_tao_rep, 2) ^
//         uint32_left_loop(rst_of_tao_rep, 10) ^
//         uint32_left_loop(rst_of_tao_rep, 18) ^
//         uint32_left_loop(rst_of_tao_rep, 24)
//     ;
// }

inline uint32_t t_rev_trans(uint32_t src_word) {
    uint32_t rst_of_tao_rep = tao_rep(src_word);

    return rst_of_tao_rep ^
        uint32_left_loop(rst_of_tao_rep, 13) ^
        uint32_left_loop(rst_of_tao_rep, 23)
    ;
}

inline uint32_t f_function(uint32_t Xor0, uint32_t xor0, uint32_t xor1, uint32_t xor2, uint32_t round_key) {
    // return Xor0 ^ t_trans(
    //     xor0 ^ xor1 ^ xor2 ^ round_key
    // );

    return Xor0 ^
        SM4_T_Table[0][
            ((uint8_t *) &xor0)[3] ^
            ((uint8_t *) &xor1)[3] ^
            ((uint8_t *) &xor2)[3] ^
            ((uint8_t *) &round_key)[3]
        ] ^
        SM4_T_Table[1][
            ((uint8_t *) &xor0)[2] ^
            ((uint8_t *) &xor1)[2] ^
            ((uint8_t *) &xor2)[2] ^
            ((uint8_t *) &round_key)[2]
        ] ^
        SM4_T_Table[2][
            ((uint8_t *) &xor0)[1] ^
            ((uint8_t *) &xor1)[1] ^
            ((uint8_t *) &xor2)[1] ^
            ((uint8_t *) &round_key)[1]
        ] ^
        SM4_T_Table[3][
            ((uint8_t *) &xor0)[0] ^
            ((uint8_t *) &xor1)[0] ^
            ((uint8_t *) &xor2)[0] ^
            ((uint8_t *) &round_key)[0]
        ];
}