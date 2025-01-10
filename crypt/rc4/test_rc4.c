/*
 * @Author: “shoutifeng” “shoutifeng@gmail.com”
 * @Date: 2025-01-08 18:37:34
 * @LastEditors: “shoutifeng” “shoutifeng@gmail.com”
 * @LastEditTime: 2025-01-09 20:06:57
 * @FilePath: \file_digital_envelope\file-digital-envelope\crypt\rc4\test_rc4.c
 */

#include <rc4.h>
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
void test_rc4_correctness()
{
    uint8_t key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                       0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    // Fixed example plaintext 0x0123456789abcdeffedcba9876543210ff
    uint8_t plaintext[17] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                             0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0xff};

    // Corresponding ciphertext 0xAC5BDAFC5B967A10F343EC750FBEBE3B04
    uint8_t correctResult[17] = {0xAC, 0x5B, 0xDA, 0xFC, 0x5B, 0x96, 0x7A, 0x10, 0xF3, 0x43, 0xEC, 0x75, 0x0F, 0xBE, 0xBE, 0x3B, 0x04};

    uint8_t ciphertext[17];
    uint8_t decrypted[17];

    printf("Original plaintext: ");
    print_bytes(plaintext, 17);

    printf("Correct ciphertext: ");
    print_bytes(correctResult, 17);

    // Encrypt
    rc4_encrypt(plaintext, 17, key, ciphertext);
    printf("Encrypted ciphertext: ");
    print_bytes(ciphertext, 17);

    // Decrypt
    rc4_decrypt(ciphertext, 17, key, decrypted);
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
    test_rc4_correctness();

    return 0;
}
