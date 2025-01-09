/*
 * @Author: “shoutifeng” “shoutifeng@gmail.com”
 * @Date: 2025-01-08 18:37:34
 * @LastEditors: “shoutifeng” “shoutifeng@gmail.com”
 * @LastEditTime: 2025-01-09 23:50:25
 * @FilePath: \file_digital_envelope\file-digital-envelope\crypt\zuc\test_zuc.c
 */

#include <zuc.h>
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
void test_zuc_correctness()
{
    uint8_t vector[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                          0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    uint8_t key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                       0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    // Fixed example plaintext 0x0123456789abcdeffedcba9876543210ff
    uint8_t plaintext[17] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                             0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0xff};

    // Corresponding ciphertext 0x1DD73E846E2FBC001AEF9B1857034B25D5
    uint8_t correctResult[17] = {0x1D, 0xD7, 0x3E, 0x84, 0x6E, 0x2F, 0xBC, 0x00, 0x1A, 0xEF, 0x9B, 0x18, 0x57, 0x03, 0x4B, 0x25, 0xD5};

    uint8_t ciphertext[17];
    uint8_t decrypted[17];

    printf("Original plaintext: ");
    print_bytes(plaintext, 17);

    printf("Correct ciphertext: ");
    print_bytes(correctResult, 17);

    // Encrypt
    zuc_encrypt(plaintext, 17, key, vector, ciphertext);
    printf("Encrypted ciphertext: ");
    print_bytes(ciphertext, 17);

    // Decrypt
    zuc_decrypt(ciphertext, 17, key, vector, decrypted);
    printf("Decrypted plaintext: ");
    print_bytes(decrypted, 17);

    // Verify encryption result
    if ((memcmp(plaintext, decrypted, 17) == 0) && (memcmp(ciphertext, correctResult, 17) == 0))
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
    test_zuc_correctness();

    return 0;
}
