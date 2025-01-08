#include <rc4.h>
#include <table_defs.h>
#include <malloc.h>
#include <utils.h>

#define SWAP(x, y)   \
    {                \
        int tmp = x; \
        x = y;       \
        y = tmp;     \
    }

/**
 * @brief init the Sbox with key
 * @param key
 * @param key_len
 */
void rc4_init(uint8_t *key, uint32_t key_len)
{
    uint32_t i;
    for( i=0; i<256; i++ )
    {
        RC4_S_BOX[i] = (uint8_t)i;
    }
    uint32_t j = 0;
    for (i = 0; i < 256; i++)
    {
        /* code */
        j = (j + RC4_S_BOX[i] + key[i % key_len]) % 256;
        SWAP(RC4_S_BOX[i], RC4_S_BOX[j]);
    }
}

/**
 * @brief rc4 encrypt
 * @param input input data
 * @param key key
 * @param output output data
 */
void rc4_encrypt(uint8_t *input, size_t in_len, uint8_t *key, uint8_t *output)
{
    rc4_init(key, 256);
    uint32_t i = 0, j = 0;
    uint8_t k[256];
    for (size_t n = 0; n < in_len; n++)
    {
        /* code */
        i = (i + 1) % 256;
        j = (j + RC4_S_BOX[i]) % 256;
        SWAP(RC4_S_BOX[i], RC4_S_BOX[j]);
        k[n] = RC4_S_BOX[(RC4_S_BOX[i] + RC4_S_BOX[j]) % 256];
        output[n] = input[n] ^ k[n];
    }
}

/**
 * @brief
 * @param input input data
 * @param key key
 * @param output output data
 */
void rc4_decrypt(uint8_t *input, size_t in_len, uint8_t *key, uint8_t *output)
{
    rc4_init(key, 256);
    uint32_t i = 0, j = 0;
    uint8_t k[256];
    for (size_t n = 0; n < in_len; n++)
    {
        /* code */
        i = (i + 1) % 256;
        j = (j + RC4_S_BOX[i]) % 256;
        SWAP(RC4_S_BOX[i], RC4_S_BOX[j]);
        k[n] = RC4_S_BOX[(RC4_S_BOX[i] + RC4_S_BOX[j]) % 256];
        output[n] = input[n] ^ k[n];
    }
}