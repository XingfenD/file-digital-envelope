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
    #include <stddef.h>

    // RSA key lengths
    #define RSA_MAX_MODULUS_BITS                2048
    #define RSA_MAX_MODULUS_LEN                 ((RSA_MAX_MODULUS_BITS + 7) / 8)
    #define RSA_MAX_PRIME_BITS                  ((RSA_MAX_MODULUS_BITS + 1) / 2)
    #define RSA_MAX_PRIME_LEN                   ((RSA_MAX_PRIME_BITS + 7) / 8)

    // Error codes
    #define ERR_WRONG_DATA                      0x1001
    #define ERR_WRONG_LEN                       0x1002

    typedef uint64_t dbn_t;
    typedef uint32_t bn_t;

    // void generate_rand(uint8_t *block, uint32_t block_len);

    typedef struct {
        uint32_t bits;
        uint8_t  modulus[RSA_MAX_MODULUS_LEN];    //n
        uint8_t  exponent[RSA_MAX_MODULUS_LEN];   //e/d
    } rsa_key;


    typedef struct {
        uint32_t bits;                   /* length in bits of modulus */
        uint8_t  modulus[256];           /* modulus */
        uint8_t  publicExponent[4];      /* public exponent */
        uint8_t  exponent[256];          /* private exponent */
        uint8_t  prime[2][128];          /* prime factors */
        uint8_t  primeExponent[2][128];  /* exponents for CRT */
        uint8_t  coefficient[128];       /* CRT coefficient */
    } rsa_pkey;

    // void read_key_file(const char *key_path, uint8_t **key, size_t *key_len);
    void rsa_encrypt(uint8_t *input, uint8_t *output, size_t in_len, size_t *out_len, uint8_t *key, size_t key_len);
    void rsa_decrypt(uint8_t *input, uint8_t *output, size_t in_len, size_t *out_len, uint8_t *key, size_t key_len);

#endif /* !RSA_H */
