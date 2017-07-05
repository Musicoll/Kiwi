cmake_minimum_required (VERSION 3.0.2)

project (Kiwi CXX)

#--------------------------------------
# Check Compiler Version
#--------------------------------------

if (WIN32)
    if((CMAKE_CXX_COMPILER_ID NOT STREQUAL "MSVC") OR (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19))
        message(WARNING "Using untested compiler, tested compiler is MSVC 19.0 or later")
    endif()
elseif(UNIX AND NOT APPLE)
    if ((CMAKE_CXX_COMPILER_ID NOT STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9))
        message(WARNING "Using untested compiler, tested compiler is gcc 4.9 or later")
    endif()
else()
    if ((CMAKE_CXX_COMPILER_ID NOT STREQUAL "AppleClang") OR (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.3))
        message(WARNING "Using untested compiler, tested compiler is AppleClang 7.3 or later")
    endif()
endif()

#--------------------------------------
# Global Settings
#--------------------------------------

option(GCOV_SUPPORT "Build for gcov" Off)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(ROOT_DIR ${PROJECT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/KiwiBuild")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/KiwiBuild")

if(CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})
   set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
     "Reset the configurations to what we need"
     FORCE)
 endif()

 if(UNIX AND NOT APPLE)
        set(LINUX TRUE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG TRUE)
endif()

#--------------------------------------
# Compiler Check
#--------------------------------------

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#--------------------------------------
# IDE Settings
#--------------------------------------

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.8)
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
set(CMAKE_SUPPRESS_REGENERATION true)

#--------------------------------------
# Ressources
#--------------------------------------

set(KIWI_RESSOURCES_DIR ${ROOT_DIR}/Ressources)

if (APPLE)
	set(KIWI_APP_ICON ${KIWI_RESSOURCES_DIR}/Project/XCode/Icon.icns)
	set(KIWI_PATCHER_FILETYPE_ICON ${KIWI_RESSOURCES_DIR}/Project/XCode/kiwi_filetype.icns)
	set(KIWI_PLIST ${KIWI_RESSOURCES_DIR}/Project/XCode/Info.plist)
	set_source_files_properties(${KIWI_APP_ICON} ${KIWI_PATCHER_FILETYPE_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	set(KIWI_CLIENT_RESSOURCES ${KIWI_APP_ICON} ${KIWI_PATCHER_FILETYPE_ICON} ${KIWI_PLIST})
elseif(WIN32)
	set(KIWI_CLIENT_RESSOURCES ${KIWI_RESSOURCES_DIR}/Project/VisualStudio15/Kiwi.rc)
endif()

#--------------------------------------
# Flags
#--------------------------------------

if (WIN32)
	set(CMAKE_CXX_FLAGS "/EHsc")
	set(CMAKE_EXE_LINKER_FLAGS "")

	if (DEBUG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /Od")
		set(CMAKE_CXX_FLAGS_DEBUG "/MD")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
		set(CMAKE_CXX_FLAGS_RELEASE "/MD")
	endif()

    if (CMAKE_CL_64)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MACHINE:X64")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MACHINE:X86")
    endif()

elseif(APPLE)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-warning-option -Wno-pessimizing-move
                                            -Wno-overloaded-virtual -Wnounused-function -Wno-unused-local-typedefs")

    if (DEBUG)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()

elseif(LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")

  if (DEBUG)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
  else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
  endif()

endif()

#--------------------------------------
# Preprocessor macro
#--------------------------------------

if (WIN32)
    add_definitions("-D_CPPUNWIND")

	if (DEBUG)
		add_definitions("-D_CONSOLE=1")
	endif()
endif()

if (DEBUG)
    add_definitions(-DDEBUG=1)
else()
    add_definitions(-DNDEBUG=1)
endif()

#--------------------------------------
# Search libraries option
#--------------------------------------

if (WIN32)
	set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "" "lib")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES} ".lib" ".dll")
	if (CMAKE_CL_64)
    set(WINDOWS_SDK_PATH "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64")
	else()
		set(WINDOWS_SDK_PATH "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib")
	endif()
else()
    set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "" "lib")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES} ".a" ".so")
endif()

#--------------------------------------
# Macros
#--------------------------------------

