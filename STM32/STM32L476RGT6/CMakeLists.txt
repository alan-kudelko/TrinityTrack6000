# Cmake project for TrinityTrack6000
# MCU - STM32L476RGT6
#
cmake_minimum_required(VERSION 3.24)

# Build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Setting up output folders
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Setting up output folders
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
else()
    message(FATAL_ERROR "CMAKE_BUILD_TYPE not set. Please set it to Debug or Release")
endif()

message(STATUS "[1] Build type: ${CMAKE_BUILD_TYPE}")

# Setting up C, C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
message(STATUS "[2] C/C++ standards: ${CMAKE_C_STANDARD}/${CMAKE_CXX_STANDARD}")

# ARM toolchain
get_filename_component(ARM_ROOT "${CMAKE_SOURCE_DIR}" DIRECTORY)
get_filename_component(ARM_ROOT "${ARM_ROOT}" DIRECTORY)
set(ARM_ROOT "${ARM_ROOT}/tools/arm-gnu-toolchain-14.3.rel1-mingw-w64-x86_64-arm-none-eabi")
message(STATUS "[3] ARM toolchain set (local toolchain): ${ARM_ROOT}")

# Setting up compilers for ARM
set(CMAKE_C_COMPILER "${ARM_ROOT}/bin/arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER "${ARM_ROOT}/bin/arm-none-eabi-g++.exe")
set(CMAKE_ASM_COMPILER "${ARM_ROOT}/bin/arm-none-eabi-as.exe")
# No resource compiler
set(CMAKE_RC_COMPILER " ")

# Setting up cross-compilation
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
# Avoiding CMake compiler checks
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Disable MSVC
set(CMAKE_FIND_ROOT_PATH "${ARM_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Compiler paths
message(STATUS "[4] C   Compiler path:  ${CMAKE_C_COMPILER}")
message(STATUS "    CXX Compiler path:  ${CMAKE_CXX_COMPILER}")
message(STATUS "    ASM Compiler path:  ${CMAKE_ASM_COMPILER}")

project(STM32L476RGT6 C CXX ASM)
# Linker script
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32L476RGTX_FLASH.ld")
message(STATUS "    Linker script path: ${LINKER_SCRIPT}")

# Common compiler flags
set(CPU_FLAGS_C "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DSTM32L476xx")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG_FLAGS "-g3 -O0")
endif()

# Setting up compiler flags
set(CMAKE_C_FLAGS "${CPU_FLAGS_C} -std=gnu${CMAKE_C_STANDARD} -Wall -Werror -Wno-unused-function -Wno-unused-variable -fdata-sections -ffunction-sections ${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS "${CPU_FLAGS_C} -std=gnu++${CMAKE_CXX_STANDARD} -Wall -Werror -fno-exceptions -fno-rtti ${DEBUG_FLAGS}")
set(CMAKE_ASM_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# Need to added explicitly, because ninja generator does add -O3 flag automatically
set(CMAKE_ASM_FLAGS_RELEASE "${CPU_FLAGS_ASM}")
set(CMAKE_ASM_FLAGS_DEBUG   "${CPU_FLAGS_ASM}")

# Setting up linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS_ASM} -T${LINKER_SCRIPT} -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map ")

message(STATUS "[6] Compiler flags C:   ${CMAKE_C_FLAGS}")
message(STATUS "    Compiler flags CXX: ${CMAKE_CXX_FLAGS}")
message(STATUS "    Compiler flags ASM: ${CMAKE_ASM_FLAGS}")
message(STATUS "    Linker flags:       ${CMAKE_EXE_LINKER_FLAGS}")

# Include project files
message(STATUS "[7] Adding ARM HAL sources")
file(GLOB HAL_CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/*.c"
)
message(STATUS "[8] Adding ARM HAL headers")
file(GLOB HAL_CORE_HEADERS
    "${CMAKE_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc/*.h"
    "${CMAKE_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy/*.h"
)
message(STATUS "[9] Adding STM32 Core sources")
file(GLOB ARM_CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/Core/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Core/Startup/*.s"
    "${CMAKE_SOURCE_DIR}/Core/Startup/*.S"
)
message(STATUS "[10] Adding STM32 Core headers")
file(GLOB ARM_CORE_HEADERS
    "${CMAKE_SOURCE_DIR}/Core/Inc/*.h"
    "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include/*.h"
)
message(STATUS "[11] Adding project sources")
file(GLOB PROJECT_SOURCES
    "${CMAKE_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_SOURCE_DIR}/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Utils/*.cpp"
    "${CMAKE_SOURCE_DIR}/Utils/*.c"
    "${CMAKE_SOURCE_DIR}/Init/*.cpp"
    "${CMAKE_SOURCE_DIR}/Init/*.c"
)
message(STATUS "[12] Adding project headers")
file(GLOB PROJECT_HEADERS
    "${CMAKE_SOURCE_DIR}/Include/*.h"
    "${CMAKE_SOURCE_DIR}/Include/*.hpp"
    "${CMAKE_SOURCE_DIR}/Utils/*.h"
    "${CMAKE_SOURCE_DIR}/Init/*.h"
)
#
include_directories(
    "${CMAKE_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc"
    "${CMAKE_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy"
    "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32L4xx/Include"
    "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/Core/Inc"
    "${CMAKE_SOURCE_DIR}/Include"
    "${CMAKE_SOURCE_DIR}/Utils"
    "${CMAKE_SOURCE_DIR}/Init"
)

add_executable(${PROJECT_NAME}.elf
    ${HAL_CORE_SOURCES}
    ${ARM_CORE_SOURCES}
    ${PROJECT_SOURCES}
)

message(STATUS "Final sources for ${PROJECT_NAME}:")
foreach(src ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${ARM_CORE_SOURCES} ${ARM_CORE_HEADERS} ${HAL_CORE_SOURCES} ${HAL_CORE_HEADERS})
    message(STATUS "${src}")
endforeach()
