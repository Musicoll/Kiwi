cmake_minimum_required(VERSION 3.9)

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

#-------------------------------------------------------------------------------
# COMPILATION FLAGS
#-------------------------------------------------------------------------------

if(WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /O2")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /Zi /Od")
else()
  if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") #-Weverything -Wno-c++98-compat -Wno-undef")
		if(CMAKE_GENERATOR STREQUAL Xcode)
		    set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_VALUE YES)
				set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_VARIABLE YES)
				set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_PARAMETER NO)
				set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_LABEL YES)
				set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNUSED_FUNCTION YES)

				set(CMAKE_XCODE_ATTRIBUTE_CLANG_WARN_UNREACHABLE_CODE YES)
				set(CMAKE_XCODE_ATTRIBUTE_GCC_WARN_UNKNOWN_PRAGMAS YES)
		endif()
  else() # linux
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-multichar -Wno-sign-compare")
  endif()
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
endif()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG=1")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG=1")

#--------------------------------------
# Macros
#--------------------------------------

include(./Scripts/group.cmake)

# Dependencies
#-------------------------------------------------------------------------------
message(STATUS "")

if(NOT DEFINED KIWI_DEPENDENCIES_DIR)
	set(KIWI_DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/ThirdParty")
endif()

include(CMake/ConfigFlip.cmake)
include(CMake/ConfigBoost.cmake)
include(CMake/ConfigJUCE.cmake)
include(CMake/ConfigLLVM.cmake)
include(CMake/ConfigFAUST.cmake)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

set(JSON_INCLUDE_DIRS ${KIWI_DEPENDENCIES_DIR}/json)
set(CONCURRENTQUEUE_INCLUDE_DIRS ${KIWI_DEPENDENCIES_DIR}/concurrentqueue)

# Kiwi
# -----------------------------------------------------------------------------#
message(STATUS "Configuring Kiwi")
set(KIWI_MODULE_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/Modules)

# KiwiTool
# -----------------------------------------------------------------------------#
set(KIWI_TOOL_INCLUDE_DIRS ${KIWI_MODULE_INCLUDE_DIRS} ${CONCURRENTQUEUE_INCLUDE_DIRS})
set(KIWI_TOOL_COMPILE_DEFINITIONS "")
set(KIWI_TOOL_LINK_LIBRARIES "")

file(GLOB_RECURSE KIWI_TOOL_SOURCES ${KIWI_MODULE_INCLUDE_DIRS}/KiwiTool/*.[c|h]pp ${KIWI_MODULE_INCLUDE_DIRS}/KiwiTool/*.h)
add_library(KiwiTool STATIC ${KIWI_TOOL_SOURCES})
target_include_directories(KiwiTool PUBLIC ${KIWI_TOOL_INCLUDE_DIRS})

set_target_properties(KiwiTool PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_TOOL_SOURCES}" ${PROJECT_SOURCE_DIR}/Modules/KiwiTool)

# KiwiModel
#------------------------------------------------------------------------------#
set(KIWI_MODEL_DEPENDENCIES KiwiTool)
set(KIWI_MODEL_INCLUDE_DIRS ${KIWI_MODULE_INCLUDE_DIRS} ${KIWI_TOOL_INCLUDE_DIRS} ${FLIP_INCLUDE_DIRS})
set(KIWI_MODEL_COMPILE_DEFINITIONS ${KIWI_TOOL_COMPILE_DEFINITIONS} ${FLIP_COMPILE_DEFINITIONS})
set(KIWI_MODEL_LINK_LIBRARIES ${KIWI_TOOL_LINK_LIBRARIES} KiwiTool ${FLIP_LIBRARIES})

file(GLOB_RECURSE KIWI_MODEL_SOURCES ${KIWI_MODULE_INCLUDE_DIRS}/KiwiModel/*.[c|h]pp ${KIWI_MODULE_INCLUDE_DIRS}/KiwiModel/*.h)
add_library(KiwiModel STATIC ${KIWI_MODEL_SOURCES})
add_dependencies(KiwiModel ${KIWI_MODEL_DEPENDENCIES})
target_include_directories(KiwiModel PUBLIC ${KIWI_MODEL_INCLUDE_DIRS})
target_compile_definitions(KiwiModel PUBLIC ${KIWI_MODEL_COMPILE_DEFINITIONS})
target_link_libraries(KiwiModel PUBLIC ${KIWI_MODEL_LINK_LIBRARIES})

set_target_properties(KiwiModel PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_MODEL_SOURCES}" ${PROJECT_SOURCE_DIR}/Modules/KiwiModel)

# KiwiDsp
#------------------------------------------------------------------------------#
set(KIWI_DSP_DEPENDENCIES)
set(KIWI_DSP_INCLUDE_DIRS ${KIWI_MODULE_INCLUDE_DIRS})
set(KIWI_DSP_COMPILE_DEFINITIONS KIWI_DSP_FLOAT=1)
set(KIWI_DSP_LINK_LIBRARIES "")

file(GLOB_RECURSE KIWI_DSP_SOURCES ${KIWI_MODULE_INCLUDE_DIRS}/KiwiDsp/*.[c|h]pp ${KIWI_MODULE_INCLUDE_DIRS}/KiwiDsp/*.h)
add_library(KiwiDsp STATIC ${KIWI_DSP_SOURCES})
target_include_directories(KiwiDsp PUBLIC ${KIWI_DSP_INCLUDE_DIRS})
target_compile_definitions(KiwiDsp PUBLIC "${KIWI_DSP_COMPILE_DEFINITIONS}")
target_link_libraries(KiwiDsp PUBLIC ${KIWI_DSP_LINK_LIBRARIES})

set_target_properties(KiwiDsp PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_DSP_SOURCES}" ${PROJECT_SOURCE_DIR}/Modules/KiwiDsp)

# KiwiNetwork
#------------------------------------------------------------------------------#
set(KIWI_NETWORK_DEPENDENCIES)
set(KIWI_NETWORK_INCLUDE_DIRS ${KIWI_MODULE_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
set(KIWI_NETWORK_COMPILE_DEFINITIONS BOOST_ALL_NO_LIB=1)

if(APPLE)
	set(KIWI_NETWORK_LINK_LIBRARIES ${Boost_LIBRARIES})
elseif(UNIX)
  set(KIWI_NETWORK_LINK_LIBRARIES ${Boost_LIBRARIES} ${PTHREAD})
elseif(WIN32)
	set(KIWI_NETWORK_LINK_LIBRARIES ${Boost_LIBRARIES} ${ws2_32})
endif()

file(GLOB_RECURSE KIWI_NETWORK_SOURCES ${KIWI_MODULE_INCLUDE_DIRS}/KiwiNetwork/*.[c|h]pp ${KIWI_MODULE_INCLUDE_DIRS}/KiwiNetwork/*.h)
add_library(KiwiNetwork STATIC ${KIWI_NETWORK_SOURCES})
target_include_directories(KiwiNetwork PUBLIC ${KIWI_NETWORK_INCLUDE_DIRS})
target_compile_definitions(KiwiNetwork PUBLIC "${KIWI_NETWORK_COMPILE_DEFINITIONS}")
target_link_libraries(KiwiNetwork PUBLIC ${KIWI_NETWORK_LINK_LIBRARIES})

set_target_properties(KiwiNetwork PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_NETWORK_SOURCES}" ${PROJECT_SOURCE_DIR}/Modules/KiwiNetwork)

# Kiwi Engine
#------------------------------------------------------------------------------#
set(KIWI_ENGINE_DEPENDENCIES KiwiModel KiwiDsp Juce staticlib)
set(KIWI_ENGINE_INCLUDE_DIRS ${KIWI_MODULE_INCLUDE_DIRS} ${KIWI_MODEL_INCLUDE_DIRS} ${KIWI_DSP_INCLUDE_DIRS} ${JUCE_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
set(KIWI_ENGINE_COMPILE_DEFINITIONS ${KIWI_MODEL_COMPILE_DEFINITIONS} ${KIWI_DSP_COMPILE_DEFINITIONS} ${JUCE_COMPILE_DEFINITIONS})
set(KIWI_ENGINE_LINK_LIBRARIES ${KIWI_MODEL_LINK_LIBRARIES} KiwiModel KiwiDsp ${JUCE_LINK_LIBRARIES} Juce staticlib)


file(GLOB_RECURSE KIWI_ENGINE_SOURCES ${KIWI_MODULE_INCLUDE_DIRS}/KiwiEngine/*)
add_library(KiwiEngine STATIC ${KIWI_ENGINE_SOURCES})
add_dependencies(KiwiEngine ${KIWI_ENGINE_DEPENDENCIES})
target_include_directories(KiwiEngine PUBLIC ${KIWI_ENGINE_INCLUDE_DIRS})
set_target_properties(KiwiEngine PROPERTIES COMPILE_DEFINITIONS "${KIWI_ENGINE_COMPILE_DEFINITIONS}")
target_link_libraries(KiwiEngine PUBLIC ${KIWI_ENGINE_LINK_LIBRARIES})

set_target_properties(KiwiEngine PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_ENGINE_SOURCES}" ${PROJECT_SOURCE_DIR}/Modules/KiwiEngine)

#----------------------------------
# Server
#----------------------------------
set(KIWI_SERVER_DEPENDENCIES KiwiModel Juce)
set(KIWI_SERVER_INCLUDE_DIRS ${KIWI_MODULE_INCLUDE_DIRS} ${KIWI_MODEL_INCLUDE_DIRS} ${JUCE_INCLUDE_DIRS} ${JSON_INCLUDE_DIRS} )
set(KIWI_SERVER_COMPILE_DEFINITIONS ${KIWI_MODEL_COMPILE_DEFINITIONS} ${KIWI_MODEL_COMPILE_DEFINITIONS} ${JUCE_COMPILE_DEFINITIONS})
set(KIWI_SERVER_LINK_LIBRARIES ${KIWI_MODEL_LINK_LIBRARIES} KiwiModel ${JUCE_LINK_LIBRARIES} Juce)

file(GLOB_RECURSE KIWI_SERVER_SOURCES ${KIWI_MODULE_INCLUDE_DIRS}/KiwiServer/*.[c|h]pp ${KIWI_MODULE_INCLUDE_DIRS}/KiwiServer/*.h)
add_library(KiwiServer ${KIWI_SERVER_SOURCES})
add_dependencies(KiwiServer ${KIWI_SERVER_DEPENDENCIES})
target_include_directories(KiwiServer PUBLIC ${KIWI_SERVER_INCLUDE_DIRS})
set_target_properties(KiwiServer PROPERTIES COMPILE_DEFINITIONS "${KIWI_SERVER_COMPILE_DEFINITIONS}")
target_link_libraries(KiwiServer PUBLIC ${KIWI_SERVER_LINK_LIBRARIES})

set_target_properties(KiwiServer PROPERTIES FOLDER Modules)
source_group_rec("${KIWI_SERVER_SOURCES}" ${PROJECT_SOURCE_DIR}/Modules/KiwiServer)

# Kiwi Server application
#------------------------------------------------------------------------------#
file(GLOB_RECURSE KIWI_SERVER_APP_SOURCES ${PROJECT_SOURCE_DIR}/Server/Source/*.[c|h]pp ${PROJECT_SOURCE_DIR}/Server/Source/*.h)
add_executable(Server ${KIWI_SERVER_SOURCES} ${KIWI_SERVER_APP_SOURCES})
add_dependencies(Server KiwiServer)
target_compile_definitions(Server PUBLIC ${KIWI_SERVER_COMPILE_DEFINITIONS})
target_link_libraries(Server PUBLIC KiwiServer)

# Kiwi Client application
#------------------------------------------------------------------------------#

set(KIWI_RESSOURCES_DIR ${PROJECT_SOURCE_DIR}/Ressources)

if (APPLE)
	set(KIWI_APP_ICON ${KIWI_RESSOURCES_DIR}/Project/XCode/Icon.icns)
	set(KIWI_PATCHER_FILETYPE_ICON ${KIWI_RESSOURCES_DIR}/Project/XCode/kiwi_filetype.icns)
	set(KIWI_PLIST ${KIWI_RESSOURCES_DIR}/Project/XCode/Info.plist)
	set_source_files_properties(${KIWI_APP_ICON} ${KIWI_PATCHER_FILETYPE_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set(KIWI_CLIENT_RESSOURCES ${KIWI_APP_ICON} ${KIWI_PATCHER_FILETYPE_ICON} ${KIWI_PLIST})

elseif(WIN32)
	set(KIWI_CLIENT_RESSOURCES ${KIWI_RESSOURCES_DIR}/Project/VisualStudio15/Kiwi.rc)
endif()

set(KIWI_CLIENT_DEPENDENCIES KiwiNetwork KiwiEngine)
set(KIWI_CLIENT_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/Client/Source ${KIWI_MODULE_INCLUDE_DIRS} ${KIWI_NETWORK_INCLUDE_DIRS} ${KIWI_ENGINE_INCLUDE_DIRS} ${JSON_INCLUDE_DIRS} )
set(KIWI_CLIENT_COMPILE_DEFINITIONS ${KIWI_ENGINE_COMPILE_DEFINITIONS} ${KIWI_ENGINE_COMPILE_DEFINITIONS} ${KIWI_NETWORK_COMPILE_DEFINITIONS})
set(KIWI_CLIENT_LINK_LIBRARIES KiwiEngine KiwiNetwork)

file(GLOB_RECURSE KIWI_CLIENT_SOURCES ${PROJECT_SOURCE_DIR}/Client/Source/*.[c|h]pp ${PROJECT_SOURCE_DIR}/Client/Source/*.h)
add_executable(Kiwi ${KIWI_CLIENT_SOURCES} "${KIWI_CLIENT_RESSOURCES}")
add_dependencies(Kiwi ${KIWI_CLIENT_DEPENDENCIES})
target_include_directories(Kiwi PUBLIC ${KIWI_CLIENT_INCLUDE_DIRS})
target_compile_definitions(Kiwi PUBLIC ${KIWI_CLIENT_COMPILE_DEFINITIONS})
target_link_libraries(Kiwi PUBLIC ${KIWI_CLIENT_LINK_LIBRARIES})

if (APPLE)
	set_target_properties(Kiwi PROPERTIES MACOSX_BUNDLE TRUE MACOSX_BUNDLE_INFO_PLIST ${KIWI_PLIST})

	# Copy ressources to bundle
	set(KIWI_APP_BUNDLE_RESSOURCES $<TARGET_BUNDLE_CONTENT_DIR:Kiwi>/Resources)

	set(KIWI_HELPS_RESOURCE ${PROJECT_SOURCE_DIR}/docs/helps)
	set(KIWI_EXAMPLES_RESOURCE ${PROJECT_SOURCE_DIR}/docs/examples)
    set(FAUST_LIB_RESOURCE ${KIWI_DEPENDENCIES_DIR}/faust/libraries)

	add_custom_command(TARGET Kiwi POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
		${KIWI_HELPS_RESOURCE} "${KIWI_APP_BUNDLE_RESSOURCES}/helps"
	)

	add_custom_command(TARGET Kiwi POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
		${KIWI_EXAMPLES_RESOURCE} "${KIWI_APP_BUNDLE_RESSOURCES}/examples"
	)

	add_custom_command(TARGET Kiwi POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
		${FAUST_LIB_RESOURCE} "${KIWI_APP_BUNDLE_RESSOURCES}/libs/faust"
	)

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

source_group_rec("${KIWI_CLIENT_SOURCES}" ${PROJECT_SOURCE_DIR}/Client/Source)

# Tests
#------------------------------------------------------------------------------#
# Coverage setting
if(${GCOV_SUPPORT} STREQUAL "On")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
	set(KIWI_TESTS_LINK_LIBRARIES gcov)
endif()

if(UNIX AND NOT APPLE)
	set(KIWI_TESTS_LINK_LIBRARIES ${KIWI_TESTS_LINK_LIBRARIES} ${PTHREAD})
endif()

# Test Tool
#------------------------------------------------------------------------------#
file(GLOB TEST_TOOL_SOURCES ${PROJECT_SOURCE_DIR}/Test/Tool/*.[c|h]pp ${PROJECT_SOURCE_DIR}/Test/Tool/*.h)
add_executable(test_tool ${TEST_TOOL_SOURCES})
add_dependencies(test_tool KiwiTool)
set_target_properties(test_tool PROPERTIES FOLDER Test)
target_include_directories(test_tool PUBLIC ${KIWI_TOOL_INCLUDE_DIRS})
target_link_libraries(test_tool KiwiTool ${KIWI_TESTS_LINK_LIBRARIES})
source_group_rec("${TEST_TOOL_SOURCES}" ${PROJECT_SOURCE_DIR}/Test/Tool)

# Test Dsp
#------------------------------------------------------------------------------#
file(GLOB TEST_DSP_SOURCES ${PROJECT_SOURCE_DIR}/Test/Dsp/*.[c|h]pp ${PROJECT_SOURCE_DIR}/Test/Dsp/*.h)
add_executable(test_dsp ${TEST_DSP_SOURCES})
add_dependencies(test_dsp KiwiDsp)
set_target_properties(test_dsp PROPERTIES FOLDER Test)
target_include_directories(test_dsp PUBLIC ${KIWI_DSP_INCLUDE_DIRS})
target_link_libraries(test_dsp KiwiDsp ${KIWI_TESTS_LINK_LIBRARIES})
source_group_rec("${TEST_DSP_SOURCES}" ${PROJECT_SOURCE_DIR}/Test/Dsp)

# Test Model
#------------------------------------------------------------------------------#
file(GLOB TEST_MODEL_SOURCES ${PROJECT_SOURCE_DIR}/Test/Model/*.[c|h]pp ${PROJECT_SOURCE_DIR}/Test/Model/*.h)
add_executable(test_model ${TEST_MODEL_SOURCES})
add_dependencies(test_model KiwiModel)
set_target_properties(test_model PROPERTIES FOLDER Test)
target_include_directories(test_model PUBLIC ${KIWI_MODEL_INCLUDE_DIRS})
target_link_libraries(test_model KiwiModel ${KIWI_TESTS_LINK_LIBRARIES})
source_group_rec("${TEST_MODEL_SOURCES}" ${PROJECT_SOURCE_DIR}/Test/Model)

# Test Network
#------------------------------------------------------------------------------#
file(GLOB TEST_NETWORK_SOURCES ${PROJECT_SOURCE_DIR}/Test/Network/*.[c|h]pp ${PROJECT_SOURCE_DIR}/Test/Network/*.h)
add_executable(test_network ${TEST_NETWORK_SOURCES})
add_dependencies(test_network KiwiNetwork)
set_target_properties(test_network PROPERTIES FOLDER Test)
target_include_directories(test_network PUBLIC ${KIWI_NETWORK_INCLUDE_DIRS})
target_link_libraries(test_network KiwiNetwork ${KIWI_TESTS_LINK_LIBRARIES})
source_group_rec("${TEST_NETWORK_SOURCES}" ${PROJECT_SOURCE_DIR}/Test/Network)

# Test Server
#------------------------------------------------------------------------------#
file(GLOB TEST_SERVER_SOURCES ${PROJECT_SOURCE_DIR}/Test/Server/*.[c|h]pp ${PROJECT_SOURCE_DIR}/Test/Server/*.h)
add_executable(test_server ${TEST_SERVER_SOURCES})
add_dependencies(test_server KiwiServer)
set_target_properties(test_server PROPERTIES FOLDER Test)
set_target_properties(test_server PROPERTIES COMPILE_DEFINITIONS "${KIWI_SERVER_COMPILE_DEFINITIONS}")
target_include_directories(test_server PUBLIC ${KIWI_SERVER_INCLUDE_DIRS})
target_link_libraries(test_server PUBLIC KiwiServer ${KIWI_TESTS_LINK_LIBRARIES})
source_group_rec("${TEST_SERVER_SOURCES}" ${PROJECT_SOURCE_DIR}/Test/Server)

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
