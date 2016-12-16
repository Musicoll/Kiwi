cmake_minimum_required (VERSION 3.0.2)

project (Kiwi CXX)

#--------------------------------------
# Global Settings
#--------------------------------------

option(GCOV_SUPPORT "Build for gcov" Off)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ROOT_DIR ${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/KiwiBuild/Bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/KiwiBuild/Lib")
set(TEST_OUTPUT_PATH "${CMAKE_BINARY_DIR}/KiwiBuild/Test")

if(CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})
   set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
     "Reset the configurations to what we need"
     FORCE)
 endif()

 if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
    endif()

#--------------------------------------
# Compiler Check
#--------------------------------------

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#--------------------------------------
# IDE Settings
#--------------------------------------

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
set(CMAKE_SUPPRESS_REGENERATION true)

#--------------------------------------
# Ressources
#--------------------------------------

set(KIWI_RESSOURCES_DIR ${ROOT_DIR}/Ressources)

set(KIWI_APP_ICON ${KIWI_RESSOURCES_DIR}/Icon.icns)
set(KIWI_PATCHER_FILETYPE_ICON ${KIWI_RESSOURCES_DIR}/kiwi_filetype.icns)
set(KIWI_PLIST ${KIWI_RESSOURCES_DIR}/Info.plist)

set_source_files_properties(${KIWI_APP_ICON} ${KIWI_PATCHER_FILETYPE_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set(KIWI_CLIENT_RESSOURCES ${KIWI_APP_ICON} ${KIWI_PATCHER_FILETYPE_ICON} ${KIWI_PLIST})

#--------------------------------------
# Flags
#--------------------------------------

if (WIN32)
	set(CMAKE_CXX_FLAGS "/EHsc")
	set(CMAKE_EXE_LINKER_FLAGS "")

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /Zi")
    endif()

    if (CMAKE_CL_64)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MACHINE:X64")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MACHINE:X86")
    endif()

elseif(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-warning-option -Wno-pessimizing-move -Wno-overloaded-virtual -Wnounused-function")

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    endif()

elseif(LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
  endif()

endif()

#--------------------------------------
# Preprocessor macro
#--------------------------------------

if (WIN32)
    add_definitions("-D_CPPUNWIND")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG=1)
else()
    add_definitions(-DNDEBUG=1)
endif()

#--------------------------------------
# Search libraries option
#--------------------------------------

if (WIN32)
	set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES} ".lib" ".dll")
	if (CMAKE_CL_64)
        set(WINDOWS_SDK_PATH "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64")
	else()
		set(WINDOWS_SDK_PATH "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib")
	endif()
else()
    set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "" "lib")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES} ".a" ".so")
    if (LINUX)
        include_directories(/usr/include/freetype2)
    endif()
endif()

#--------------------------------------
# Macros
#--------------------------------------

include(./Config/group.cmake)
include(./Config/dependency.cmake)

#----------------------------------
# Juce
#----------------------------------

if (APPLE)
    find_library(COCOA Cocoa)
    find_library(IOKIT IOKit)
    find_library(QUARTZCORE QuartzCore)
    find_library(CARBON Carbon)
    find_library(WEBKIT WebKit)
    find_library(COREAUDIO CoreAudio)
    find_library(COREMIDI CoreMIDI)
    find_library(ACCELERATE Accelerate)
    find_library(DISCRECORDING DiscRecording)
    find_library(AUDIOTOOLBOX AudioToolbox)
    set(JUCE_DEP ${COCOA} ${IOKIT} ${QUARTZCORE} ${CARBON} ${WEBKIT} ${COREAUDIO} ${COREMIDI} ${ACCELERATE} ${DISCRECORDING} ${AUDIOTOOLBOX})
