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
    // Fixed example key  0x0123456789abcdeffedcba9876543210
    uint8_t key[SM4_KEY_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
    0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    // Fixed example plaintext 0x0123456789abcdeffedcba9876543210
    uint8_t plaintext[SM4_BLOCK_SIZE] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
    0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

    // Corresponding ciphertext 0x681edf34d206965e86b3e94f536e4246
    uint8_t correctResult[SM4_BLOCK_SIZE] = {0x68, 0x1e, 0xdf, 0x34, 0xd2, 0x06, 0x96, 0x5e,
    0x86, 0xb3, 0xe9, 0x4f, 0x53, 0x6e, 0x42, 0x46};

    uint8_t ciphertext[SM4_BLOCK_SIZE];
    uint8_t decrypted[SM4_BLOCK_SIZE];

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
    print_bytes(plaintext, SM4_BLOCK_SIZE);

    printf("Correct ciphertext: ");
    print_bytes(correctResult, SM4_BLOCK_SIZE);

    // Encrypt
    sm4_encrypt_block(plaintext, encSubKeys, ciphertext);
    printf("Encrypted ciphertext: ");
    print_bytes(ciphertext, SM4_BLOCK_SIZE);

    // Decrypt
    sm4_decrypt_block(ciphertext, decSubKeys, decrypted);
    printf("Decrypted plaintext: ");
    print_bytes(decrypted, SM4_BLOCK_SIZE);

    // Verify encryption result
    if ((memcmp(ciphertext, correctResult, SM4_BLOCK_SIZE) == 0) && (memcmp(plaintext, decrypted, SM4_BLOCK_SIZE) == 0))
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


