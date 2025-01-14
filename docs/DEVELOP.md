# file-digital-envelope develop document

## 项目环境

项目使用的库将是windows和linux共有的库，因此在在两种环境中开发都可以。

但是makefile中的指令是bash指令，因此如果想要构建项目，应该在git bash中构建（在git bash中运行make命令）。

### vscode插件建议

- Doxygen Documentation Generator 用于自动生成文件头与函数注释，键入`/*`后回车触发
- GitLess 可视化Git工具，可以用于对比不同commit之间的区别
- Git Graph 可视化Git工具，以图的形式展现多个branch与commit之间的关系与变化
- Todo Tree 列出项目中的`TODO:`，`NOTE:`等关键字

注：部分插件需要手动配置。

## 英文缩写说明

|        | ASY                      | SYM       | algo      |
| ------ | ------------------------ | --------- | --------- |
| 原英文 | asymmetric               | symmetric | algorithm |
| 含义   | 非对称加密（公私钥加密） | 对称加密  | 算法      |

## 头文件、源文件、函数命名规范与注释

在这一节内，同名的.c和.h文件被称为一个模块，如`sm4.h`和`sm4.c`这一对被认为是sm4模块。

在每一个头文件内，头文件的正式内容应该被如下的宏语句对包裹：

for example, 在sm4.h文件内：

~~~c
#ifndef SM4_H
#define SM4_H

    // the formal content of this head file

#endif /* !SM4_H */
~~~

宏名称为头文件名的大写，并将dot替换为下划线。

`/* !SM4_H */`意为在未定义宏`SM4_H`时候，编译并执行其上方的分支代码块，惊叹号为非的含义。

应当谨慎在头文件中添加暴露在全局中的函数。

1. 如果在实现密码算法时，为了优化代码结构，需要将部分子函数拆分到另一个模块中，
则这部分子函数应当**以密码算法名称作为前缀**。(如`sm4_subfunction()`)
2. 在实现功能时，定义的仅在该文件内调用的函数，应当定义在同文件内并使用`static`修饰
3. 不同的密码算法中可以有相同的模块名，单定义的全局函数不能重名 - 仍建议使用**算法名称前缀修饰模块名**以避免重名，否则很可能会出现bug

具体可参考sm4算法内实现的sm4模块与sm4_sub模块。

### 注释建议(非硬性要求)

项目中的函数、文件头注释使用vscode插件`Doxygen Documentation Generator`自动生成

#### 文件头注释

文件头是在一个文件的开始处的注释，注释形如：

~~~c
/**
 * @file ./main.c
 * @brief the main function of FDE tool
 * @author 2022302181113
 * @version 1.0
 * @date 2024-12-09
 *
 * @copyright Copyright (c) 2024
 *
 */
~~~

`@file`字段标注当前文件相对于目录的相对路径

其他字段均为自动生成`@author`字段可以自己配置

#### 函数接口注释

函数注释在函数的声明处给出（即头文件处），如果是只在同文件内调用的静态函数，则在定义出给出。

函数接口注释形如:

~~~c
    /**
     * @brief parse the fde file head and output the @crypted_key and @crypted_text
     * @param[in]   fde_file_path           the path to the fde file
     * @param[out]  file_ext_name           the file extension encoded in fde file
     * @param[out]  parse_rst               more output stored in @ref ParseRst
     * @return int - the status code
     * 0 - returns normally
     *
     * @note
     * NOTE: the caller should malloc 17 bytes to $file_ext_name before calling
     *
     * NOTE: the caller should free the pointer in $parse_rst
     *
     * NOTE: the caller should pass address of two size_t $parse_rst->$crypted_key_len and $crypted_text_len
     *
     * we should malloc and asign an address to two (uint8_t*) $crypted_key and $crypted_text,
     * conseqently, we pass the address of (uint8_t*)
     */
    int parse_fde_file(const char *fde_file_path, char *file_ext_name, ParseRst *parse_rst);
~~~

