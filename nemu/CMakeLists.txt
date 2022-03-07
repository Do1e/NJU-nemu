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