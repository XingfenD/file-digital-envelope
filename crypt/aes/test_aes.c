#include <aes.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
// Print bytes in hexadecimal format
void print_bytes(const unsigned char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}


// Correctness test function
void test_aes_correctness()
{
    // Fixed example key  0x00012001710198aeda79171460153594  
    unsigned char key[AES_KEY_SIZE] = {0x00, 0x01, 0x20, 0x01, 0x71, 0x01, 0x98, 0xae, 
    0xda, 0x79, 0x17, 0x14, 0x60, 0x15, 0x35, 0x94};
    // Fixed example plaintext 0x0001000101a198afda78173486153566 
    unsigned char plaintext[AES_BLOCK_SIZE] = {0x00, 0x01, 0x00, 0x01, 0x01, 0xa1, 0x98, 0xaf,
    0xda, 0x78, 0x17, 0x34, 0x86, 0x15, 0x35, 0x66};

    // Corresponding ciphertext 0x6cdd596b8f5642cbd23b47981a65422a
    unsigned char correctResult[AES_BLOCK_SIZE] = {0x6c, 0xdd, 0x59, 0x6b, 0x8f, 0x56, 0x42, 0xcb,
    0xd2, 0x3b, 0x47, 0x98, 0x1a, 0x65, 0x42, 0x2a};

    unsigned char ciphertext[AES_BLOCK_SIZE];
    unsigned char decrypted[AES_BLOCK_SIZE];

    unsigned char encSubKeys[11][16];
    unsigned char decSubKeys[11][16];

    // Generate encryption subkeys
    if (aes_make_enc_subkeys(key, encSubKeys) != 0)
    {
        printf("Failed to generate encryption subkeys.\n");
        return;
    }
    // Generate decryption subkeys
    if (aes_make_dec_subkeys(key, decSubKeys) != 0)
    {
        printf("Failed to generate decryption subkeys.\n");
        return;
    }

    printf("Original plaintext: ");
    print_bytes(plaintext, AES_BLOCK_SIZE);

    printf("Correct ciphertext: ");
    print_bytes(correctResult, AES_BLOCK_SIZE);

    // Encrypt
    aes_encrypt_block(plaintext, encSubKeys, ciphertext);
    printf("Encrypted ciphertext: ");
    print_bytes(ciphertext, AES_BLOCK_SIZE);

    // Decrypt
    aes_decrypt_block(ciphertext, decSubKeys, decrypted);
    printf("Decrypted plaintext: ");
    print_bytes(decrypted, AES_BLOCK_SIZE);

    // Verify encryption result
    if ((memcmp(ciphertext, correctResult, AES_BLOCK_SIZE) == 0) && (memcmp(plaintext, decrypted, AES_BLOCK_SIZE) == 0))
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
    test_aes_correctness();

    return 0;
}


