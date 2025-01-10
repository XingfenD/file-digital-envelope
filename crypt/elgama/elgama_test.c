#include </home/ran/work/crypt/crypt/elgama/inc/elgama.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void print_bytes(const uint8_t *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

int main() {
    

    elGamalKeyGeneration("keys.txt");

    char *plaintext_ = "00000000000nnnn";
    int size = strlen(plaintext_);

    print_bytes((uint8_t *)plaintext_, size);

    int *ciphertext = elGamal_enc(plaintext_,size, "keys.txt");

   // print_bytes((uint8_t *)ciphertext, 2*size);


    char *plaintext = elGamal_dec(ciphertext, 2*size,"keys.txt");
    size_t size2 = strlen(plaintext);
    print_bytes((uint8_t *)plaintext, size);
    free(plaintext);
    free(ciphertext);
    return 0;
}

