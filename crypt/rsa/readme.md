## 生成密钥

```bash
//生成2048位的私钥
openssl genrsa -out private.pem 2048
 
//将生成的文件转换为文本
openssl rsa -in private.pem -text -out private.txt
 
//将BASE64编码的文件转换为bin文件
openssl   base64  -d  -in private.pem -out private.bin
```
运行`./keys/gen_keys.sh`即可获得密钥对


