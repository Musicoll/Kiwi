cmake_minimum_required(VERSION 3.0.2)

project(Kiwi CXX)

#--------------------------------------
# Global Settings
#--------------------------------------

option(GCOV_SUPPORT "Build for gcov" Off)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/KiwiBuild")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/KiwiBuild")

#--------------------------------------
# Compiler Check
#--------------------------------------

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9)
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")

#--------------------------------------
# Ressources
#--------------------------------------

file(GLOB_RECURSE FAUST_LIB_RESOURCE ${PROJECT_SOURCE_DIR}/ThirdParty/faust/libraries/*.lib)
set(KIWI_RESSOURCES_DIR ${PROJECT_SOURCE_DIR}/Ressources)
if (APPLE)
	set(KIWI_APP_ICON ${KIWI_RESSOURCES_DIR}/Project/XCode/Icon.icns)
	set(KIWI_PATCHER_FILETYPE_ICON ${KIWI_RESSOURCES_DIR}/Project/XCode/kiwi_filetype.icns)
	set(KIWI_PLIST ${KIWI_RESSOURCES_DIR}/Project/XCode/Info.plist)
	set_source_files_properties(${KIWI_APP_ICON} ${KIWI_PATCHER_FILETYPE_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
  set_source_files_properties(${FAUST_LIB_RESOURCE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/Faust/Libs")
	set(KIWI_CLIENT_RESSOURCES ${KIWI_APP_ICON} ${KIWI_PATCHER_FILETYPE_ICON} ${KIWI_PLIST} ${FAUST_LIB_RESOURCE} )
elseif(WIN32)
	set(KIWI_CLIENT_RESSOURCES ${KIWI_RESSOURCES_DIR}/Project/VisualStudio15/Kiwi.rc)
endif()

#-------------------------------------------------------------------------------
# COMPILATION FLAGS
#-------------------------------------------------------------------------------

if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi /Od")
else()
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-multichar")
  endif()
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
endif()

#-------------------------------------------------------------------------------
# PREPROCESSOR DEFINTIIONS
#-------------------------------------------------------------------------------

if (WIN32)
    add_definitions("-D_CPPUNWIND")
    add_definitions("-D_WIN32_WINNT=0x0501")
    add_definitions("-D_USE_MATH_DEFINES")
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
	if (CMAKE_CL_64)
    set(WINDOWS_SDK_PATH "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64")
	else()
		set(WINDOWS_SDK_PATH "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib")
	endif()
endif()

#--------------------------------------
# Macros
#--------------------------------------

include(./Scripts/group.cmake)
include(./Scripts/dependency.cmake)

#----------------------------------
# Juce
#----------------------------------

set(JUCE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules)

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
elseif(UNIX)
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

file (GLOB_RECURSE JUCE_SRC ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/*.[h|c]pp
                            ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/*.h
                            ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/*.mm)

set_source_files_properties(${JUCE_SRC} PROPERTIES HEADER_FILE_ONLY TRUE)

set(JUCE_COMPILED_SRC ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_core/juce_core.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_data_structures/juce_data_structures.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_events/juce_events.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_graphics/juce_graphics.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_gui_basics/juce_gui_basics.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_gui_extra/juce_gui_extra.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_audio_basics/juce_audio_basics.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_audio_devices/juce_audio_devices.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_audio_formats/juce_audio_formats.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_audio_utils/juce_audio_utils.cpp
                      ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules/juce_audio_processors/juce_audio_processors.cpp)

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
source_group_rec("${JUCE_SRC}" ${PROJECT_SOURCE_DIR}/ThirdParty/Juce/modules)


# Dependencies
#-------------------------------------------------------------------------------
message(STATUS "")
include(CMake/ConfigFlip.cmake)
include(CMake/ConfigBoost.cmake)
include(CMake/ConfigLLVM.cmake)
include(CMake/ConfigFAUST.cmake)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

set(JSON_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ThirdParty/json)

#----------------------------------
# KiwiTool
#----------------------------------

file(GLOB_RECURSE KIWI_TOOL_SRC ${PROJECT_SOURCE_DIR}/Modules/KiwiTool/*.[c|h]pp
                                ${PROJECT_SOURCE_DIR}/Modules/KiwiTool/*.h)
add_library(KiwiTool STATIC ${KIWI_TOOL_SRC})
target_include_directories(KiwiTool PUBLIC ${PROJECT_SOURCE_DIR}/Modules ${PROJECT_SOURCE_DIR}/ThirdParty/concurrentqueue)
set_target_properties(KiwiTool PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_TOOL_SRC}" ${PROJECT_SOURCE_DIR}/Modules/KiwiTool)

#----------------------------------
# Model
#----------------------------------

file(GLOB_RECURSE KIWI_MODEL_SRC ${PROJECT_SOURCE_DIR}/Modules/KiwiModel/*.[c|h]pp
                                 ${PROJECT_SOURCE_DIR}/Modules/KiwiModel/*.h)
add_library(KiwiModel STATIC ${KIWI_MODEL_SRC})
target_add_dependency(KiwiModel KiwiTool)
target_include_directories(KiwiModel PUBLIC ${PROJECT_SOURCE_DIR}/Modules ${FLIP_INCLUDE_DIRS})
target_link_libraries(KiwiModel PUBLIC ${FLIP_LIBRARIES})
target_compile_definitions(KiwiModel PUBLIC ${FLIP_COMPILE_DEFINITIONS})
set_target_properties(KiwiModel PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_MODEL_SRC}" ${PROJECT_SOURCE_DIR}/Modules/KiwiModel)

#----------------------------------
# Dsp
#----------------------------------

file(GLOB_RECURSE KIWI_DSP_SRC ${PROJECT_SOURCE_DIR}/Modules/KiwiDsp/*.[c|h]pp
                               ${PROJECT_SOURCE_DIR}/Modules/KiwiDsp/*.h)
add_library(KiwiDsp STATIC ${KIWI_DSP_SRC})
target_compile_definitions(KiwiDsp PUBLIC -DKIWI_DSP_FLOAT=1)
target_include_directories(KiwiDsp PUBLIC ${PROJECT_SOURCE_DIR}/Modules)
set_target_properties(KiwiDsp PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_DSP_SRC}" ${PROJECT_SOURCE_DIR}/Modules/KiwiDsp)

#----------------------------------
# Network
#----------------------------------

file(GLOB_RECURSE KIWI_NETWORK_SRC ${PROJECT_SOURCE_DIR}/Modules/KiwiNetwork/*.[c|h]pp
                                   ${PROJECT_SOURCE_DIR}/Modules/KiwiNetwork/*.h)
add_library(KiwiNetwork STATIC ${KIWI_NETWORK_SRC})
target_include_directories(KiwiNetwork PUBLIC ${PROJECT_SOURCE_DIR}/Modules
                                              ${PROJECT_SOURCE_DIR}/ThirdParty/Beast/include
                                              ${Boost_INCLUDE_DIRS})
target_compile_definitions(KiwiNetwork PUBLIC "-DBOOST_ALL_NO_LIB=1")
#add_dependencies(KiwiNetwork Boost)
set(NETWORK_DEP ${Boost_LIBRARIES})
if (WIN32)
    set(NETWORK_DEP ${NETWORK_DEP} ${WS2_32})
elseif(UNIX AND NOT APPLE)
    set(NETWORK_DEP ${NETWORK_DEP} ${PTHREAD})
endif()
target_link_libraries(KiwiNetwork PUBLIC ${NETWORK_DEP})
set_target_properties(KiwiNetwork PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_NETWORK_SRC}" ${PROJECT_SOURCE_DIR}/Modules/KiwiNetwork)


#----------------------------------
# Engine
#----------------------------------

file(GLOB_RECURSE KIWI_ENGINE_SRC ${PROJECT_SOURCE_DIR}/Modules/KiwiEngine/*.[c|h]pp
                                  ${PROJECT_SOURCE_DIR}/Modules/KiwiEngine/*.h)
add_library(KiwiEngine STATIC ${KIWI_ENGINE_SRC})
target_include_directories(KiwiEngine PUBLIC ${PROJECT_SOURCE_DIR}/Modules)
target_add_dependency(KiwiEngine KiwiModel)
target_add_dependency(KiwiEngine KiwiDsp)
target_add_dependency(KiwiEngine Juce)
add_dependencies(KiwiEngine staticlib)
target_link_libraries(KiwiEngine PUBLIC ${LLVM_ALL_LIBRARIES})
target_link_libraries(KiwiEngine PUBLIC staticlib)
set_target_properties(KiwiEngine PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_ENGINE_SRC}" ${PROJECT_SOURCE_DIR}/Modules/KiwiEngine)

#----------------------------------
# Server
#----------------------------------

file(GLOB_RECURSE KIWI_SERVER_SRC ${PROJECT_SOURCE_DIR}/Modules/KiwiServer/*.[c|h]pp
                                  ${PROJECT_SOURCE_DIR}/Modules/KiwiServer/*.h)
add_library(KiwiServer ${KIWI_SERVER_SRC})
target_include_directories(KiwiServer PUBLIC ${JSON_INCLUDE_DIRS})
target_add_dependency(KiwiServer KiwiModel)
target_add_dependency(KiwiServer Juce)
set_target_properties(KiwiServer PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_SERVER_SRC}" ${PROJECT_SOURCE_DIR}/Modules/KiwiServer)

file(GLOB_RECURSE SERVER_SRC ${PROJECT_SOURCE_DIR}/Server/Source/*.[c|h]pp
                             ${PROJECT_SOURCE_DIR}/Server/Source/*.h)
add_executable(Server ${SERVER_SRC})
target_add_dependency(Server KiwiServer)
source_group_rec("${SERVER_SRC}" ${PROJECT_SOURCE_DIR}/Server/Source)

#----------------------------------
# Client
#----------------------------------

file(GLOB_RECURSE KIWI_CLIENT_SRC ${PROJECT_SOURCE_DIR}/Client/Source/*.[c|h]pp ${PROJECT_SOURCE_DIR}/Client/Source/*.h)
add_executable(Kiwi ${KIWI_CLIENT_SRC} "${KIWI_CLIENT_RESSOURCES}")
target_add_dependency(Kiwi KiwiNetwork)
target_add_dependency(Kiwi KiwiEngine)
target_add_dependency(Kiwi KiwiModel)
target_add_dependency(Kiwi KiwiDsp)
target_add_dependency(Kiwi Juce)
target_include_directories(Kiwi PUBLIC ${JSON_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/Client/Source)


if (APPLE)
	set_target_properties(Kiwi PROPERTIES MACOSX_BUNDLE TRUE MACOSX_BUNDLE_INFO_PLIST ${KIWI_PLIST})
elseif(WIN32)
	if (CMAKE_CL_64)
		set(CRT_LIB "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/redist/x64/Microsoft.VC140.CRT")
	else()
		set(CRT_LIB "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/redist/x86/Microsoft.VC140.CRT")
	endif()

	add_custom_command(TARGET Kiwi POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CRT_LIB} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}" USES_TERMINAL)

  set_target_properties(Kiwi PROPERTIES LINK_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
  set_target_properties(Kiwi PROPERTIES LINK_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

source_group_rec("${KIWI_CLIENT_SRC}" ${PROJECT_SOURCE_DIR}/Client/Source)

#----------------------------------
# Tests
#----------------------------------

# Test Tool

file(GLOB TEST_TOOL_SRC ${PROJECT_SOURCE_DIR}/Test/Tool/*.[c|h]pp
                        ${PROJECT_SOURCE_DIR}/Test/Tool/*.h)
add_executable(test_tool ${TEST_TOOL_SRC})
target_add_dependency(test_tool KiwiTool)
set_target_properties(test_tool PROPERTIES FOLDER Test)
if (UNIX AND NOT APPLE)
  target_link_libraries(test_tool PUBLIC ${PTHREAD})
endif()
source_group_rec("${TEST_TOOL_SRC}" ${PROJECT_SOURCE_DIR}/Test/Tool)

# Test Dsp

file(GLOB TEST_DSP_SRC ${PROJECT_SOURCE_DIR}/Test/Dsp/*.[c|h]pp
                       ${PROJECT_SOURCE_DIR}/Test/Dsp/*.h)
add_executable(test_dsp ${TEST_DSP_SRC})
target_add_dependency(test_dsp KiwiDsp)
set_target_properties(test_dsp PROPERTIES FOLDER Test)
source_group_rec("${TEST_DSP_SRC}" ${PROJECT_SOURCE_DIR}/Test/Dsp)

# Test Model

file(GLOB TEST_MODEL_SRC ${PROJECT_SOURCE_DIR}/Test/Model/*.[c|h]pp
                         ${PROJECT_SOURCE_DIR}/Test/Model/*.h)
add_executable(test_model ${TEST_MODEL_SRC})
target_add_dependency(test_model KiwiModel)
set_target_properties(test_model PROPERTIES FOLDER Test)
source_group_rec("${TEST_MODEL_SRC}" ${PROJECT_SOURCE_DIR}/Test/Model)

# Test Network

file(GLOB TEST_NETWORK_SRC ${PROJECT_SOURCE_DIR}/Test/Network/*.[c|h]pp
                           ${PROJECT_SOURCE_DIR}/Test/Network/*.h)
add_executable(test_network ${TEST_NETWORK_SRC})
target_add_dependency(test_network KiwiNetwork)
set_target_properties(test_network PROPERTIES FOLDER Test)
source_group_rec("${TEST_NETWORK_SRC}" ${PROJECT_SOURCE_DIR}/Test/Network)

# Test Server

file(GLOB TEST_SERVER_SRC ${PROJECT_SOURCE_DIR}/Test/Server/*.[c|h]pp
                          ${PROJECT_SOURCE_DIR}/Test/Server/*.h)
add_executable(test_server ${TEST_SERVER_SRC})
target_add_dependency(test_server KiwiServer)
set_target_properties(test_server PROPERTIES FOLDER Test)
if (UNIX AND NOT APPLE)
  target_link_libraries(test_server PUBLIC ${PTHREAD})
endif()
source_group_rec("${TEST_SERVER_SRC}" ${PROJECT_SOURCE_DIR}/Test/Server)

# Coverage setting
if(${GCOV_SUPPORT} STREQUAL "On")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
  target_link_libraries(test_dsp PUBLIC gcov)
  target_link_libraries(test_model PUBLIC gcov)
  #target_link_libraries(test_engine PUBLIC gcov)
  target_link_libraries(test_server PUBLIC gcov)
  target_link_libraries(test_network PUBLIC gcov)
  target_link_libraries(test_tool PUBLIC gcov)
endif()

# Tests Target

add_custom_target(Tests ALL DEPENDS test_dsp test_model test_network test_tool)
set_target_properties(Tests PROPERTIES FOLDER Test)

add_custom_command(TARGET Tests
                   POST_BUILD COMMAND test_dsp
                              COMMAND test_model
                              COMMAND test_tool
                              COMMAND test_server
                              COMMAND test_network
                   USES_TERMINAL)
