/**
 * @file ./crypt/sm4/test_sm4.c
 * @brief test the correctness of sm4 algorithm
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <sm4.h>
#include <stdio.h>
#include <string.h>

// Print bytes in hexadecimal format
void print_bytes(const uint8_t *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

// Correctness test function
void test_sm4_correctness()
{
    uint8_t vector[SM4_BLOCK_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                                     0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    // Fixed example key  0x0123456789abcdeffedcba9876543210
    uint8_t key[SM4_KEY_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                                 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    // Fixed example plaintext 0x0123456789abcdeffedcba9876543210
    uint8_t plaintext[SM4_BLOCK_SIZE + 1] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                                             0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0xff};

    // Corresponding ciphertext 0x681edf34d206965e86b3e94f536e4246
    uint8_t correctResult[SM4_BLOCK_SIZE * 2] = {0x26, 0x77, 0xf4, 0x6b, 0x09, 0xc1, 0x22, 0xcc, 0x97, 0x55, 0x33, 0x10, 0x5b, 0xd4, 0xa2, 0x2a, 0x36, 0x01, 0xc2, 0x5f, 0x09, 0x6a, 0x8e, 0xeb, 0xb4, 0xe4, 0x8b, 0x49, 0x7b, 0x15, 0xe0, 0xc4};

    uint8_t ciphertext[SM4_BLOCK_SIZE * 2];
    uint8_t decrypted[SM4_BLOCK_SIZE + 1];

    uint32_t encSubKeys[SM4_ROUNDS];
    uint32_t decSubKeys[SM4_ROUNDS];

    // Generate encryption subkeys
    if (sm4_make_enc_subkeys(key, encSubKeys) != 0)
    {
        printf("Failed to generate encryption subkeys.\n");
        return;
    }

    // Generate decryption subkeys
    if (sm4_make_dec_subkeys(key, decSubKeys) != 0)
    {
        printf("Failed to generate decryption subkeys.\n");
        return;
    }

    printf("Original plaintext: ");
    print_bytes(plaintext, SM4_BLOCK_SIZE + 1);

    printf("Correct ciphertext: ");
    print_bytes(correctResult, SM4_BLOCK_SIZE * 2);

    // Encrypt
    size_t *out_len = malloc(sizeof(uint8_t) * 32);
    ;
    sm4_padding_encrypt(plaintext, ciphertext, 17, out_len, vector, encSubKeys);
    printf("Encrypted ciphertext: ");
    print_bytes(ciphertext, SM4_BLOCK_SIZE * 2);

    // Decrypt
    sm4_padding_decrypt(ciphertext, decrypted, 32, out_len, vector, decSubKeys);
    printf("Decrypted plaintext: ");
    print_bytes(decrypted, SM4_BLOCK_SIZE + 1);

    // Verify encryption result
    if ((memcmp(ciphertext, correctResult, SM4_BLOCK_SIZE * 2) == 0) && (memcmp(plaintext, decrypted, SM4_BLOCK_SIZE + 1) == 0))
    {
        printf(">> Correctness test passed.\n\n");
    }
    else
    {
        printf(">> Correctness test failed.\n\n");
    }
}

int main()
{
    // Perform correctness test
    printf(">> Performing correctness test...\n");
    test_sm4_correctness();

    return 0;
}
