# Cmake project for TrinityTrack6000
# MCU - STM32L476RGT6
#
cmake_minimum_required(VERSION 3.24)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Setting up C, C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
message(STATUS "C/C++ standards: ${CMAKE_C_STANDARD}/${CMAKE_CXX_STANDARD}")
# Setting up output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

get_filename_component(ARM_ROOT "${CMAKE_SOURCE_DIR}" DIRECTORY)
set(ARM_ROOT "${ARM_ROOT}/arm-gnu-toolchain-14.3.rel1-mingw-w64-x86_64-arm-none-eabi")
message(STATUS "ARM toolchain set (local toolchain): ${ARM_ROOT}")
# Setting up compilers
set(CMAKE_C_COMPILER "${ARM_ROOT}/bin/arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER "${ARM_ROOT}/bin/arm-none-eabi-g++.exe")
set(CMAKE_ASM_COMPILER "${ARM_ROOT}/bin/arm-none-eabi-as.exe")
set(CMAKE_RC_COMPILER " ")

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_ASM_COMPILER_WORKS 1)
set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(STM32L476RGT6 C CXX ASM)
# Setting up compiler flags
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32L476RGTX_FLASH.ld")
set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_MODULE_LINKER_FLAGS "")

set(CPU_FLAGS_C "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DSTM32L476xx")
set(CPU_FLAGS_ASM "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(CMAKE_C_FLAGS "${CPU_FLAGS_C} -std=gnu11 -Wall -Werror -Wno-unused-function -Og -g3 -fdata-sections -ffunction-sections")
set(CMAKE_CXX_FLAGS "${CPU_FLAGS_C} -std=gnu++17 -Wall -Werror -fno-exceptions -fno-rtti")
set(CMAKE_ASM_FLAGS "${CPU_FLAGS_ASM} ")

message(STATUS "Compiler flags ASM: ${CMAKE_ASM_FLAGS}")
message(STATUS "Compiler flags C:   ${CMAKE_C_FLAGS}")
message(STATUS "Compiler flags CXX: ${CMAKE_CXX_FLAGS}")
# Include directories
message(STATUS "Adding project sources")
file(GLOB PROJECT_SOURCES
    "${CMAKE_SOURCE_DIR}/Src/*.cpp"
    "${CMAKE_SOURCE_DIR}/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Utils/*.cpp"
    "${CMAKE_SOURCE_DIR}/Utils/*.c"
    "${CMAKE_SOURCE_DIR}/Init/*.cpp"
    "${CMAKE_SOURCE_DIR}/Init/*.c"
)
message(STATUS "Adding STM32 Core sources")
file(GLOB ARM_CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/Core/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Core/Startup/*.s"
    "${CMAKE_SOURCE_DIR}/Core/Startup/*.S"
)
message(STATUS "Adding ARM HAL sources")
file(GLOB HAL_CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Src/*.c"
    "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32L4xx/Source/Templates/*.c"
)
message(STATUS "Adding project headers")
file(GLOB PROJECT_HEADERS
    "${CMAKE_SOURCE_DIR}/Include/*.h"
    "${CMAKE_SOURCE_DIR}/Include/*.hpp"
    "${CMAKE_SOURCE_DIR}/Utils/*.h"
    "${CMAKE_SOURCE_DIR}/Init/*.h"
)
message(STATUS "Adding STM32 Core headers")
file(GLOB ARM_CORE_HEADERS
    "${CMAKE_SOURCE_DIR}/Core/Inc/*.h"
    "${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include/*.h"
)
message(STATUS "Adding ARM HAL headers")
file(GLOB HAL_CORE_HEADERS
    "${CMAKE_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc/*.h"
    "${CMAKE_SOURCE_DIR}/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy/*.h"
)
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
# Disable MSVC
set(CMAKE_FIND_ROOT_PATH "${ARM_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

message(STATUS "C   Compiler path: ${CMAKE_C_COMPILER}")
message(STATUS "CXX Compiler path: ${CMAKE_CXX_COMPILER}")
message(STATUS "ASM Compiler path: ${CMAKE_ASM_COMPILER}")

# Disable Windows default linker flags
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "")

add_executable(${PROJECT_NAME}.elf
    ${HAL_CORE_SOURCES}
    ${ARM_CORE_SOURCES}
    ${PROJECT_SOURCES}
)
# Generate map file
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS_ASM} -T${LINKER_SCRIPT} -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map ")

message(STATUS "Final sources for ${PROJECT_NAME}:")
foreach(src ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${ARM_CORE_SOURCES} ${ARM_CORE_HEADERS} ${HAL_CORE_SOURCES} ${HAL_CORE_HEADERS})
    message(STATUS "${src}")
endforeach()

message(STATUS "Linker script: ${LINKER_SCRIPT}")
message(STATUS "Linker flags: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "Link flags: properties: ${LINK_FLAGS}")