
message(STATUS "LLVM - searching the static LLVM library 5.0.0 (or superior)")
option(USE_SYSTEM_LLVM  "Use the pre-compiled LLVM library of your system" OFF)
set(LLVM_DIR      			CACHE STRING "LLVM CMake module directory")
set(USE_LLVM_CONFIG 		CACHE STRING "Use system LLVM configuration")

# -----------------------------------------------------------------------------#

# LLVM internal directory
# -----------------------------------------------------------------------------#
if(NOT DEFINED KIWI_DEPENDENCIES_DIR)
	set(KIWI_DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/ThirdParty")
endif()
set(KIWI_LLVM_INTERN_DIR "${KIWI_DEPENDENCIES_DIR}/llvm")

# Local OS Specific Variables
# -----------------------------------------------------------------------------#
set(LLVM_URL_APPLE "https://github.com/Musicoll/KiwiDependenciesPrebuilt/releases/download/v1.3")
set(LLVM_URL_LINUX "http://releases.llvm.org/5.0.0")
set(LLVM_URL_WIN32 "https://github.com/Musicoll/KiwiDependenciesPrebuilt/releases/download/v1.3")

set(LLVM_PKG_NAME_APPLE "llvm-macos-x64")
set(LLVM_PKG_NAME_LINUX "clang+llvm-5.0.0-linux-x86_64-ubuntu14.04")
set(LLVM_PKG_NAME_WIN32 "llvm-windows-x64-md")

set(LLVM_PKG_EXT_APPLE "zip")
set(LLVM_PKG_EXT_LINUX "tar.xz")
set(LLVM_PKG_EXT_WIN32 "zip")

set(LLVM_INTERN_CMAKE_DIR_APPLE "${KIWI_LLVM_INTERN_DIR}/lib/cmake/llvm")
set(LLVM_INTERN_CMAKE_DIR_LINUX "${KIWI_LLVM_INTERN_DIR}/lib/cmake/llvm")
set(LLVM_INTERN_CMAKE_DIR_WIN32 "${KIWI_LLVM_INTERN_DIR}/lib/cmake/llvm")

# Local Generic Variables
# -----------------------------------------------------------------------------#
if(APPLE)
	set(LLVM_URL ${LLVM_URL_APPLE})
	set(LLVM_PKG_NAME ${LLVM_PKG_NAME_APPLE})
	set(LLVM_PKG_EXT ${LLVM_PKG_EXT_APPLE})
	set(LLVM_INTERN_CMAKE_DIR ${LLVM_INTERN_CMAKE_DIR_APPLE})
elseif(UNIX)
	set(LLVM_URL ${LLVM_URL_LINUX})
	set(LLVM_PKG_NAME ${LLVM_PKG_NAME_LINUX})
	set(LLVM_PKG_EXT ${LLVM_PKG_EXT_LINUX})
	set(LLVM_INTERN_CMAKE_DIR ${LLVM_INTERN_CMAKE_DIR_LINUX})
elseif(WIN32)
	set(LLVM_URL ${LLVM_URL_WIN32})
	set(LLVM_PKG_NAME ${LLVM_PKG_NAME_WIN32})
	set(LLVM_PKG_EXT ${LLVM_PKG_EXT_WIN32})
	set(LLVM_INTERN_CMAKE_DIR ${LLVM_INTERN_CMAKE_DIR_WIN32})
endif()

set(LLVM_PKG_FILE "${LLVM_PKG_NAME}.${LLVM_PKG_EXT}")
set(LLVM_PKG_PATH "${LLVM_URL}/${LLVM_PKG_FILE}")

# Use the LLVM internal library
# -----------------------------------------------------------------------------#
if(NOT ${USE_SYSTEM_LLVM})

	# Search for the LLVM internal library
	# ---------------------------------------------------------------------------#
	set(LLVM_INTERNAL_VALID OFF)
	set(LLVM_DIR ${LLVM_INTERN_CMAKE_DIR})
	set(USE_LLVM_CONFIG OFF)
	find_package(LLVM CONFIG)
	if(${LLVM_FOUND})
		if(NOT ${LLVM_PACKAGE_VERSION} VERSION_LESS "5.0")
			set(LLVM_INTERNAL_VALID ON)
		endif()
	endif()

	# Set up for the LLVM internal library
	# ---------------------------------------------------------------------------#
	if(NOT ${LLVM_INTERNAL_VALID})

		# Remove the current invalid LLVM folder
		# -------------------------------------------------------------------------#
		if(EXISTS ${KIWI_LLVM_INTERN_DIR})
			message(STATUS "Remove current invalid llvm folder ${KIWI_LLVM_INTERN_DIR}")
			file(REMOVE_RECURSE ${KIWI_LLVM_INTERN_DIR})
		endif()

		# Download the LLVM package
		# -------------------------------------------------------------------------#
		if(NOT EXISTS "${KIWI_DEPENDENCIES_DIR}/${LLVM_PKG_FILE}")
			message(STATUS "Download ${LLVM_PKG_FILE}")
			file(DOWNLOAD ${LLVM_PKG_PATH} "${KIWI_DEPENDENCIES_DIR}/${LLVM_PKG_FILE}")
		endif()

		# Unpack the LLVM package
		# -------------------------------------------------------------------------#
		message(STATUS "Unpack ${LLVM_PKG_FILE} to ${KIWI_LLVM_INTERN_DIR}")
		if(APPLE)
	    execute_process(COMMAND tar xzf ./${LLVM_PKG_FILE}  WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
	  elseif(UNIX)
	    execute_process(COMMAND tar xzf ./${LLVM_PKG_FILE}  WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
	  elseif(WIN32)
	    execute_process(COMMAND 7z x ${LLVM_PKG_FILE}  WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
	  endif()
		file(RENAME ${KIWI_DEPENDENCIES_DIR}/${LLVM_PKG_NAME} ${KIWI_LLVM_INTERN_DIR})

		# Find the LLVM internal library now it is installed
		# -------------------------------------------------------------------------#
		set(LLVM_DIR ${LLVM_INTERN_CMAKE_DIR})
		set(USE_LLVM_CONFIG OFF)
		find_package(LLVM CONFIG)

	endif(NOT ${LLVM_INTERNAL_VALID})

else()

	# Find the LLVM system library
	# ---------------------------------------------------------------------------#
	find_package(LLVM CONFIG)

endif(NOT ${USE_SYSTEM_LLVM})

if(LLVM_FOUND)
	llvm_map_components_to_libnames(LLVM_ALL_LIBRARIES all)
	list(REMOVE_ITEM LLVM_ALL_LIBRARIES LTO)

	message(STATUS "LLVM " ${LLVM_PACKAGE_VERSION})
	message(STATUS "LLVM include directory: " ${LLVM_INCLUDE_DIRS})
	message(STATUS "LLVM library directory: " ${LLVM_LIBRARY_DIRS})
	message(STATUS "LLVM libraries:" True)
else()
	message(SEND_ERROR "LLVM not installed")
endif()

message(STATUS "")