include(./Scripts/group.cmake)
include(./Scripts/dependency.cmake)

#----------------------------------
# Juce
#----------------------------------

set(JUCE_INCLUDE_DIRS ${ROOT_DIR}/ThirdParty/Juce/modules)

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
    find_package(PkgConfig)
    pkg_check_modules(GTK3_X11 REQUIRED gtk+-x11-3.0)
    pkg_check_modules(WEBKIT_TO_GTK4 REQUIRED webkit2gtk-4.0)
    set(JUCE_INCLUDE_DIRS ${JUCE_INCLUDE_DIRS} /usr/include/freetype2 ${GTK3_X11_INCLUDE_DIRS} ${WEBKIT_TO_GTK4_INCLUDE_DIRS})
    set(JUCE_DEP ${RT} ${DL} ${PTHREAD} ${FREETYPE} ${X11} ${XEXT} ${ASOUND} ${GTK3_X11_LIBRARIES} ${WEBKIT_TO_GTK4_LIBRARIES})
endif()

set(JUCE_MACRO_DEFINITIONS JUCE_STANDALONE_APPLICATION=1
                           JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
                           JUCE_DISPLAY_SPLASH_SCREEN=0
                           JUCE_REPORT_APP_USAGE=0)

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
target_include_directories(Juce PUBLIC ${JUCE_INCLUDE_DIRS})
set_target_properties(Juce PROPERTIES FOLDER ThirdParty)
source_group_rec("${JUCE_SRC}" ${ROOT_DIR}/ThirdParty/Juce/modules)

#----------------------------------
# Flip
#----------------------------------

if(WIN32)
    if (CMAKE_CL_64)
        set(KIWI_FLIP_LIB $ENV{KIWI_FLIP_LIB_x64})
        set(KIWI_FLIP_INCLUDE $ENV{KIWI_FLIP_INCLUDE_x64})
    else()
        set(KIWI_FLIP_LIB $ENV{KIWI_FLIP_LIB_WIN32})
        set(KIWI_FLIP_INCLUDE $ENV{KIWI_FLIP_INCLUDE_WIN32})
    endif()
else()
    set(KIWI_FLIP_LIB $ENV{KIWI_FLIP_LIB})
    set(KIWI_FLIP_INCLUDE $ENV{KIWI_FLIP_INCLUDE})
endif()

message(STATUS "Searching flip library")
message(STATUS "Library search path is : " ${KIWI_FLIP_LIB})
message(STATUS "Include search path is : " ${KIWI_FLIP_INCLUDE})

find_library(FLIP_LIBRARIES NAMES flip PATHS ${KIWI_FLIP_LIB} NO_DEFAULT_PATH)
find_path(FLIP_INCLUDE_DIRS NAMES flip PATHS ${KIWI_FLIP_INCLUDE} NO_DEFAULT_PATH)

if (WIN32)
    set(FLIP_LIBRARIES ${FLIP_LIBRARIES} ${WS2_32})
endif()

