cmake_minimum_required (VERSION 3.0.2)

project (Kiwi CXX)

#--------------------------------------
# Global Settings
#--------------------------------------

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

set(KIWI_ICON ${ROOT_DIR}/Config/Icon.icns)
set(KIWI_PLIST ${ROOT_DIR}/Config/Info.plist)
set_source_files_properties(${KIWI_ICON} ${KIWI_PLIST} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wno-unknown-warning-option -Wno-pessimizing-move -Wno-overloaded-virtual")
    
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
    set(JUCE_DEP ${COCOA} ${IOKIT} ${QUARTZCORE} ${CARBON} ${WEBKIT})
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
endif()

set(JUCE_MACRO_DEFINITIONS JUCE_MODULE_AVAILABLE_juce_core=1
                           JUCE_MODULE_AVAILABLE_juce_data_structures=1
                           JUCE_MODULE_AVAILABLE_juce_events=1
                           JUCE_MODULE_AVAILABLE_juce_graphics=1
                           JUCE_MODULE_AVAILABLE_juce_gui_basics=1
                           JUCE_MODULE_AVAILABLE_juce_gui_extra=1
                           JUCE_STANDALONE_APPLICATION=1
                           JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
               
file (GLOB_RECURSE JUCE_SRC ${ROOT_DIR}/ThirdParty/Juce/modules/*.[h|c]pp 
                            ${ROOT_DIR}/ThirdParty/Juce/modules/*.h
                            ${ROOT_DIR}/ThirdParty/Juce/modules/*.mm)
                            
set_source_files_properties(${JUCE_SRC} PROPERTIES HEADER_FILE_ONLY TRUE)
               
set(JUCE_COMPILED_SRC ${ROOT_DIR}/ThirdParty/Juce/modules/juce_core/juce_core.mm
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_data_structures/juce_data_structures.mm
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_events/juce_events.mm
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_graphics/juce_graphics.mm
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_gui_basics/juce_gui_basics.mm
                      ${ROOT_DIR}/ThirdParty/Juce/modules/juce_gui_extra/juce_gui_extra.mm)
             
set_source_files_properties(${JUCE_COMPILED_SRC} PROPERTIES HEADER_FILE_ONLY FALSE)
               

add_library(Juce STATIC ${JUCE_SRC})
target_link_libraries(Juce ${JUCE_DEP})
target_compile_definitions(Juce PUBLIC ${JUCE_MACRO_DEFINITIONS})
target_include_directories(Juce PUBLIC ${ROOT_DIR}/ThirdParty/Juce/modules)
set_target_properties(Juce PROPERTIES FOLDER ThirdParty)
source_group_rec("${JUCE_SRC}" ${ROOT_DIR}/ThirdParty/Juce/modules)

#----------------------------------
# Flip
#----------------------------------

if (APPLE)
    if (${FLIP_PRIVATE})
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            find_library(FlipLib NAMES flip PATHS ${ROOT_DIR}/ThirdParty/flip/projects/build/Debug)
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            find_library(FlipLib NAMES flip PATHS ${ROOT_DIR}/ThirdParty/flip/projects/build/Release)
        endif()
    else()
        find_library(FlipLib NAMES flip PATHS ${ROOT_DIR}/ThirdParty/flip-demo/lib/mac)
    endif()
    set(FLIP_DEP ${FlipLib})
elseif (WIN32)
	if (CMAKE_CL_64)
        if (${FLIP_PRIVATE})
            if (CMAKE_BUILD_TYPE STREQUAL "Debug")
                find_library(FlipLib NAMES flip PATHS "${ROOT_DIR}/ThirdParty/flip/projects/build/lib/x64/Debug")
            elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
                find_library(FlipLib NAMES flip PATHS "${ROOT_DIR}/ThirdParty/flip/projects/build/lib/x64/Release")
            endif()
        else()
            find_library(FlipLib NAMES flip PATHS "${ROOT_DIR}/ThirdParty/flip-demo/lib/windows/VS2015/x64/Release")
        endif()
	else()
        if (${FLIP_PRIVATE})
            if (CMAKE_BUILD_TYPE STREQUAL "Debug")
                find_library(FlipLib NAMES flip PATHS "${ROOT_DIR}/ThirdParty/flip/projects/build/Win32/Debug")
            elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
                find_library(FlipLib NAMES flip PATHS "${ROOT_DIR}/ThirdParty/flip/projects/build/Win32/Release")
            endif()
        else()
            find_library(FlipLib NAMES flip PATHS "${ROOT_DIR}/ThirdParty/flip-demo/lib/windows/VS2015/Win32/Release")
        endif()
	endif()
    
    find_library(WS2_32 ws2_32 PATHS ${WINDOWS_SDK_PATH} NO_DEFAULT_PATH)
	set(FLIP_DEP ${WS2_32} ${FlipLib})
endif()

if (${FLIP_PRIVATE})
    set(FLIP_DIR "${ROOT_DIR}/ThirdParty/flip")
    file (GLOB_RECURSE FLIP_SRC ${ROOT_DIR}/ThirdParty/flip/inlude/*.hpp
                                ${ROOT_DIR}/ThirdParty/flip/include/*.h
                                ${ROOT_DIR}/ThirdParty/flip/src/*.cpp)
else()
    set(FLIP_DIR "${ROOT_DIR}/ThirdParty/flip-demo")
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
# Engine
#----------------------------------

file(GLOB_RECURSE KIWI_ENGINE_SRC ${ROOT_DIR}/Modules/KiwiEngine/*.[c|h]pp)
add_library(KiwiEngine STATIC ${KIWI_ENGINE_SRC})
target_include_directories(KiwiEngine PUBLIC ${ROOT_DIR}/Modules)
target_add_dependency(KiwiEngine Flip)
target_add_dependency(KiwiEngine KiwiModel)
set_target_properties(KiwiEngine PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_ENGINE_SRC}" ${ROOT_DIR}/Modules/KiwiEngine)

#----------------------------------
# Client
#----------------------------------

file(GLOB_RECURSE KIWI_CLIENT_SRC ${ROOT_DIR}/Client/Source/*.[c|h]pp)
add_executable(Kiwi ${KIWI_CLIENT_SRC} ${KIWI_ICON} ${KIWI_PLIST})
target_add_dependency(Kiwi KiwiEngine)
target_add_dependency(Kiwi KiwiModel)
target_add_dependency(Kiwi Juce)

if (APPLE)
	set_target_properties(Kiwi PROPERTIES MACOSX_BUNDLE TRUE
									      MACOSX_BUNDLE_INFO_PLIST ${KIWI_PLIST})
elseif(WIN32)
	set_target_properties(Kiwi PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

source_group_rec("${KIWI_CLIENT_SRC}" ${ROOT_DIR}/Client/Source)

#----------------------------------
# Server
#----------------------------------

file(GLOB_RECURSE KIWI_SERVER_SRC ${ROOT_DIR}/Server/Source/*.[c|h]pp)
add_executable(Server ${KIWI_SERVER_SRC})
target_add_dependency(Server KiwiModel)
target_add_dependency(Server Flip)
source_group_rec("${KIWI_SERVER_SRC}" ${ROOT_DIR}/Server/Source)

#----------------------------------
# Tests
#----------------------------------

# Test Atom

file(GLOB TEST_ATOM_SRC ${ROOT_DIR}/Test/Atom/*.[c|h]pp)
add_executable(test_atom ${TEST_ATOM_SRC})
target_add_dependency(test_atom KiwiModel)
set_target_properties(test_atom PROPERTIES FOLDER Test)
source_group_rec("${TEST_ATOM_SRC}" ${ROOT_DIR}/Test/Atom)
set_target_properties(test_atom PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_PATH})