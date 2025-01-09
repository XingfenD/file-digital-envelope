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


## 代码相关
在`./src/read_pem_file.c`的第45行和第64行

```c
//45
if(fopen("/home/ran/work/crypt/crypt/rsa/keys/private.bin", "r"))
//64
flie_buffer = copyFile("/home/ran/work/crypt/crypt/rsa/keys/private.bin", &file_size);
```
文件路径需要改为自己的路径`/your/path/to/crypt/rsa/keys/private.bin`