if (FLIP_LIBRARIES AND FLIP_INCLUDE_DIRS)
    message(STATUS "Flip found on system")
    message(STATUS "FLIP_LIBRARIES : " ${FLIP_LIBRARIES})
    message(STATUS "FLIP_INCLUDE_DIRS : " ${FLIP_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Flip was not found on system, check Readme.md for instructions on how to set searh path")
endif()

set(FLIP_COMPILE_DEFINITIONS "flip_NOTHING=flip_NOTHING_FATAL")

#----------------------------------
# Beast
#----------------------------------

set(Boost_INCLUDE_DIR $ENV{KIWI_BOOST_INCLUDE})

if (WIN32)
    if (CMAKE_CL_64)
        set(Boost_LIBRARY_DIR $ENV{KIWI_BOOST_LIB_x64})
    else()
        set(Boost_LIBRARY_DIR $ENV{KIWI_BOOST_LIB_WIN32})
    endif()
else()
    set(Boost_LIBRARY_DIR $ENV{KIWI_BOOST_LIB})
    set(Boost_NO_BOOST_CMAKE true)
    set(Boost_NO_SYSTEM_PATHS true)
endif()

option (Boost_USE_STATIC_LIBS "Use static libraries for boost" ON)
find_package (Boost 1.63.0 REQUIRED COMPONENTS system)

if (Boost_FOUND)
    message(STATUS "Boost found on system")
    message(STATUS "Boost_LIBRARIES : " ${Boost_LIBRARIES})
    message(STATUS "BOOST_INCLUDE_DIRECTORES : " ${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found on system")
endif()

#----------------------------------
# Cpr
#----------------------------------

# Cpr settings

set(CMAKE_USE_OPENSSL OFF CACHE BOOL "Don't use openssl")
set(BUILD_CPR_TESTS OFF CACHE BOOL "Don't build cpr tests")

if(WIN32)
    if (CMAKE_CL_64)
        set(KIWI_CURL_LIB $ENV{KIWI_CURL_LIB_x64})
        set(KIWI_CURL_INCLUDE $ENV{KIWI_CURL_INCLUDE_x64})
    else()
        set(KIWI_CURL_LIB $ENV{KIWI_CURL_LIB_WIN32})
        set(KIWI_CURL_INCLUDE $ENV{KIWI_CURL_INCLUDE_WIN32})
    endif()
else()
    set(KIWI_CURL_LIB $ENV{KIWI_CURL_LIB})
    set(KIWI_CURL_INCLUDE $ENV{KIWI_CURL_INCLUDE})
endif()

message(STATUS "Searching curl library")
message(STATUS "Library search path is : " ${KIWI_CURL_LIB})
message(STATUS "Include search path is : " ${KIWI_CURL_INCLUDE})

find_library(CURL_LIBRARIES NAMES curl PATHS ${KIWI_CURL_LIB} NO_DEFAULT_PATH)
find_path(CURL_INCLUDE_DIRS NAMES curl PATHS ${KIWI_CURL_INCLUDE} NO_DEFAULT_PATH)

if (CURL_LIBRARIES AND CURL_INCLUDE_DIRS)
    set(USE_SYSTEM_CURL ON CACHE BOOL "Use system curl")
    set(CURL_FOUND TRUE CACHE BOOL "Curl found")
    set(CMAKE_DISABLE_FIND_PACKAGE_CURL TRUE CACHE BOOL "Disactivate find package curl")
    message(STATUS "Libcurl found on system")
    message(STATUS "CURL_LIBRARIES : " ${CURL_LIBRARIES})
    message(STATUS "CURL_INCLUDE_DIRS : " ${CURL_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Curl was not found on system, check Readme.md for instructions on how to set searh path")
endif()

add_subdirectory(ThirdParty/cpr)
set_target_properties(${CPR_LIBRARIES} PROPERTIES FOLDER ThirdParty)
set_target_properties(${CPR_LIBRARIES} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
target_compile_definitions(${CPR_LIBRARIES} PUBLIC -DCURL_STATICLIB=1)

#----------------------------------
# Json
#----------------------------------

set(JSON_INCLUDE_DIRS ${ROOT_DIR}/ThirdParty/json)

#----------------------------------
# Model
#----------------------------------

file(GLOB_RECURSE KIWI_MODEL_SRC ${ROOT_DIR}/Modules/KiwiModel/*.[c|h]pp
                                 ${ROOT_DIR}/Modules/KiwiModel/*.h)
add_library(KiwiModel STATIC ${KIWI_MODEL_SRC})
target_include_directories(KiwiModel PUBLIC ${ROOT_DIR}/Modules ${FLIP_INCLUDE_DIRS})
target_link_libraries(KiwiModel ${FLIP_LIBRARIES})
target_compile_definitions(KiwiModel PUBLIC ${FLIP_COMPILE_DEFINITIONS})
set_target_properties(KiwiModel PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_MODEL_SRC}" ${ROOT_DIR}/Modules/KiwiModel)

#----------------------------------
# Dsp
#----------------------------------

file(GLOB_RECURSE KIWI_DSP_SRC ${ROOT_DIR}/Modules/KiwiDsp/*.[c|h]pp
                               ${ROOT_DIR}/Modules/KiwiDsp/*.h)
add_library(KiwiDsp STATIC ${KIWI_DSP_SRC})
target_compile_definitions(KiwiDsp PUBLIC -DKIWI_DSP_FLOAT=1)
target_include_directories(KiwiDsp PUBLIC ${ROOT_DIR}/Modules)
set_target_properties(KiwiDsp PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_DSP_SRC}" ${ROOT_DIR}/Modules/KiwiDsp)

#----------------------------------
# Network
#----------------------------------

file(GLOB_RECURSE KIWI_NETWORK_SRC ${ROOT_DIR}/Modules/KiwiNetwork/*.[c|h]pp
                                   ${ROOT_DIR}/Modules/KiwiNetwork/*.h)
add_library(KiwiNetwork STATIC ${KIWI_NETWORK_SRC})
target_include_directories(KiwiNetwork PUBLIC ${ROOT_DIR}/Modules
                                              ${ROOT_DIR}/ThirdParty/Beast/include
                                              ${Boost_INCLUDE_DIRS})
target_compile_definitions(KiwiNetwork PUBLIC "-DBOOST_ALL_NO_LIB=1")
set(NETWORK_DEP ${Boost_LIBRARIES})
if (WIN32)
    set(NETWORK_DEP ${NETWORK_DEP} ${WS2_32})
elseif(LINUX)
    set(NETWORK_DEP ${NETWORK_DEP} ${PTHREAD})
endif()
target_link_libraries(KiwiNetwork PUBLIC ${NETWORK_DEP})
set_target_properties(KiwiNetwork PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_NETWORK_SRC}" ${ROOT_DIR}/Modules/KiwiNetwork)


#----------------------------------
# Engine
#----------------------------------

file(GLOB_RECURSE KIWI_ENGINE_SRC ${ROOT_DIR}/Modules/KiwiEngine/*.[c|h]pp
                                  ${ROOT_DIR}/Modules/KiwiEngine/*.h)
add_library(KiwiEngine STATIC ${KIWI_ENGINE_SRC})
target_include_directories(KiwiEngine PUBLIC ${ROOT_DIR}/ThirdParty/concurrentqueue)
target_include_directories(KiwiEngine PUBLIC ${ROOT_DIR}/Modules)
target_add_dependency(KiwiEngine KiwiModel)
target_add_dependency(KiwiEngine KiwiDsp)
set_target_properties(KiwiEngine PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_ENGINE_SRC}" ${ROOT_DIR}/Modules/KiwiEngine)

#----------------------------------
# Server
#----------------------------------

file(GLOB_RECURSE KIWI_SERVER_SRC ${ROOT_DIR}/Modules/KiwiServer/*.[c|h]pp
                                  ${ROOT_DIR}/Modules/KiwiServer/*.h)
add_library(KiwiServer ${KIWI_SERVER_SRC})
target_include_directories(KiwiServer PUBLIC ${JSON_INCLUDE_DIRS})
target_add_dependency(KiwiServer KiwiModel)
target_add_dependency(KiwiServer Juce)
set_target_properties(KiwiServer PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_SERVER_SRC}" ${ROOT_DIR}/Modules/KiwiServer)

file(GLOB_RECURSE SERVER_SRC ${ROOT_DIR}/Server/Source/*.[c|h]pp
                             ${ROOT_DIR}/Server/Source/*.h)
add_executable(Server ${SERVER_SRC})
target_add_dependency(Server KiwiServer)
source_group_rec("${SERVER_SRC}" ${ROOT_DIR}/Server/Source)

#----------------------------------
# Client
#----------------------------------

file(GLOB_RECURSE KIWI_CLIENT_SRC ${ROOT_DIR}/Client/Source/*.[c|h]pp
                                  ${ROOT_DIR}/Client/Source/*.h)
add_executable(Kiwi ${KIWI_CLIENT_SRC} "${KIWI_CLIENT_RESSOURCES}")
target_add_dependency(Kiwi KiwiEngine)
target_add_dependency(Kiwi KiwiModel)
target_add_dependency(Kiwi KiwiDsp)
target_add_dependency(Kiwi Juce)
target_add_dependency(Kiwi ${CPR_LIBRARIES})
target_include_directories(Kiwi PUBLIC ${JSON_INCLUDE_DIRS} ${CPR_INCLUDE_DIRS})

if (APPLE)
	set_target_properties(Kiwi PROPERTIES MACOSX_BUNDLE TRUE
									      MACOSX_BUNDLE_INFO_PLIST ${KIWI_PLIST})
elseif(WIN32)
	if (CMAKE_CL_64)
		set(CRT_LIB "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/redist/x64/Microsoft.VC140.CRT")
	else()
		set(CRT_LIB "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/redist/x86/Microsoft.VC140.CRT")
	endif()

	add_custom_command(TARGET Kiwi POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CRT_LIB} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}" USES_TERMINAL)

	if (DEBUG)
		set_target_properties(Kiwi PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
	else()
		set_target_properties(Kiwi PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	endif()
endif()

source_group_rec("${KIWI_CLIENT_SRC}" ${ROOT_DIR}/Client/Source)

#----------------------------------
# Tests
#----------------------------------

# Test Dsp

file(GLOB TEST_DSP_SRC ${ROOT_DIR}/Test/Dsp/*.[c|h]pp
                       ${ROOT_DIR}/Test/Dsp/*.h)
add_executable(test_dsp ${TEST_DSP_SRC})
target_add_dependency(test_dsp KiwiDsp)
set_target_properties(test_dsp PROPERTIES FOLDER Test)
source_group_rec("${TEST_DSP_SRC}" ${ROOT_DIR}/Test/Dsp)

# Test Model

file(GLOB TEST_MODEL_SRC ${ROOT_DIR}/Test/Model/*.[c|h]pp
                         ${ROOT_DIR}/Test/Model/*.h)
add_executable(test_model ${TEST_MODEL_SRC})
target_add_dependency(test_model KiwiModel)
set_target_properties(test_model PROPERTIES FOLDER Test)
source_group_rec("${TEST_MODEL_SRC}" ${ROOT_DIR}/Test/Model)

# Test Network

file(GLOB TEST_NETWORK_SRC ${ROOT_DIR}/Test/Network/*.[c|h]pp
                           ${ROOT_DIR}/Test/Network/*.h)
add_executable(test_network ${TEST_NETWORK_SRC})
target_add_dependency(test_network KiwiNetwork)
set_target_properties(test_network PROPERTIES FOLDER Test)
source_group_rec("${TEST_NETWORK_SRC}" ${ROOT_DIR}/Test/Network)

# Test Engine

file(GLOB TEST_ENGINE_SRC ${ROOT_DIR}/Test/Engine/*.[c|h]pp
                          ${ROOT_DIR}/Test/Engine/*.h)
add_executable(test_engine ${TEST_ENGINE_SRC})
target_add_dependency(test_engine KiwiEngine)
if (LINUX)
  target_link_libraries(test_engine PUBLIC ${PTHREAD})
endif()
set_target_properties(test_engine PROPERTIES FOLDER Test)
source_group_rec("${TEST_ENGINE_SRC}" ${ROOT_DIR}/Test/Engine)

# Test Server

file(GLOB TEST_SERVER_SRC ${ROOT_DIR}/Test/Server/*.[c|h]pp
                          ${ROOT_DIR}/Test/Server/*.h)
add_executable(test_server ${TEST_SERVER_SRC})
target_add_dependency(test_server KiwiServer)
set_target_properties(test_server PROPERTIES FOLDER Test)
if (LINUX)
  target_link_libraries(test_server PUBLIC ${PTHREAD})
endif()
source_group_rec("${TEST_SERVER_SRC}" ${ROOT_DIR}/Test/Server)

# Coverage setting
if(${GCOV_SUPPORT} STREQUAL "On" AND DEBUG)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
  target_link_libraries(test_dsp PUBLIC gcov)
  target_link_libraries(test_model PUBLIC gcov)
  target_link_libraries(test_engine PUBLIC gcov)
  target_link_libraries(test_server PUBLIC gcov)
  target_link_libraries(test_network PUBLIC gcov)
endif()

# Tests Target

add_custom_target(Tests ALL DEPENDS test_dsp test_model test_engine test_network)
set_target_properties(Tests PROPERTIES FOLDER Test)

add_custom_command(TARGET Tests
                   POST_BUILD COMMAND test_dsp
                              COMMAND test_model
                              COMMAND test_engine
                              COMMAND test_server
                              COMMAND test_network
                   USES_TERMINAL)