应当使用`@brief`指明函数的作用，`@param[in]`、`@param[out]`指明参数及其传递方向，`@return`指明函数返回值类型及意义。

使用`@note`指明调用函数的注意事项，如应当给哪些变量分配多大的内存空间，在函数内部会给哪个变量分配内存空间，在函数外部需要给哪个变量释放内存空间。

需要时可以指明为什么要使用该类型的参数。

如果在函数注释内使用函数涉及的变量名，使用`@`或者`$`**修饰这个变量名**。


#### 其他注释

对函数功能、语句功能的注释应当使用`/* */`而不是`//`。

使某一个语句不起作用的注释，使用`//`。

如：

~~~c
int main () {
    /* define a variable i */
    int i = 0;

    printf("%d\n", i); /* print i */
    // printf("%d\n", i + 1); /* print i + 1 */

    return 0; /* return */
}
~~~

## 项目结构

~~~plaintext
file-digital-envelope
│  .gitignore
│  DEVELOP.md
│  main.c
│  Makefile
│
├─crypt
│  ├─rsa
│  └─sm4
│
├─IOfile
│      plaintext.txt
│      template.fde
│
├─test
│      not_compile_c
│      test_demo.c
│
└─utils
    ├─inc
    │      bin_lib.h
    │      crypt.h
    │      debug.h
    │      defs.h
    │      main_callee.h
    │      table_defs.h
    │      utils.h
    │
    └─src
            bin_lib.c
            debug.c
            main_callee.c
            utils.c
~~~

### crypt目录及密码算法实现

crypt为密码算法子目录，其下可新建任意数量子文件夹（会自动被Makefile识别并编译），子文件夹命名为密码算法名称，子文件夹内应当有inc和src两个目录，\*.h文件全部放在inc内，**不含main函数**的\*.c文件全部放在src目录内。

如果需要构建可执行文件以测试密码算法，应当**在密码算法名称（如sm4）文件夹目录下**执行如下操作：

- 新建Makefile文件
- 新建test_sm.c（含main函数）文件
- cd到密码算法文件夹下执行`make`命令

Makefile的具体内容可参照项目内已经实现的sm4密码算法**修改**，且Makefile文件内标注了具体哪些位置需要修改。

用以构建密码算法可执行文件的Makefile将会包含./utils内的c程序，因此你可以使用在utils目录下定义的变量与函数。（但是并不会包含其他密码算法目录下的文件）

### temp目录介绍

temp目录被写入.gitignore文件，如果想要使用它，应当自行新建该文件夹。其内的任何文件都不会被Makefile编译。

你可以使用它方便的暂时存储某些文件

### test目录

test目录下的所有*.c文件都会项目根目录下的Makefile编译为同名可执行文件（到./build/bin目录下），因此，其内的每一个*.c文件都应该有独立的main函数，且这些c文件之间应当是独立的，没有相互的调用关系。

且所有的*.c文件都与main.c文件等价，也就是说，你可以在test目录下的*.c文件中调用crypt目录下的密码算法与utils目录下的工具函数。

在项目根目录下运行`make test`以将test目录下的*.c文件编译为可执行文件。该命令会在编译结束后自动运行所有*.c文件。

如果不想运行该目录下的某些文件，将`.c`后缀修改为`_c`后缀。

具体可参考test目录下的`test_demo.c`与`not_compile_c`文件。

### utils目录

声明并实现项目中所需的非密码算法函数。

如果要添加的模块功能较多，需要单独写作一个模块，在inc中添加头文件，src中添加源文件。

新增模块内的函数定义、命名规则与前文密码算法中规定的类似：

1. 函数名前添加统一的前缀
2. 减少暴露在全局作用域的函数
3. 不暴露在全局作用域的函数无需写在头文件内，使用static修饰

#### debug.h及debug.c

用于方便的打印调试信息。

其内实现了一个与printf几乎等价的函数debug_print，如果想要自定义地打印格式化字符串，调用debug_print而不是printf。

