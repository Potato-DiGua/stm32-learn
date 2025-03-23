project(utils CXX)

# 创建库
add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Delay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Random.cpp
)

target_sources(${PROJECT_NAME} INTERFACE ${SOURCES})

target_link_libraries(${PROJECT_NAME} INTERFACE stm32_sys)

