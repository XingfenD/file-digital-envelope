#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <../inc/elgama.h>

// Global variables
int public_key_part1, public_key_part2;
int prime_modulus, private_key;
int cipher_component1, cipher_component2;
FILE *cipher_file1, *cipher_file2;

// Function to calculate the greatest common divisor
static int calculateGCD(int a, int b);
// Function for fast exponentiation
static void fastExponentiation(int bit, int n, int* result, int* base);
// Function to find T using modular exponentiation
static int findT(int base, int exponent, int modulus);
// Primality test for a given number
static int primalityTest(int base, int number);
// Find a primitive root of a prime number
static int findPrimitiveRoot(int prime);


static void elGamalEncryption(int plaintext,int *cipher1, int *cipher2, ElGamalKeys keys);
static void elGamalDecryption(int component1, int component2, char *deciphered_value, ElGamalKeys keys);

// Key generation for ElGamal
void elGamalKeyGeneration(const char *key_file) {

    FILE *_key_file = fopen(key_file, "w");
    if (_key_file == NULL) {
        printf("无法打开密钥文件\n");
        exit(1); // 打开文件失败，退出程序
    }
    
    ElGamalKeys keys;
    srand(time(NULL));
    do {
        do {
           keys.prime_modulus = rand() + 256;
        } while (keys.prime_modulus % 2 == 0);

    } while (!primalityTest(2, keys.prime_modulus));
   
    keys.prime_modulus = 107;

    keys.public_key_part1 = 2;
    do {
        keys.private_key = rand() % (keys.prime_modulus - 2) + 1; // 1 <= d <= prime_modulus-2
    } while (calculateGCD(keys.private_key, keys.prime_modulus) != 1);

    keys.private_key = 67;
    keys.public_key_part2 = findT(keys.public_key_part1, keys.private_key, keys.prime_modulus);
  
    fprintf(_key_file, "%d %d %d %d\n", keys.prime_modulus, keys.public_key_part1, keys.public_key_part2, keys.private_key);
      
   fclose(_key_file);
}


// ElGamal 加密函数
int* elGamal_enc(char *plaintext,int len,const char *key_filename) {
    // 读取密钥文件
    FILE *key_file = fopen(key_filename, "r");
    if (key_file == NULL) {
        printf("无法读取密钥\n");
        exit(1);
    }

    ElGamalKeys keys; 
    fscanf(key_file, "%d %d %d %d", &keys.prime_modulus, &keys.public_key_part1, &keys.public_key_part2, &keys.private_key);
    fclose(key_file);

    // 返回密文字符串
    int *ciphertext =  (int *)malloc(sizeof(int) * (len*2));

    for(int i = 0;i<len;i++)
    {
        elGamalEncryption(plaintext[i], &ciphertext[i], &ciphertext[i+len], keys);
    }
 
    
    return ciphertext;
}

// ElGamal 解密函数
char* elGamal_dec(int *ciphertext, int len,const char *key_filename) {
    
    // 读取密钥文件
    FILE *key_file = fopen(key_filename, "r");
    if (key_file == NULL) {
        printf("无法读取密钥\n");
        exit(1);
    }

    ElGamalKeys keys;
    
    fscanf(key_file, "%d %d %d %d", &keys.prime_modulus, &keys.public_key_part1, &keys.public_key_part2, &keys.private_key);
    printf("%d\n",len/2);
    fclose(key_file);

    char *plaintext = (char *)malloc(sizeof(char) * (len));
    for(int i = 0;i<len/2;i++)
    {
        elGamalDecryption(ciphertext[i], ciphertext[i+len/2], &plaintext[i], keys);
    }

    return plaintext;

}

// Function to calculate the greatest common divisor
static int calculateGCD(int a, int b) {
    int quotient, remainder1, remainder2, temp;

    if (a > b) {
        remainder1 = a;
        remainder2 = b;
    } else {
        remainder1 = b;
        remainder2 = a;
    }

    while (remainder2 > 0) {
        quotient = remainder1 / remainder2;
        temp = remainder1 - quotient * remainder2;
        remainder1 = remainder2;
        remainder2 = temp;
    }

    return remainder1;
}

// Function for fast exponentiation
static void fastExponentiation(int bit, int n, int* result, int* base) {
    if (bit == 1) {
        *result = (*result * (*base)) % n;
    }

    *base = (*base) * (*base) % n;
}

// Function to find T using modular exponentiation
static int findT(int base, int exponent, int modulus) {
    int remainder;
    int result = 1;

    while (exponent > 0) {
        remainder = exponent % 2;
        fastExponentiation(remainder, modulus, &result, &base);
        exponent = exponent / 2;
    }

    return result;
}

// Primality test for a given number
static int primalityTest(int base, int number) {
    int n = number - 1;
    int k = 0;
    int m, T;

    while (n % 2 == 0) {
        k++;
        n = n / 2;
    }

    m = n;
    T = findT(base, m, number);
    if (T == 1 || T == number - 1) {
        return 1;
    }

    for (int j = 0; j < k; j++) {
        T = findT(T, 2, number);
        if (T == 1) {
            return 0;
        }
        if (T == number - 1) {
            return 1;
        }
    }

    return 0;
}

// Find a primitive root of a prime number
static int findPrimitiveRoot(int prime) {
    int flag;

    for (int root = 2; root < prime; root++) {
        flag = 1;
        for (int i = 1; i < prime; i++) {
            if (findT(root, i, prime) == 1 && i < prime - 1) {
                flag = 0;
            } else if (flag && findT(root, i, prime) == 1 && i == prime - 1) {
                return root;
            }
        }
    }
}


// Encryption using ElGamal
static void elGamalEncryption(int plaintext,int *cipher1, int *cipher2, ElGamalKeys keys) {

    int random_value;
    do {
        random_value = rand() % (keys.prime_modulus - 1) + 1; // 1 < r < prime_modulus
    } while (calculateGCD(random_value, keys.prime_modulus) != 1);
    *cipher1 = findT(keys.public_key_part1, random_value, keys.prime_modulus);
    *cipher2 = findT(keys.public_key_part2, random_value, keys.prime_modulus) * plaintext % keys.prime_modulus;
}

// Decryption using ElGamal
static void elGamalDecryption(int component1, int component2, char *deciphered_value, ElGamalKeys keys) {
    *deciphered_value = component2 * findT(component1, keys.prime_modulus - 1 - keys.private_key, keys.prime_modulus) % keys.prime_modulus;
}