基于debug_print，你可以在debug库中自己实现打印调试信息的函数。

如果想禁用debug打印，注释掉在debug.h中定义的宏`DEBUG_ON`，然后重新构建项目，（无需注释在项目中调用的debug_print函数），即可在不输出调试信息的情况下运行项目。

#### bin_lib.h及bin_lib.c

用于定义一些涉及底层位运算的常用操作。

#### utils.h及utils.c

用于定义一些其他工具函数。

#### table_defs.h

定义密码算法需要的table，修改该文件的规范在文件内有详细说明。

该文件内定义的为全局变量，应当严格遵守文件修改规范。

#### main_callee.h及main_callee.c

定义仅在main函数调用的main函数的子函数（为了复用部分代码以及适度简化main函数）。

#### defs.h

宏定义错误码，运行模式码，加密算法代码（cipher_algo字段，后文将会提及），以及相关的宏函数。

#### crypt.h

用于包含所有加密模块的头文件的头文件，包含了此文件即无需在main.c中包含具体加密模块的头文件。

## 文件数字信封

这一节具体介绍文件数字信封的概念。

数字信封大体上由两部分组成：被非对称算法加密的密钥，被对称算法加密的密文。

其中密钥部分是对称算法的密钥，也就是用于解密密文的密钥。

也就是给你两个上锁的盒子，一个盒子内放着打开另一个盒子的钥匙。

后面把这两个盒子称为钥匙盒子与密文盒子。你手中有着打开钥匙盒子的钥匙。

### 为什么要多此一举

前面已经说过，数字信封的两部分采用不同的加密算法。

其中钥匙盒子采用非对称加密算法加密，而密文盒子采用对称算法加密。

这样文件数字信封就同时具有了非对称加密算法的便利性，与对称加密算法的运行效率。

（用非对称算法加密的密钥长度远小于实际数据）。

### PKCS#7填充模式（实现比较容易但是最终可能不会使用这个填充方式）

由于加密算法一般都是按块加密的，因此实际应用中很容易出现实际要加密的明文长度不是块的整数倍的情况。

我们这里采用PKCS#7填充模式，也就是明文不足一个block时，在明文尾部添加字节，补齐一个块，每一个字节的内容都是缺少的字节数

如一个块的大小为16个字节时，文件尾部不足一个块的数据为`FF FF FF FF FF FF FF FF FF FF`，可以发现我们的数据缺少了6个字节。

那么使用PKCS#7填充模式填充后的数据为`FF FF FF FF FF FF FF FF FF FF 06 06 06 06 06 06`。

容易知道，有可能会出现某些未填充数据恰好与不同数据填充之后相同的情况。

如假设一段数据末尾恰好是一个块，且为`FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 01`时，

它与`FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF`的填充结果恰好相同。

为了避免这种冲突，如果需要填充的原始数据恰好是一个块的整数倍，我们会添加另一个完整的块作为填充，这个块内的每一个字节都是0x10。即填充了16个16。

### 文件数字信封的具体设计

我们将文件数字信封设计为拓展名为fde的二进制文件。

根据前文所言，我们知道文件数字信封应该至少包含两个部分：被加密的密钥，与被加密的明文。

为了保证解析文件数字信封的便利性与鲁棒性我们将在文件首部添加一个section，称为head section。

head section的字段在`./utils/utils.h`文件中有定义：

