cmake_minimum_required(VERSION 3.10.2)
project(kernel)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR}/../libs/newlib/include)

file(GLOB_RECURSE KERNEL_DIR_SRCS ${PROJECT_SOURCE_DIR}/src/*.c)

set(KERNEL ${KERNEL_DIR_SRCS})
set(CMAKE_C_FLAGS "-m32")
set(CMAKE_CXX_FLAGS "-m32")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

add_executable(kernel ${KERNEL})
set_target_properties(kernel PROPERTIES COMPILE_FLAGS "-m32 -ggdb3 -MP -Wall -Werror -fno-builtin -fno-omit-frame-pointer -fno-stack-protector" LINK_FLAGS "-m32 -Ttext=0x30000 -m elf_i386")
target_link_libraries(kernel PRIVATE readline SDL ${PROJECT_SOURCE_DIR}/src/memory/mm_malloc.o ${PROJECT_SOURCE_DIR}/start/start.o ${PROJECT_SOURCE_DIR}/../libs/newlib/libc.a)
