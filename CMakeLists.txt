cmake_minimum_required(VERSION 3.10)

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

project(monorepo)

# 启动文件
set(STARTUP ${CMAKE_SOURCE_DIR}/lib/sys/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/gcc_ride7/startup_stm32f10x_md.s)

# 链接器脚本
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/stm32f10x_flash.ld)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_definitions(-DSTM32F10X_MD)

# 包含子目录（模块、应用、测试）
add_subdirectory(lib/sys)
add_subdirectory(lib/utils)
add_subdirectory(apps/led)
add_subdirectory(apps/buzzer)