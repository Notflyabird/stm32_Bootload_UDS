cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME a)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Src/aes_cmac.c
    Core/Src/boot_jump.c
        # 其他源文件
        # RingBuf sources
        RingBuf/msg_buf.c

        # UDSBase sources
        UDSBase/uds_port.c
        UDSBase/uds_service.c
        UDSBase/uds_tp.c

        # UDSLogic sources
        UDSLogic/service_cfg.c
        UDSLogic/SID10_SessionControl.c
        UDSLogic/SID11_EcuReset.c
        UDSLogic/SID14_ClearDiagnosticInformation.c
        UDSLogic/SID19_ReadDTCInformation.c
        UDSLogic/SID22_ReadDataByIdentifier.c
        UDSLogic/SID27_SecurityAccess.c
        UDSLogic/SID28_CommunicationControl.c
        UDSLogic/SID2E_WriteDataByIdentifier.c
        UDSLogic/SID31_RoutineControl.c
        UDSLogic/SID34_36_37_UpDownloadData.c
        UDSLogic/SID3E_TesterPresent.c
        UDSLogic/SID85_ControlDTCSetting.c

        # Headers (for IDE visibility)
        RingBuf/msg_buf.h
        RingBuf/ring.h

        UDSBase/uds_port.h
        UDSBase/uds_service.h
        UDSBase/uds_tp.h
        UDSBase/uds_tp_private.h
        UDSBase/uds_type.h

        UDSLogic/service_cfg.h
        UDSLogic/SID10_SessionControl.h
        UDSLogic/SID11_EcuReset.h
        UDSLogic/SID14_ClearDiagnosticInformation.h
        UDSLogic/SID19_ReadDTCInformation.h
        UDSLogic/SID22_ReadDataByIdentifier.h
        UDSLogic/SID27_SecurityAccess.h
        UDSLogic/SID28_CommunicationControl.h
        UDSLogic/SID2E_WriteDataByIdentifier.h
        UDSLogic/SID31_RoutineControl.h
        UDSLogic/SID34_36_37_UpDownloadData.h
        UDSLogic/SID3E_TesterPresent.h
        UDSLogic/SID85_ControlDTCSetting.h
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Inc
    RingBuf
    UDSBase
    UDSLogic
    # 其他头文件路径
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

# Add custom command to generate hex file
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/${CMAKE_PROJECT_NAME}.hex
    COMMENT "Generating hex file..."
)

