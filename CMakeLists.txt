cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)

# 跳过编译器 -rdynamic 检查
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(STM32Project C ASM)

set(CMAKE_C_STANDARD 11)



# ELF 文件名称
set(ELF_TARGET ${PROJECT_NAME}.elf)


add_definitions(-DSTM32F10X_MD)
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

file(GLOB_RECURSE AUTO_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/STM32F10x_StdPeriph_Driver/src/*.c"
)
set(SOURCES
    ${AUTO_SOURCES}

    ${CMAKE_SOURCE_DIR}/CMSIS/CM3/CoreSupport/core_cm3.c
    ${CMAKE_SOURCE_DIR}/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c
)

# 启动文件
set(STARTUP ${CMAKE_SOURCE_DIR}/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.s)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 添加可执行文件
add_executable(${ELF_TARGET} ${SOURCES} ${STARTUP})

target_include_directories(${ELF_TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/CMSIS/CM3/CoreSupport
    ${CMAKE_SOURCE_DIR}/CMSIS/CM3/DeviceSupport/ST/STM32F10x
    ${CMAKE_SOURCE_DIR}/STM32F10x_StdPeriph_Driver/inc
    ${CMAKE_SOURCE_DIR}/src
)

# 链接器脚本
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32f10x_flash.ld)


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
    COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ELF_TARGET} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/out.bin
)

# 生成HEX文件
add_custom_command(TARGET ${ELF_TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ELF_TARGET} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/out.hex
)

# 打印大小信息
add_custom_command(TARGET ${ELF_TARGET} POST_BUILD
    COMMAND ${SIZE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${ELF_TARGET}
    COMMAND ${SIZE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/out.hex
)