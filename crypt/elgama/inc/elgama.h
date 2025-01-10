#ifndef ELGama_H
#define ELGama_H

#include <stdint.h>

typedef struct {
    int prime_modulus;  // 素数模数 p
    int public_key_part1;  // 公钥的一部分 g
    int public_key_part2;  // 公钥的另一部分 g^x mod p
    int private_key;  // 私钥 x
} ElGamalKeys;

void elGamalKeyGeneration(const char *key_file);

int* elGamal_enc(char *plaintext, int len, const char *key_filename);

char* elGamal_dec(int *ciphertext, int len, const char *key_filename);


#endif  
