# 2022NJU计算机系统基础课程PA
* 本代码仅供参考，请遵守`LICENSE`，请勿抄袭。
## 如何将提供的代码拷贝到自己的Linux运行
1. Linux安装32位支持库、libsdl1.2-dev（可自行百度）。
2. nemu/src/cpu/fpu.c的第4行或者nemu/src/cpu/cpu.c的第11行都有`FPU fpu`，删除其中一个，我删除的是cpu.c里的。
3. 若要使用老师提供的nemu/Makefile，在`CFLAGS`和`LDFLAGS`后面加上` -m32`。
4. 推荐使用cmake，在Linux下安装cmake，在vs code中安装cmake、C/C++插件。
5. 将nemu/include/cpu/alu.h中的
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
6. 在nemu下新建文件CMakeLists.txt，内容如下：
```cmake
cmake_minimum_required(VERSION 3.10.2)
# Project name
PROJECT(nemu)
# Head file path
INCLUDE_DIRECTORIES(
	${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR}/../libs/nemu-ref/include
)
# Source directory: src/
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/cpu DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/cpu/decode DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/cpu/instr DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/cpu/test DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/device DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/device/dev DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/device/io DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/memory DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/memory/mmu DIR_SRCS)
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/monitor DIR_SRCS)
# Set environment variable
SET(NEMU
	${DIR_SRCS}
)
# Set Compiler，指定编译为32位程序
SET(CMAKE_C_FLAGS "-m32")
SET(CMAKE_CXX_FLAGS "-m32")

# Add executable file: /bin/test_out，程序输出名称
ADD_EXECUTABLE(nemu ${NEMU})
set_target_properties(nemu PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
link_libraries(${PROJECT_SOURCE_DIR}/../libs/nemu-ref/lib-nemu-ref.a)
target_link_libraries(nemu PRIVATE readline SDL ${PROJECT_SOURCE_DIR}/../libs/nemu-ref/lib-nemu-ref.a)
```
7. 界面左下角改为debug模式。
![](https://gitee.com/do1e/file-bed/raw/master/1646617632458.png)
8. 由于运行需要参数，vs code默认不给参数，因此可根据debug的内容，在nemu/src/main.c的main函数开头加上
```c
argc = 2;
argv[1] = "--test-reg";
```
或
```c
argc = 3;
argv[1] = "--test-alu";
argv[2] = "add";
```
具体参数见pa_nju/Makefile

9. 快乐地debug或者run吧。
![](https://gitee.com/do1e/file-bed/raw/master/1646617661458.png)

**注：由于不同编译器对未定义行为的操作可能不同，强烈建议结果不对且没发现明显错误时，在PA平台上测试一次。正确的话可以更改`\*_test.c`跳过该测试用例。比如PA1-3中便有不同编译器对nan的解释不同。**
