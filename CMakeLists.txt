cmake_minimum_required(VERSION 3.10)

# 设置系统
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 设置C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)

# 设置C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

# 设置汇编
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# 设置链接器
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)

# 设置大小
set(SIZE arm-none-eabi-size)

# 跳过编译器 -rdynamic 检查
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(monorepo)

# 启动文件
set(STARTUP ${CMAKE_SOURCE_DIR}/lib/sys/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.s)

# 链接器脚本
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32f10x_flash.ld)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# CPU 架构
set(CPU_FLAGS
    -mcpu=cortex-m3
)

# 定义宏
add_definitions(-DSTM32F10X_MD)

# 系统库
add_subdirectory(lib/sys)

# 工具库
add_subdirectory(lib/utils)

# 应用
add_subdirectory(apps/led)
add_subdirectory(apps/buzzer)