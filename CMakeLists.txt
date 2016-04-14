cmake_minimum_required (VERSION 2.8.7)
project (Kiwi CXX)

set (Kiwi_VERSION_MAJOR 0)
set (Kiwi_VERSION_MINOR 1)

#--------------------------------------
# OPTIONS
#--------------------------------------
option(COVERALL "Build with coverall" OFF)

#--------------------------------------
# Compiler Check
#--------------------------------------

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

if(COVERALL)
    message(STATUS "Build with coverall")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage" )
else()
    message(STATUS "Build without coverall")
endif()

# Call make to build the whole project
# Each target (Libraries/Binaries) can be built separatly calling [make [targetname]]
# Target dependencies are built as well if necessary

#----------------------------------
# Modules
#----------------------------------

# -> ThirdParty

#find_library(Flip NAMES flip_macos HINTS ${PROJECT_SOURCE_DIR}/ThirdParty/flip-public/bin)

# -> Module Core

file(GLOB KIWI_CORE_SRC
     ${PROJECT_SOURCE_DIR}/Modules/KiwiCore/*.h
     ${PROJECT_SOURCE_DIR}/Modules/KiwiCore/*.hpp
     ${PROJECT_SOURCE_DIR}/Modules/KiwiCore/*.cpp
)
#add_library(KiwiCore STATIC ${KIWI_CORE_SRC})
#set_property(TARGET KiwiCore
#             PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Modules)

# -> Module Model (only Mac for now)
if(${APPLE})
    file(GLOB KIWI_MODEL_SRC
     ${PROJECT_SOURCE_DIR}/Modules/KiwiModel/*.h
     ${PROJECT_SOURCE_DIR}/Modules/KiwiModel/*.hpp
     ${PROJECT_SOURCE_DIR}/Modules/KiwiModel/*.cpp
    )

    add_library(KiwiModel STATIC ${KIWI_MODEL_SRC})
    set_property(TARGET KiwiModel
                 PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Modules
                                              ${PROJECT_SOURCE_DIR}/ThirdParty/flip-public/include)

    target_link_libraries(KiwiModel ${PROJECT_SOURCE_DIR}/ThirdParty/flip-public/bin/flip_macos.a)
endif()

# -> Module DSP

file(GLOB KIWI_DSP_SRC
     ${PROJECT_SOURCE_DIR}/Modules/KiwiDsp/*.hpp
     ${PROJECT_SOURCE_DIR}/Modules/KiwiDsp/*.cpp
)
add_library(KiwiDsp STATIC ${KIWI_DSP_SRC})
set_property(TARGET KiwiDsp
             PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Modules)
set_property(TARGET KiwiDsp
             APPEND PROPERTY COMPILE_DEFINITIONS KIWI_DSP_FLOAT)

# -> Module Gui

file(GLOB KIWI_GRAPHICS_SRC                                   
     ${PROJECT_SOURCE_DIR}/Modules/KiwiGui/*.hpp
     ${PROJECT_SOURCE_DIR}/Modules/KiwiGui/*.cpp
)
add_library(KiwiGui STATIC ${KIWI_GRAPHICS_SRC})
target_link_libraries(KiwiGui)
set_property(TARGET KiwiGui
             PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Modules)


#----------------------------------
# Tests
#----------------------------------

# -> Core Unit Tests

file(GLOB TEST_CORE_SRC
    ${PROJECT_SOURCE_DIR}/test/Modules/Core/*.cpp)
add_executable(test_core ${TEST_CORE_SRC})
#target_link_libraries(test_core KiwiCore)
set_property(TARGET test_core
             PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Modules)

# -> Model Unit Tests (only Mac for now)
if(${APPLE})
    file(GLOB TEST_MODEL_SRC
        ${PROJECT_SOURCE_DIR}/test/Modules/Model/*.cpp)
    add_executable(test_model ${TEST_MODEL_SRC})

    target_link_libraries(test_model KiwiModel ${PROJECT_SOURCE_DIR}/ThirdParty/flip-public/bin/flip_macos.a)
    set_property(TARGET test_model
                 PROPERTY INCLUDE_DIRECTORIES   ${PROJECT_SOURCE_DIR}/Modules
                                                ${PROJECT_SOURCE_DIR}/ThirdParty/flip-public/include)
endif()

# -> Dsp Unit Tests

file(GLOB TEST_DSP_SRC ${PROJECT_SOURCE_DIR}/test/Modules/Dsp/*.cpp)
add_executable(test_dsp ${TEST_DSP_SRC})
target_link_libraries(test_dsp KiwiDsp)
set_property(TARGET test_dsp
             PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Modules)
set_property(TARGET test_dsp
             APPEND PROPERTY COMPILE_DEFINITIONS KIWI_DSP_FLOAT)

# -> Gui Unit Tests

file(GLOB TEST_GUI_SRC ${PROJECT_SOURCE_DIR}/test/Modules/Gui/*.cpp)
add_executable(test_gui ${TEST_GUI_SRC})
target_link_libraries(test_gui KiwiGui)
set_property(TARGET test_gui
             PROPERTY INCLUDE_DIRECTORIES ${PROJECT_SOURCE_DIR}/Modules)
