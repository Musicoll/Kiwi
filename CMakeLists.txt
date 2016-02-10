cmake_minimum_required (VERSION 2.8.7)
project (Kiwi CXX)
set (Kiwi_VERSION_MAJOR 0)
set (Kiwi_VERSION_MINOR 1)

# Compiler checks
if(NOT ${CMAKE_VERSION} LESS 3.2)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    message(STATUS "Checking compiler flags for C++11 support.")
    # Set C++11 support flags for various compilers
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX11)
        message(STATUS "C++11 is supported.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        # Add compiler specific flags
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            # The Travis CI build machines don't have libc++ installed
            if ( NOT DEFINED ENV{TRAVIS} )
                set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++" )
            endif()
            #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            # no GNU specific flags yet
        endif() # End compiler specific flags
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

# Add Platform-specific CXX flags
if(${APPLE})
    # the following flag is (only) needed to compile Accelerate on old Mac/gcc version
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flax-vector-conversions")
endif()

set(KIWI_MODULES_DIR ${PROJECT_SOURCE_DIR}/Modules)
set(KIWI_TEST_DIR ${PROJECT_SOURCE_DIR}/test)

include_directories(
    ${KIWI_MODULES_DIR}
)

# Module Core
file(GLOB KIWI_MODULE_CORE_SRC
    ${KIWI_MODULES_DIR}/KiwiCore/*.cpp
)

# Module DSP
file(GLOB KIWI_MODULE_DSP_SRC
    ${KIWI_MODULES_DIR}/KiwiDsp/*.cpp
)

# Core Unit Tests sources
set(TEST_CORE
    ${KIWI_MODULE_CORE_SRC}
    ${KIWI_TEST_DIR}/Modules/Core/test_KiwiCore.cpp
) 
add_executable(test_core ${TEST_CORE})

# DSP Unit Tests sources
set(TEST_DSP
    ${KIWI_MODULE_DSP_SRC}
    ${KIWI_TEST_DIR}/Modules/Dsp/test_KiwiDsp.cpp
)
add_executable(test_dsp ${TEST_DSP})

# target_link_libraries(test_suite ${TEST_LIBS})
