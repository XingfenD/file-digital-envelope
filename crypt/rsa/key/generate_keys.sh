#!/bin/bash

# 生成 RSA 私钥
openssl genrsa -out private.pem 2048

# 提取模数 n
n=$(openssl rsa -in private.pem -noout -modulus | cut -d'=' -f2 | xxd -r -p | xxd -i | sed 's/0x/0x/g' | tr -d '\n' | sed 's/,/, /g')

# 提取私钥指数 d
d=$(openssl rsa -in private.pem -text 2>/dev/null | sed -n '/privateExponent:/,/\S/p' | sed '/privateExponent:/d' | tr -d '\n' | sed 's/\(..\)/0x\1,/g' | sed 's/,$//')

# 将结果写入 keys.h 文件
cat <<EOF > ../inc/keys.h
#include <stdint.h>

#define KEY_M_BITS      2048

// RSA Modulus n
uint8_t key_n[] = {
    $n
};

// RSA Public Exponent e
uint8_t key_e[] = {
    0x01, 0x00, 0x01
};

// RSA Private Exponent d
uint8_t key_d[] = {
    $d
};

#endif
EOF

echo "keys.h 文件已生成"
