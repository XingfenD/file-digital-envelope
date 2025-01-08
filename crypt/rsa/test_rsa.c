#include "inc/rsa.h"
#include "inc/keys.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


// Print bytes in hexadecimal format
void print_bytes(const uint8_t *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

static int RSA2048(void){
    int ret;
    rsa_key pk = {0};
    rsa_key sk = {0};
    uint8_t output[256];

    // message to encrypt
    uint8_t input [256] = { 0x21,0x55,0x53,0x53,0x53,0x53};

    unsigned char msg [256];
    uint32_t outputLen, msg_len;
    uint8_t  inputLen;

    // copy keys.h message about public key and private key to the flash RAM
    pk.bits = KEY_M_BITS;
    memcpy(&pk.modulus         [RSA_MAX_MODULUS_LEN-sizeof(key_m) ],  key_m,  sizeof(key_m ));
    memcpy(&pk.exponent        [RSA_MAX_MODULUS_LEN-sizeof(key_e) ],  key_e,  sizeof(key_e ));
    sk.bits = KEY_M_BITS;
    memcpy(&sk.modulus         [RSA_MAX_MODULUS_LEN-sizeof(key_m) ],  key_m,  sizeof(key_m ));
    memcpy(&sk.exponent        [RSA_MAX_MODULUS_LEN-sizeof(key_pe)],  key_pe, sizeof(key_pe));
    

    inputLen = strlen((const char*)input);

    print_bytes(input,6);

    // public key encrypt
    rsa_public_encrypt(output, &outputLen, input, inputLen, &pk);
    print_bytes(output,outputLen);

    // private key decrypt
    rsa_private_decrypt(msg, &msg_len, output, outputLen, &sk);
    printf(",,,,,,,,,,,,,,\n");
    print_bytes(msg,msg_len);

    // private key encrypt
   // rsa_private_encrypt(output, &outputLen, input, inputLen, &sk);

    // public key decrypted
   // rsa_public_decrypt(msg, &msg_len, output, outputLen, &pk);

    return 0;
}
/* RSA2048 function ended */

int main(int argc, char const *argv[])
{
    clock_t start, finish;
    double  duration;
    start = clock();    // init start time
    RSA2048();
    finish = clock();   // print end time
    duration = (double)(finish - start) / CLOCKS_PER_SEC;   // print encrypt and decrypt time
    printf( "%f seconds\n", duration );
    return 0;
}
