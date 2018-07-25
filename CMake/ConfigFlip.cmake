
message(STATUS "Flip - searching the Flip static library ")
option(USE_SYSTEM_FLIP  "Use the pre-compiled Flip library of your system" OFF)
# -----------------------------------------------------------------------------#

# FLIP internal directory
# -----------------------------------------------------------------------------#
if(NOT DEFINED KIWI_DEPENDENCIES_DIR)
	set(KIWI_DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/ThirdParty")
endif()
set(KIWI_FLIP_INTERN_DIR "${KIWI_DEPENDENCIES_DIR}/flip")

# Local OS Specific Variables
# -----------------------------------------------------------------------------#
set(FLIP_URL_APPLE "https://github.com/Musicoll/KiwiDependenciesPrebuilt/releases/download/v1.3")
set(FLIP_URL_LINUX "https://github.com/Musicoll/KiwiDependenciesPrebuilt/releases/download/v1.3")
set(FLIP_URL_WIN32 "https://github.com/Musicoll/KiwiDependenciesPrebuilt/releases/download/v1.3")

set(FLIP_PKG_NAME_APPLE "flip-demo-macos-c47e41da05")
set(FLIP_PKG_NAME_LINUX "flip-demo-linux-c47e41da05")
set(FLIP_PKG_NAME_WIN32 "flip-demo-windows-c47e41da05")

set(FLIP_PKG_EXT_APPLE "tar.gz")
set(FLIP_PKG_EXT_LINUX "tar.gz")
set(FLIP_PKG_EXT_WIN32 "zip")

set(FLIP_PKG_FOLDER_NAME_APPLE "flip-demo")
set(FLIP_PKG_FOLDER_NAME_LINUX "flip-demo")
set(FLIP_PKG_FOLDER_NAME_WIN32 "flip-demo")

set(FLIP_INTERN_INCLUDE_DIR_APPLE "${KIWI_FLIP_INTERN_DIR}/include")
set(FLIP_INTERN_INCLUDE_DIR_LINUX "${KIWI_FLIP_INTERN_DIR}/include")
set(FLIP_INTERN_INCLUDE_DIR_WIN32 "${KIWI_FLIP_INTERN_DIR}/include")

set(FLIP_INTERN_LIBRARIES_DIR_APPLE "${KIWI_FLIP_INTERN_DIR}/lib")
set(FLIP_INTERN_LIBRARIES_DIR_LINUX "${KIWI_FLIP_INTERN_DIR}/lib/gcc")
set(FLIP_INTERN_LIBRARIES_DIR_WIN32 "${KIWI_FLIP_INTERN_DIR}/lib/VS2015/x64/Release")

# Local Generic Variables
# -----------------------------------------------------------------------------#
if(APPLE)
	set(FLIP_URL ${FLIP_URL_APPLE})
	set(FLIP_PKG_NAME ${FLIP_PKG_NAME_APPLE})
	set(FLIP_PKG_EXT ${FLIP_PKG_EXT_APPLE})
	set(FLIP_PKG_FOLDER_NAME ${FLIP_PKG_FOLDER_NAME_APPLE})
	set(FLIP_INTERN_INCLUDE_DIR ${FLIP_INTERN_INCLUDE_DIR_APPLE})
	set(FLIP_INTERN_LIBRARIES_DIR ${FLIP_INTERN_LIBRARIES_DIR_APPLE})
elseif(UNIX)
	set(FLIP_URL ${FLIP_URL_LINUX})
	set(FLIP_PKG_NAME ${FLIP_PKG_NAME_LINUX})
	set(FLIP_PKG_EXT ${FLIP_PKG_EXT_LINUX})
	set(FLIP_PKG_FOLDER_NAME ${FLIP_PKG_FOLDER_NAME_LINUX})
	set(FLIP_INTERN_INCLUDE_DIR ${FLIP_INTERN_INCLUDE_DIR_LINUX})
	set(FLIP_INTERN_LIBRARIES_DIR ${FLIP_INTERN_LIBRARIES_DIR_LINUX})
elseif(WIN32)
	set(FLIP_URL ${FLIP_URL_WIN32})
	set(FLIP_PKG_NAME ${FLIP_PKG_NAME_WIN32})
	set(FLIP_PKG_EXT ${FLIP_PKG_EXT_WIN32})
	set(FLIP_PKG_FOLDER_NAME ${FLIP_PKG_FOLDER_NAME_WIN32})
	set(FLIP_INTERN_INCLUDE_DIR ${FLIP_INTERN_INCLUDE_DIR_WIN32})
	set(FLIP_INTERN_LIBRARIES_DIR ${FLIP_INTERN_LIBRARIES_DIR_WIN32})
endif()

set(FLIP_PKG_FILE "${FLIP_PKG_NAME}.${FLIP_PKG_EXT}")
set(FLIP_PKG_PATH "${FLIP_URL}/${FLIP_PKG_FILE}")

# Option Variables
# -----------------------------------------------------------------------------#
if(NOT DEFINED FLIP_INCLUDE_DIRS)
	set(FLIP_INCLUDE_DIRS ${FLIP_INTERN_INCLUDE_DIR} CACHE STRING "Flip include directories")
endif()
if(NOT DEFINED FLIP_LIBRARY_DIRS)
	set(FLIP_LIBRARY_DIRS ${FLIP_INTERN_LIBRARIES_DIR} CACHE STRING "Flip library directories")
endif()

# Use the FLIP internal library
# -----------------------------------------------------------------------------#
if(NOT ${USE_SYSTEM_FLIP})

	# Search for the FLIP internal library
	# ---------------------------------------------------------------------------#
	find_library(FLIP_LIBRARIES NAMES flip PATHS ${FLIP_LIBRARY_DIRS} NO_DEFAULT_PATH)

	# Set up the FLIP internal library
	# ---------------------------------------------------------------------------#
	if(${FLIP_LIBRARIES} STREQUAL "FLIP_LIBRARIES-NOTFOUND")
		set(FLIP_INCLUDE_DIRS ${FLIP_INTERN_INCLUDE_DIR})
		set(FLIP_LIBRARY_DIRS ${FLIP_INTERN_LIBRARIES_DIR})

		# Remove the current invalid FLIP folder
		# -------------------------------------------------------------------------#
		if(EXISTS ${KIWI_FLIP_INTERN_DIR})
			message(STATUS "Remove current invalid flip folder ${KIWI_FLIP_INTERN_DIR}")
			file(REMOVE_RECURSE ${KIWI_FLIP_INTERN_DIR})
		endif()

		# Download the FLIP package
		# -------------------------------------------------------------------------#
		if(NOT (EXISTS "${KIWI_DEPENDENCIES_DIR}/${FLIP_PKG_FILE}"))
			message(STATUS "Download ${FLIP_PKG_FILE}")
			file(DOWNLOAD ${FLIP_PKG_PATH} "${KIWI_DEPENDENCIES_DIR}/${FLIP_PKG_FILE}")
		endif()

		# Unpack the FLIP package
		# -------------------------------------------------------------------------#
		message(STATUS "Unpack ${FLIP_PKG_FILE} to ${KIWI_FLIP_INTERN_DIR}")
		if(APPLE)
	    execute_process(COMMAND tar xzf ./${FLIP_PKG_FILE} WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
	  elseif(UNIX)
	    execute_process(COMMAND tar xzf ./${FLIP_PKG_FILE} WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
	  elseif(WIN32)
	    execute_process(COMMAND 7z x ${FLIP_PKG_FILE} WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
	  endif()
		file(RENAME ${KIWI_DEPENDENCIES_DIR}/${FLIP_PKG_FOLDER_NAME} ${KIWI_FLIP_INTERN_DIR})

		# Find the Flip internal library now it is installed
		# ---------------------------------------------------------------------------#
		find_library(FLIP_LIBRARIES NAMES flip PATHS ${FLIP_LIBRARY_DIRS} NO_DEFAULT_PATH)
	endif()

else()

	# Search for the system FLIP library
	# ---------------------------------------------------------------------------#
	find_library(FLIP_LIBRARIES NAMES flip PATHS ${FLIP_LIBRARY_DIRS})

endif(NOT ${USE_SYSTEM_FLIP})

# -----------------------------------------------------------------------------#

if(FLIP_LIBRARIES)
	set(FLIP_COMPILE_DEFINITIONS "flip_NOTHING=flip_NOTHING_FATAL")
	if(WIN32)
	    set(FLIP_LIBRARIES ${FLIP_LIBRARIES} ${WS2_32})
	endif()
	message(STATUS "Flip include directory: " ${FLIP_INCLUDE_DIRS})
	message(STATUS "Flip library directory: " ${FLIP_LIBRARY_DIRS})
	message(STATUS "Flip libraries:" ${FLIP_LIBRARIES})
else()
  message(SEND_ERROR "Flip not installed")
endif()

message(STATUS "")
