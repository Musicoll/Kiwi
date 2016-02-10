cmake_minimum_required (VERSION 2.6)
project (KiwiDsp CXX)

set (Kiwi_VERSION_MAJOR 0)
set (Kiwi_VERSION_MINOR 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flax-vector-conversions")

if(NOT ${CMAKE_VERSION} LESS 3.2)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    message(STATUS "Checking compiler flags for C++11 support.")
    # Set C++11 support flags for various compilers
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
    check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        message(STATUS "C++11 is supported.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(COMPILER_SUPPORTS_CXX0X)
        message(STATUS "C++0x is supported.")
        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        endif()
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

# Global
set(KIWI_PROJECT_DIR ${PROJECT_SOURCE_DIR}/..)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Build)
include_directories(${KIWI_PROJECT_DIR}/Modules)

# KiwiDsp
file(GLOB TEST_DSP_SOURCE Modules/KiwiDsp/*.cpp)

find_library(KIWI_DSP_LIB KiwiDsp ${KIWI_PROJECT_DIR}/Modules/KiwiDsp/Build)
add_executable(test_dsp ${TEST_DSP_SOURCE})
target_link_libraries(test_dsp ${KIWI_DSP_LIB})

# KiwiCore
#file(GLOB TEST_CORE_SOURCE Modules/KiwiCore/*.cpp)
#add_executable(test_core ${TEST_CORE_SOURCE})
#target_link_libraries(test_dsp PUBLIC ${KIWI_PROJECT_DIR}/Modules/KiwiDsp/Build)