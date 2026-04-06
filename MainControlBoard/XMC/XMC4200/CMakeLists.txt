# Cmake project for TrinityTrack6000
# MCU - XMC4200F64
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
get_filename_component(ARM_ROOT "${ARM_ROOT}" DIRECTORY)
set(ARM_ROOT "${ARM_ROOT}/tools/arm-gnu-toolchain-14.3.rel1-mingw-w64-x86_64-arm-none-eabi")
message(STATUS "[3] ARM toolchain set (local toolchain): ${ARM_ROOT}")

# Setting up compilers for ARM
set(CMAKE_C_COMPILER "${ARM_ROOT}/bin/arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER "${ARM_ROOT}/bin/arm-none-eabi-g++.exe")
set(CMAKE_ASM_COMPILER "${ARM_ROOT}/bin/arm-none-eabi-gcc.exe")
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

project(XMC4200 C CXX ASM)
# Linker script
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/linker_script.ld")
message(STATUS "    Linker script path: ${LINKER_SCRIPT}")

# Common compiler flags
set(CPU_FLAGS_C "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DXMC4200_F64x256")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEBUG_FLAGS "-g3 -gdwarf-4 -O0")
endif()

# Setting up compiler flags
set(CMAKE_C_FLAGS "${CPU_FLAGS_C} -std=gnu${CMAKE_C_STANDARD} -Wall -Werror -Wno-unused-function -fdata-sections -ffunction-sections ${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS "${CPU_FLAGS_C} -std=gnu++${CMAKE_CXX_STANDARD} -Wall -Werror -fno-exceptions -fno-rtti ${DEBUG_FLAGS}")
set(CMAKE_ASM_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

# Need to added explicitly, because ninja generator does add -O3 flag automatically
set(CMAKE_ASM_FLAGS_RELEASE "${CPU_FLAGS_ASM}")
set(CMAKE_ASM_FLAGS_DEBUG   "${CPU_FLAGS_ASM}")

# Setting up linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS_ASM} -T${LINKER_SCRIPT} -Wl,--gc-sections,--cref")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${CMAKE_BINARY_DIR}/Debug/${PROJECT_NAME}.map")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${CMAKE_BINARY_DIR}/Release/${PROJECT_NAME}.map")
endif()


message(STATUS "[6] Compiler flags C:   ${CMAKE_C_FLAGS}")
message(STATUS "    Compiler flags CXX: ${CMAKE_CXX_FLAGS}")
message(STATUS "    Compiler flags ASM: ${CMAKE_ASM_FLAGS}")
message(STATUS "    Linker flags:       ${CMAKE_EXE_LINKER_FLAGS}")

# Include project files
message(STATUS "[7] Adding DAVE generated sources")
file(GLOB DAVE_CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/Dave/Generated/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/CLOCK_XMC4/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/CPU_CTRL_XMC4/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/DIGITAL_IO/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/GLOBAL_CCU4/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/GLOBAL_CCU8/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/I2C_MASTER/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/PWM_CCU4/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/PWM_CCU8/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/SPI_SLAVE/*.c*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/UART/*.c*"

    "${CMAKE_SOURCE_DIR}/Libraries/Newlib/*.c*"

    "${CMAKE_SOURCE_DIR}/Libraries/XMCLib/src/*.c*"
)
message(STATUS "[8] Adding DAVE generated headers")
file(GLOB DAVE_CORE_HEADERS
    "${CMAKE_SOURCE_DIR}/Dave/Generated/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/CLOCK_XMC4/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/CPU_CTRL_XMC4/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/DIGITAL_IO/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/GLOBAL_CCU4/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/GLOBAL_CCU8/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/I2C_MASTER/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/PWM_CCU4/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/PWM_CCU8/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/SPI_SLAVE/*.h*"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/UART/*.h*"

    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Include/*.h*"
    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Infineon/XMC4200_series/Include/*.h*"

    "${CMAKE_SOURCE_DIR}/Libraries/XMCLib/inc/*.h*"
)
message(STATUS "[9] Adding XMC4200 Core sources")
file(GLOB XMC4200_CORE_SOURCES
    "${CMAKE_SOURCE_DIR}/Startup/*.c"
    "${CMAKE_SOURCE_DIR}/Startup/*.s"
    "${CMAKE_SOURCE_DIR}/Startup/*.S"
)

list(REMOVE_ITEM XMC4200_CORE_SOURCES "${CMAKE_SOURCE_DIR}/main.c")
# Note file /Core/main.c is intentionally excluded here
message(STATUS "[10] Adding XMC4200 Core headers")
file(GLOB XMC4200_CORE_HEADERS
    "${CMAKE_SOURCE_DIR}/Startup/*.h"
)

message(STATUS "[11] Adding project sources")
file(GLOB PROJECT_SOURCES
    "${CMAKE_SOURCE_DIR}/Src/*.c*"
)

message(STATUS "[12] Adding project headers")
file(GLOB PROJECT_HEADERS

)
#
include_directories(
    "${CMAKE_SOURCE_DIR}/Dave/Generated"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/CLOCK_XMC4"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/CPU_CTRL_XMC4"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/DIGITAL_IO"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/GLOBAL_CCU4"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/GLOBAL_CCU8"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/I2C_MASTER"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/PWM_CCU4"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/PWM_CCU8"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/SPI_SLAVE"
    "${CMAKE_SOURCE_DIR}/Dave/Generated/UART"

    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Include"
    "${CMAKE_SOURCE_DIR}/Libraries/CMSIS/Infineon/XMC4200_series/Include"

    "${CMAKE_SOURCE_DIR}/Libraries/XMCLib/inc"
)

add_executable(${PROJECT_NAME}.elf
    ${DAVE_CORE_SOURCES}
    ${XMC4200_CORE_SOURCES}
    ${PROJECT_SOURCES}
)

message(STATUS "Final sources for ${PROJECT_NAME}:")
set(TOTAL_FILE_COUNT 0)
foreach(src ${PROJECT_SOURCES} ${PROJECT_HEADERS} ${XMC4200_CORE_SOURCES} ${XMC4200_CORE_HEADERS} ${DAVE_CORE_SOURCES} ${DAVE_CORE_HEADERS})
    message(STATUS "${src}")
    math(EXPR TOTAL_FILE_COUNT "${TOTAL_FILE_COUNT} + 1")
endforeach()

message(STATUS "Total file count: ${TOTAL_FILE_COUNT}")

add_custom_target(Generate_Doc
    COMMAND doxygen ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating Doxygen documentation"
    VERBATIM
)