elseif(WIN32)
	find_library(WS2_32 ws2_32 PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(UUID uuid PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(WSOCK32 wsock32 PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(WININET wininet PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(LIBVERSION version PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(OLE32 ole32 PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(OLEAUT32 oleaut32 PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(IMM32 imm32 PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(COMDLG32 comdlg32 PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(SHLWAPI shlwapi PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(RPCRT4 rpcrt4 PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	find_library(WINMM winmm PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	set(JUCE_DEP ${WS2_32} ${UUID} ${WSOCK32} ${WININET} ${LIBVERSION} ${OLE32} ${OLEAUT32} ${COMDLG32} ${SHLWAPI} ${RPCRT4} ${WINMM})
elseif(LINUX)
    find_library(RT rt)
    find_library(DL dl)
    find_library(PTHREAD pthread)
    find_library(X11 X11)
    find_library(XEXT Xext)
    find_library(FREETYPE freetype)
    find_library(ASOUND asound)
    set(JUCE_DEP ${RT} ${DL} ${PTHREAD} ${FREETYPE} ${X11} ${XEXT} ${ASOUND})
endif()

set(JUCE_MACRO_DEFINITIONS JUCE_STANDALONE_APPLICATION=1
                           JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)

file (GLOB_RECURSE JUCE_SRC ${ROOT_DIR}/ThirdParty/Juce/modules/*.[h|c]pp
                            ${ROOT_DIR}/ThirdParty/Juce/modules/*.h
                            ${ROOT_DIR}/ThirdParty/Juce/modules/*.mm)

set_source_files_properties(${JUCE_SRC} PROPERTIES HEADER_FILE_ONLY TRUE)

set(JUCE_COMPILED_SRC ${ROOT_DIR}/ThirdParty/Juce/modules/juce_core/juce_core.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_data_structures/juce_data_structures.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_events/juce_events.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_graphics/juce_graphics.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_gui_basics/juce_gui_basics.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_gui_extra/juce_gui_extra.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_audio_basics/juce_audio_basics.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_audio_devices/juce_audio_devices.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_audio_formats/juce_audio_formats.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_audio_utils/juce_audio_utils.cpp
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_audio_processors/juce_audio_processors.cpp)

if (APPLE)
    set_source_files_properties(${JUCE_COMPILED_SRC} PROPERTIES COMPILE_FLAGS "-x objective-c++"
                                                                HEADER_FILE_ONLY FALSE)
else()
    set_source_files_properties(${JUCE_COMPILED_SRC} PROPERTIES HEADER_FILE_ONLY FALSE)
endif()


add_library(Juce STATIC ${JUCE_SRC})
target_link_libraries(Juce ${JUCE_DEP})
target_compile_definitions(Juce PUBLIC ${JUCE_MACRO_DEFINITIONS})
target_include_directories(Juce PUBLIC ${ROOT_DIR}/ThirdParty/Juce/modules)
set_target_properties(Juce PROPERTIES FOLDER ThirdParty)
source_group_rec("${JUCE_SRC}" ${ROOT_DIR}/ThirdParty/Juce/modules)

#----------------------------------
# Flip
#----------------------------------

if (${FLIP_PRIVATE})
    if(APPLE)

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            find_library(FlipLib NAMES flip PATHS ${ROOT_DIR}/ThirdParty/flip/projects/build/Debug)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            find_library(FlipLib NAMES flip PATHS ${ROOT_DIR}/ThirdParty/flip/projects/build/Release)
        endif()

        set(FLIP_DEP ${FlipLib})
        set(FLIP_DIR "${ROOT_DIR}/ThirdParty/flip")
        file (GLOB_RECURSE FLIP_SRC ${ROOT_DIR}/ThirdParty/flip/inlude/*.hpp
                                ${ROOT_DIR}/ThirdParty/flip/include/*.h
                                ${ROOT_DIR}/ThirdParty/flip/src/*.cpp)
    endif()
else()

    if(APPLE)
        find_library(FlipLib NAMES flip PATHS ${ROOT_DIR}/ThirdParty/flip-public/lib/mac)
        set(FLIP_DEP ${FlipLib})

    elseif(WIN32)
        if (CMAKE_CL_64)
            find_library(FlipLib NAMES flip PATHS "${ROOT_DIR}/ThirdParty/flip-public/lib/windows/VS2015/x64/Release")
	   else()
            find_library(FlipLib NAMES flip PATHS "${ROOT_DIR}/ThirdParty/flip-public/lib/windows/VS2015/Win32/Release")
	   endif()
       find_library(WS2_32 ws2_32 PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	   set(FLIP_DEP ${WS2_32} ${FlipLib})

    elseif(LINUX)
        find_library(FlipLib NAMES flip PATHS ${ROOT_DIR}/ThirdParty/flip-public/lib/linux/gcc)
        set(FLIP_DEP ${FlipLib})
    endif()

    set(FLIP_DIR "${ROOT_DIR}/ThirdParty/flip-public")
    file (GLOB_RECURSE FLIP_SRC ${FLIP_DIR}/include/*.hpp
                                ${FLIP_DIR}/include/*.h)
endif()

add_custom_target(Flip SOURCES ${FLIP_SRC})
set_target_properties (Flip PROPERTIES FOLDER ThirdParty
                                       INTERFACE_INCLUDE_DIRECTORIES ${FLIP_DIR}/include
                                       COMPILE_DEFINITIONS "flip_NOTHING=flip_NOTHING_FATAL"
                                       INTERFACE_LINK_LIBRARIES "${FLIP_DEP}")
source_group_rec("${FLIP_SRC}" ${FLIP_DIR})

#----------------------------------
# Model
#----------------------------------

file(GLOB_RECURSE KIWI_MODEL_SRC ${ROOT_DIR}/Modules/KiwiModel/*.[c|h]pp)
add_library(KiwiModel STATIC ${KIWI_MODEL_SRC})
target_include_directories(KiwiModel PUBLIC ${ROOT_DIR}/Modules)
target_add_dependency(KiwiModel Flip)
set_target_properties(KiwiModel PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_MODEL_SRC}" ${ROOT_DIR}/Modules/KiwiModel)

#----------------------------------
# Dsp
#----------------------------------

file(GLOB_RECURSE KIWI_DSP_SRC ${ROOT_DIR}/Modules/KiwiDsp/*.[c|h]pp)
add_library(KiwiDsp STATIC ${KIWI_DSP_SRC})
target_compile_definitions(KiwiDsp PUBLIC -DKIWI_DSP_FLOAT=1)
target_include_directories(KiwiDsp PUBLIC ${ROOT_DIR}/Modules)
set_target_properties(KiwiDsp PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_DSP_SRC}" ${ROOT_DIR}/Modules/KiwiDsp)

#----------------------------------
# Engine
#----------------------------------

file(GLOB_RECURSE KIWI_ENGINE_SRC ${ROOT_DIR}/Modules/KiwiEngine/*.[c|h]pp)
add_library(KiwiEngine STATIC ${KIWI_ENGINE_SRC})
target_include_directories(KiwiEngine PUBLIC ${ROOT_DIR}/Modules)
target_add_dependency(KiwiEngine Flip)
target_add_dependency(KiwiEngine KiwiModel)
target_add_dependency(KiwiEngine KiwiDsp)
set_target_properties(KiwiEngine PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_ENGINE_SRC}" ${ROOT_DIR}/Modules/KiwiEngine)

#----------------------------------
# Server
#----------------------------------

file(GLOB_RECURSE KIWI_SERVER_SRC ${ROOT_DIR}/Modules/KiwiServer/*.[c|h]pp)
add_library(KiwiServer STATIC ${KIWI_SERVER_SRC})
target_include_directories(KiwiEngine PUBLIC ${ROOT_DIR}/Modules)
target_add_dependency(KiwiServer KiwiModel)
target_add_dependency(KiwiServer Juce)
set_target_properties(KiwiServer PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_SERVER_SRC}" ${ROOT_DIR}/Modules/KiwiServer)

file(GLOB_RECURSE SERVER_SRC ${ROOT_DIR}/Server/Source/*.[c|h]pp)
add_executable(Server ${SERVER_SRC})
target_add_dependency(Server KiwiServer)
source_group_rec("${SERVER_SRC}" ${ROOT_DIR}/Server/Source)

#----------------------------------
# Client
#----------------------------------

file(GLOB_RECURSE KIWI_CLIENT_SRC ${ROOT_DIR}/Client/Source/*.[c|h]pp)
add_executable(Kiwi ${KIWI_CLIENT_SRC} "${KIWI_CLIENT_RESSOURCES}")
target_add_dependency(Kiwi KiwiEngine)
target_add_dependency(Kiwi KiwiModel)
target_add_dependency(Kiwi KiwiDsp)
target_add_dependency(Kiwi Juce)
target_add_dependency(Kiwi KiwiServer)

if (APPLE)
	set_target_properties(Kiwi PROPERTIES MACOSX_BUNDLE TRUE
									      MACOSX_BUNDLE_INFO_PLIST ${KIWI_PLIST})
elseif(WIN32)
	set_target_properties(Kiwi PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

source_group_rec("${KIWI_CLIENT_SRC}" ${ROOT_DIR}/Client/Source)

#----------------------------------
# Tests
#----------------------------------

# Test Dsp

file(GLOB TEST_DSP_SRC ${ROOT_DIR}/Test/Dsp/*.[c|h]pp)
add_executable(test_dsp ${TEST_DSP_SRC})
target_add_dependency(test_dsp KiwiDsp)
set_target_properties(test_dsp PROPERTIES FOLDER Test)
source_group_rec("${TEST_DSP_SRC}" ${ROOT_DIR}/Test/Dsp)
set_target_properties(test_dsp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_PATH})

# Test Model

file(GLOB TEST_MODEL_SRC ${ROOT_DIR}/Test/Model/*.[c|h]pp)
add_executable(test_model ${TEST_MODEL_SRC})
target_add_dependency(test_model KiwiModel)
set_target_properties(test_model PROPERTIES FOLDER Test)
source_group_rec("${TEST_MODEL_SRC}" ${ROOT_DIR}/Test/Model)
set_target_properties(test_model PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_PATH})

# Test Engine

file(GLOB TEST_ENGINE_SRC ${ROOT_DIR}/Test/Engine/*.[c|h]pp)
add_executable(test_engine ${TEST_ENGINE_SRC})
target_add_dependency(test_engine KiwiEngine)
set_target_properties(test_engine PROPERTIES FOLDER Test)
source_group_rec("${TEST_ENGINE_SRC}" ${ROOT_DIR}/Test/Engine)
set_target_properties(test_engine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_PATH})

# Test Server

file(GLOB TEST_SERVER_SRC ${ROOT_DIR}/Test/Server/*.[c|h]pp)
add_executable(test_server ${TEST_SERVER_SRC})
target_add_dependency(test_server KiwiServer)
set_target_properties(test_server PROPERTIES FOLDER Test)
source_group_rec("${TEST_SERVER_SRC}" ${ROOT_DIR}/Test/Server)
set_target_properties(test_server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_PATH})

# Coverage setting
if(${GCOV_SUPPORT} STREQUAL "On" AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
  target_link_libraries(test_dsp PUBLIC gcov)
  target_link_libraries(test_model PUBLIC gcov)
  target_link_libraries(test_engine PUBLIC gcov)
  target_link_libraries(test_server PUBLIC gcov)
endif()

# Tests Target

add_custom_target(Tests ALL DEPENDS test_dsp test_model test_engine test_server)
set_target_properties(Tests PROPERTIES FOLDER Test)

add_custom_command(TARGET Tests
                   POST_BUILD COMMAND test_dsp 
                              COMMAND test_model
                              COMMAND test_engine
                              COMMAND test_server
                   USES_TERMINAL)

#----------------------------------
# Examples
#----------------------------------

# Dsp - PortAudio

if(APPLE)
  find_path(PORTAUDIO_INCLUDES "portaudio.h")
  include_directories("${PORTAUDIO_INCLUDES}")
  file(GLOB TEST_DSP_PORTAUDIO_SRC ${ROOT_DIR}/Test/DspPortAudio/*.[c|h]pp)
  find_library(PORTAUDIO_LIB "portaudio")
  add_executable(dsp_portaudio ${TEST_DSP_PORTAUDIO_SRC})
  target_link_libraries(dsp_portaudio KiwiDsp ${PORTAUDIO_LIB} ${SUPPORT_LIBS})
  set_target_properties(dsp_portaudio PROPERTIES FOLDER Test)
  # PortAudio configured for x86_64 archs
  set_target_properties(dsp_portaudio PROPERTIES XCODE_ATTRIBUTE_ARCHS "x86_64")
  source_group_rec("${TEST_DSP_PORTAUDIO_SRC}" ${ROOT_DIR}/Test/DspPortAudio)
endif()
