##!/bin/bash

# 生成 2048 位的私钥并保存为 private.pem
openssl genrsa -out private.pem 2048

# 将生成的私钥文件转换为文本格式并保存为 private.txt
openssl rsa -in private.pem -text -out private.txt

# 将 private.pem 文件中的 BASE64 编码转换为二进制格式并保存为 private.bin
openssl base64 -d -in private.pem -out private.bin

# 提示用户脚本执行完毕
echo "RSA 私钥生成与转换完成！"
