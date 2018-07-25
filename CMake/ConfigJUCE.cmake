
message(STATUS "JUCE - configuring the JUCE library")

# -----------------------------------------------------------------------------#

# JUCE internal directory
# -----------------------------------------------------------------------------#
if(NOT DEFINED KIWI_DEPENDENCIES_DIR)
	set(KIWI_DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/ThirdParty")
endif()
set(KIWI_JUCE_INTERN_DIR "${KIWI_DEPENDENCIES_DIR}/Juce")


set(JUCE_INCLUDE_DIRS ${KIWI_JUCE_INTERN_DIR}/modules)
set(JUCE_COMPILE_DEFINITIONS JUCE_STANDALONE_APPLICATION=1)
set(JUCE_COMPILE_DEFINITIONS ${JUCE_COMPILE_DEFINITIONS} JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1)
set(JUCE_COMPILE_DEFINITIONS ${JUCE_COMPILE_DEFINITIONS} JUCE_DISPLAY_SPLASH_SCREEN=0)
set(JUCE_COMPILE_DEFINITIONS ${JUCE_COMPILE_DEFINITIONS} JUCE_REPORT_APP_USAGE=0)

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
    set(JUCE_LINK_LIBRARIES ${COCOA} ${IOKIT} ${QUARTZCORE} ${CARBON} ${WEBKIT} ${COREAUDIO} ${COREMIDI} ${ACCELERATE} ${DISCRECORDING} ${AUDIOTOOLBOX})
elseif(WIN32)
	find_library(WS2_32 ws2_32)
	#find_library(UUID)
	find_library(WSOCK32 wsock32)
	find_library(WININET wininet)
	find_library(LIBVERSION version)
	find_library(OLE32 ole32)
	find_library(OLEAUT32 oleaut32)
	find_library(IMM32 imm32)
	find_library(COMDLG32 comdlg32)
	find_library(SHLWAPI shlwapi)
	find_library(RPCRT4 rpcrt4)
	find_library(WINMM winmm)
	set(JUCE_LINK_LIBRARIES)# ${WS2_32} ${UUID} ${WSOCK32} ${WININET} ${LIBVERSION} ${OLE32} ${OLEAUT32} ${COMDLG32} ${SHLWAPI} ${RPCRT4} ${WINMM})

	set(JUCE_COMPILE_DEFINITIONS ${JUCE_COMPILE_DEFINITIONS} CPPUNWIND=1 WIN32_WINNT=0x0501 USE_MATH_DEFINES)
	set(JUCE_COMPILE_DEFINITIONS_DEBUG ${JUCE_COMPILE_DEFINITIONS_DEBUG} CONSOLE=1)

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
    set(JUCE_LINK_LIBRARIES ${RT} ${DL} ${PTHREAD} ${FREETYPE} ${X11} ${XEXT} ${ASOUND} ${GTK3_X11_LIBRARIES} ${WEBKIT_TO_GTK4_LIBRARIES})
endif()

file (GLOB_RECURSE JUCE_SOURCES ${KIWI_JUCE_INTERN_DIR}/modules/*.[h|c]pp ${KIWI_JUCE_INTERN_DIR}/modules/*.h ${KIWI_JUCE_INTERN_DIR}/modules/*.mm)

set_source_files_properties(${JUCE_SOURCES} PROPERTIES HEADER_FILE_ONLY TRUE)

set(JUCE_COMPILED_SOURCES ${KIWI_JUCE_INTERN_DIR}/modules/juce_core/juce_core.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_data_structures/juce_data_structures.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_events/juce_events.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_graphics/juce_graphics.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_gui_basics/juce_gui_basics.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_gui_extra/juce_gui_extra.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_audio_basics/juce_audio_basics.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_audio_devices/juce_audio_devices.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_audio_formats/juce_audio_formats.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_audio_utils/juce_audio_utils.cpp
                      		${KIWI_JUCE_INTERN_DIR}/modules/juce_audio_processors/juce_audio_processors.cpp)

if (APPLE)
    set_source_files_properties(${JUCE_COMPILED_SOURCES} PROPERTIES COMPILE_FLAGS "-x objective-c++" HEADER_FILE_ONLY FALSE)
else()
    set_source_files_properties(${JUCE_COMPILED_SOURCES} PROPERTIES HEADER_FILE_ONLY FALSE)
endif()

add_library(Juce STATIC ${JUCE_SOURCES} ${JUCE_COMPILED_SOURCES})
target_link_libraries(Juce ${JUCE_LINK_LIBRARIES})
set_target_properties(Juce PROPERTIES COMPILE_DEFINITIONS "${JUCE_COMPILE_DEFINITIONS}")

target_include_directories(Juce PUBLIC ${JUCE_INCLUDE_DIRS})
set_target_properties(Juce PROPERTIES FOLDER Juce)
source_group_rec("${JUCE_SOURCES} ${JUCE_COMPILED_SOURCES}" ${KIWI_JUCE_INTERN_DIR}/modules)

message(STATUS "JUCE include directory: " ${JUCE_INCLUDE_DIRS})

message(STATUS "")
