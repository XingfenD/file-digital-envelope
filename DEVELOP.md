# file-digital-envelope

## 项目结构

~~~plaintext
file-digital-envelope
├─crypt
│  ├─aes
│  │  ├─inc
│  │  └─src
│  └─rsa
│      ├─inc
│      └─src
├─temp
├─test
└─utils
    ├─inc
    └─src
~~~

### crypt目录及密码算法实现

crypt为密码算法子目录，其下可新建任意数量子文件夹（会自动被Makefile识别并编译），子文件夹命名为密码算法名称，子文件夹内应当有inc和src两个目录，\*.h文件全部放在inc内，**不含main函数**的\*.c文件全部放在src目录内。

如果需要构建可执行文件以测试密码算法，应当在密码算法名称（如sm4）文件夹目录下新建Makefile文件、test_aes.c文件，cd到密码算法文件夹下执行make命令，Makefile的具体内容可参照项目内已经实现的sm4密码算法修改