/*
 * @Author: “shoutifeng” “shoutifeng@gmail.com”
 * @Date: 2025-01-08 18:37:34
 * @LastEditors: “shoutifeng” “shoutifeng@gmail.com”
 * @LastEditTime: 2025-01-09 18:02:56
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
    uint8_t key[128] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                        0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};
    // Fixed example plaintext 0x0123456789abcdeffedcba9876543210ff
    uint8_t plaintext[136] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe,
                              0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0xff};

    uint8_t ciphertext[136];
    uint8_t decrypted[136];

    printf("Original plaintext: ");
    print_bytes(plaintext, 17);

    // Encrypt
    rc4_encrypt(plaintext, 17, key, ciphertext);
    printf("Encrypted ciphertext: ");
    print_bytes(ciphertext, 17);

    // Decrypt
    rc4_decrypt(ciphertext, 17, key, decrypted);
    printf("Decrypted plaintext: ");
    print_bytes(decrypted, 17);

    // Verify encryption result
    if ((memcmp(plaintext, decrypted, 17) == 0))
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
