project(stm32_sys C ASM)

# 创建库
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(stm32_sys INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/CM3/CoreSupport
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/CM3/DeviceSupport/ST/STM32F10x
    ${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_StdPeriph_Driver/inc
)

file(GLOB_RECURSE AUTO_COMMON_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/STM32F10x_StdPeriph_Driver/src/*.c"
)

set(SOURCES
    ${AUTO_COMMON_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/CM3/CoreSupport/core_cm3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c
)

target_sources(stm32_sys INTERFACE ${SOURCES})

