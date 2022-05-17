cmake_minimum_required(VERSION 3.10.2)
project(nemu)
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR}/../libs/nemu-ref/include)

file(GLOB_RECURSE NEMU_DIR_SRCS ${PROJECT_SOURCE_DIR}/src/*.c)

set(NEMU ${NEMU_DIR_SRCS})
set(CMAKE_C_FLAGS "-m32")
set(CMAKE_CXX_FLAGS "-m32")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

add_executable(nemu ${NEMU})
set_target_properties(nemu PROPERTIES COMPILE_FLAGS "-m32 -ggdb3 -MP -Wall -Werror" LINK_FLAGS "-m32")
target_link_libraries(nemu PRIVATE readline SDL ${PROJECT_SOURCE_DIR}/../libs/nemu-ref/lib-nemu-ref.a)
