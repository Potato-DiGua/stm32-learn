cmake_minimum_required(VERSION 3.20)

project(buzzer C ASM)

add_definitions(-DUSE_STDPERIPH_DRIVER)

# CPU 架构
set(CPU_FLAGS
    -mcpu=cortex-m3
)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f10x_it.c
)

# 添加可执行文件
add_executable(${ELF_TARGET} ${SOURCES} ${STARTUP})

target_include_directories(${ELF_TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${ELF_TARGET} PRIVATE stm32_sys utils)

target_compile_options(${ELF_TARGET} PRIVATE
    ${CPU_FLAGS}
    -Wall
    -Wextra
    -g 
    -gdwarf-2 
    -mthumb 
    -Os 
    -fdata-sections 
    -ffunction-sections
    -fmessage-length=0
)


target_link_options(${ELF_TARGET} PRIVATE -T ${LINKER_SCRIPT}
    ${CPU_FLAGS}
    --specs=nano.specs
    -Wl,--gc-sections
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -Wl,--cref
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