#include "inc/rsa.h"
#include <stddef.h>
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


    // message to encrypt
    uint8_t input [256] = "ncncncncn";

    uint8_t *pub_key = NULL;
    size_t key_len;
    read_key_file("D:\\Project\\psd-lab\\file-digital-envelope\\crypt\\rsa\\keys\\private.bin", &pub_key, &key_len);

    unsigned char output [256];
    unsigned char msg [256];
    size_t outputLen, msg_len;
    size_t  inputLen;

    inputLen = strlen((const char*)input);

    print_bytes(input,inputLen);

    // public key encrypt
    rsa_encrypt(input, output, inputLen, &outputLen, pub_key, key_len);

    // print_bytes(output,outputLen);

    // private key decrypt
    rsa_decrypt(output, msg, outputLen, &msg_len, pub_key, key_len);
    printf("%d\n",msg_len);
    //print_bytes(msg,msg_len);
    printf("%s\n",msg);


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
