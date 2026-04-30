cmake_minimum_required(VERSION 3.14)
project(DeliveryRouteProfitOptimizer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/Order.cpp
    src/RoutePlanner.cpp
)

add_executable(DeliveryOptimizer ${SOURCES})

target_include_directories(DeliveryOptimizer PRIVATE include)

# Optional: enable optimisations in Release mode
target_compile_options(DeliveryOptimizer PRIVATE
    $<$<CONFIG:Release>:-O2>
)
