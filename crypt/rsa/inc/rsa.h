/**
 * @file ./crypt/rsa/inc/rsa.h
 * @brief state the top functions of rsa
 * @author
 * @version 1.0
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef RSA_H
#define RSA_H

    #include <stdint.h>

    /**
     * @brief the function to show the module:rsa is included and compiled correctly
     *
     * TODO: this function should be removed in the final version of this project
     */
    void rsa_print();

    void rsa_padding_encrypt(const uint8_t *input, uint8_t *output, size_t in_len, const uint8_t *key, const size_t key_len);

    void rsa_padding_decrypt(const uint8_t *input, uint8_t *output, size_t in_len, const uint8_t *key, const size_t key_len);

#endif /* RSA_H */