~~~c
    /**
     * @brief the head section structure of fde file
     * @param file_type     0x00 ~ 0x02: a string of 3 characters - must be "FDE"
     * @param origin_ext    0x03 ~ 0x12: a string of the extension of the original filename
     * @param cipher_algo     0x13 ~ 0x13: high four bits refers the asymmetric encryption, low four bits refers the symmetric encryption
     * @param asy_info_len  0x14 ~ 0x15: bytes-num of other infomation used in asymmetric encryption
     * @param sym_info_len  0x16 ~ 0x17: bytes-num of other infomation used in symmetric encryption
     * @param sym_key_len   0x18 ~ 0x19: bytes-num of the encrypted symmetric key
     * @param reserved      0x1A ~ 0x1F: reserved bytes for extension
     * @details
     * the structure of a fde file
     *
     * head section - this structure                                            - 26 bytes long
     * key section  - the symmetric key ( encrypted by asymmetric encryption )  - @sym_key_len bytes long
     * info section - the other infomation used in encrypt algorithm            - @sym_info_len + @asy_info_len bytes long
     * ciphertext   - the ciphertext encrypted by symmetric key                 - the rest bytes of the file
     */
    typedef struct _fde_head {
        uint8_t file_type[3];   /* a string of 3 characters - must be "FDE" */
        uint8_t origin_ext[16]; /* the orgin file name' extension */
        uint8_t cipher_algo;      /* high four bits refers the asymmetric encryption, low four bits refers the symmetric encryption */
        uint16_t asy_info_len;  /* bytes-num of other infomation used in asymmetric encryption */
        uint16_t sym_info_len;  /* bytes-num of other infomation used in symmetric encryption */
        uint16_t sym_key_len;   /* bytes-num of the encrypted symmetric key */
        uint8_t reserved[6];    /* reserved for futural extension */
    } FDE_HEAD;
~~~

1. fde文件的前3个字节，是一个固定字符串"fde"， 读取fde文件时应当校验这个字段，以避免错误解析
2. 紧接着的16个字节，存储原文件的拓展名
3. 下一个字节cipher_algo字段，用于标注加密部分使用的算法，前4个bit用于标注使用的非对称加密算法，后4个比特用于标注对称加密算法（具体如何对应在`./utils/inc/defs.h`定义）
4. 后两个字节asy_info_len字段，用于标注`在非对称加密中使用的其他信息`在该fde文件中所占字节数，为了便于读取，这里定义为uint16_t而不是uint8_t[2]
5. 后两个字节sym_info_len字段，用于标注`在对称加密中使用的其他信息`在该fde文件中所占字节数，也定义为uint16_t
6. 后两个字节sym_key_len字段，用于标注被加密的密钥在该fde文件中所占字节数，也定义为uint16_t
7. 最后一个字段reserved占6个字节，保留字段，供未来可能的拓展

根据字段定义，解析（解密）一个fde文件的流程大致应如下：

1. 使用fopen函数二进制读取("rb"模式)该文件
2. 判断文件的大小，如果大小不足sizeof(FDE_HEAD)字节则报错并退出
3. 使用fread函数将文件的前sizeof(FDE_HEAD)个字节写入一个`FDE_HEAD`结构体中
4. 判断file_type字段是否为"FDE"，如果不是：报错并退出
5. 基于cipher_algo字段得出该fde文件使用的两种加密算法
6. 基于asy_info_len，sym_info_len，sym_key_len字段，使用fread函数再次向后读取对应的字节数，并写入对应的地址
7. 使用拥有的私钥（用户输入或读取文件）解密该对称密钥
8. 读取文件剩下的所有字节，作为密文，使用解密出的对称密钥解密该密文

## 命令行工具功能

### 基础功能

实现基于rsa和sm4两种算法的文件数字信封，视情况决定是否添加、迁移其他加密算法。

### 命令行参数

| -f | -o  | -k  | -m | -h | -a | -s |
| -- |-----|-----|----| -- | -- | -- |
| 输入文件路径 | 输出文件路径  | 非对称密钥的文件路径 | 运行模式<br>可选值:[enc, dec] | 打印帮助 | 指定非对称加密算法 | 指定对称加密算法 |
| 必须   | 可选，默认为把拓展名替换为fde文件内存储的原拓展名 |  必须  | enc将输入文件加密为数字信封<br>dec将输入的文件数字信封解密 | 打印帮助 | 可选值由./utils/inc/defs.h::G_ASY_NAMES定义 |可选值由./utils/inc/defs.h::G_SYM_NAMES定义|
