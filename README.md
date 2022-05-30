# 2022NJU计算机系统基础课程PA
* 本代码仅供参考，请遵守`LICENSE`，请勿抄袭。
## 如何将提供的代码拷贝到自己的Linux运行（以下步骤仅供参考）
1. Linux安装32位支持库、libsdl1.2-dev等支持库（可自行百度）。
2. 或者可以使用docker避免环境配置。[使用docker运行nemu环境](./docker/README.md)
3. nemu/src/cpu/fpu.c的第4行或者nemu/src/cpu/cpu.c的第11行都有`FPU fpu`，删除其中一个，我删除的是cpu.c里的。
4. 若要使用老师提供的nemu/Makefile，在`CFLAGS`和`LDFLAGS`后面加上` -m32`。
5. 推荐使用cmake，在Linux下安装cmake，在VS Code中安装cmake、C/C++插件。

![](https://s3.bmp.ovh/imgs/2022/05/28/4e048934246380d4.png)

6. 将nemu/include/cpu/alu.h中的
```c
inline uint32_t sign_ext(uint32_t x, size_t data_size)
{
    assert(data_size == 16 || data_size == 8 || data_size == 32);
    switch (data_size)
    {
    case 8:
        return (int32_t)((int8_t)(x & 0xff));
    case 16:
        return (int32_t)((int16_t)(x & 0xffff));
    default:
        return (int32_t)x;
    }
}

inline uint64_t sign_ext_64(uint32_t x, size_t data_size)
{
    assert(data_size == 16 || data_size == 8 || data_size == 32);
    switch (data_size)
    {
    case 8:
        return (int64_t)((int8_t)(x & 0xff));
    case 16:
        return (int64_t)((int16_t)(x & 0xffff));
    default:
        return (int64_t)((int32_t)x);
    }
}
```
剪切到nemu/src/cpu/alu.c，并在nemu/include/cpu/alu.h加上
```c
uint32_t sign_ext(uint32_t x, size_t data_size);
uint64_t sign_ext_64(uint32_t x, size_t data_size);
```
7. 新建文件CMakeLists.txt(见本工程中的CMakeLists.txt)。
8. 界面左下角改为debug模式。

![](https://s3.bmp.ovh/imgs/2022/05/28/6332d67634704de1.png)

9. 由于运行需要参数，vs code默认不给参数，因此可根据debug的内容，在`.vscode/settings.json`中加入如下配置(同时保证运行路径的正确)：
```json
{
    "cmake.debugConfig": {
        "args": [
            "--autorun",
            "--testcase",
            "fib"
        ],
        "cwd": "/home/do1e/Arch/OneDrive - njuee/20212大三下/计算机系统基础/PA/NJU-nemu"
    }
}
```

10. 快乐地debug或者run吧。

![](https://s3.bmp.ovh/imgs/2022/05/28/85119b27e4d72ae6.png)

**注1：由于不同编译器对未定义行为的操作可能不同，强烈建议结果不对且没发现明显错误时，在PA平台上测试一次。正确的话可以更改`\*_test.c`跳过该测试用例。比如PA1-3中便有不同编译器对nan的解释不同。我选择的编译器为clang，gcc遇到过怪问题。**  
注2：kernel中的CMakeLists.txt并不能编译kernel，只用作kernel代码高亮。   
注3：强烈建议kernel使用平台编译，本地编译容易出现问题。
