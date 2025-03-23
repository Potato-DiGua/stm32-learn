cmake_minimum_required(VERSION 3.20)

project(led C CXX ASM)

add_definitions(-DUSE_STDPERIPH_DRIVER)

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

# 禁用LTO以解决链接问题
# if(supported)
#     set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
# else()
#     message(STATUS "IPO/LTO not supported: ${error}")
# endif()


# ELF 文件名称
set(ELF_TARGET ${PROJECT_NAME}.elf)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f10x_it.c
)

# 添加可执行文件
add_executable(${ELF_TARGET} ${SOURCES} ${STARTUP})

target_include_directories(${ELF_TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${ELF_TARGET} PRIVATE stm32_sys utils)

target_compile_options(${ELF_TARGET} PRIVATE
    # CPU特定标志，通常包含-mcpu=cortex-m3等
    ${CPU_FLAGS} 
    # 开启所有常见警告
    -Wall
    # 开启额外警告
    -Wextra
    # 生成调试信息
    -g
    # 使用DWARF-2格式的调试信息
    -gdwarf-2 
    # 使用Thumb指令集编译
    -mthumb 
    # 优化大小（在保持速度的同时减小代码体积）
    -Os
    # 每个数据项放入单独的段
    -fdata-sections
    # 每个函数放入单独的段
    -ffunction-sections 
    # 不限制错误信息长度
    -fmessage-length=0 
    # 禁用C++的异常处理机制
    -fno-exceptions
    # 禁用C++的RTTI机制
    -fno-rtti
)


target_link_options(${ELF_TARGET} PRIVATE -T ${LINKER_SCRIPT}
    # 与编译时相同的CPU标志
    ${CPU_FLAGS}
    # 使用Newlib库
    --specs=nano.specs
    # 将未使用的代码移除
    -Wl,--gc-sections
    # 生成映射文件
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    # 生成交叉引用
    -Wl,--cref
    # 打印内存使用情况
    -Wl,--print-memory-usage
)

# 链接Newlib库
# target_link_libraries(${ELF_TARGET} PRIVATE -lc -lgcc -lnosys)

# 生成二进制文件
add_custom_command(TARGET ${ELF_TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ELF_TARGET} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.bin
)

# 生成HEX文件
add_custom_command(TARGET ${ELF_TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ELF_TARGET} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex
)

# 打印大小信息
add_custom_command(TARGET ${ELF_TARGET} POST_BUILD
    COMMAND ${SIZE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ELF_TARGET}
    COMMAND ${SIZE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.hex
)