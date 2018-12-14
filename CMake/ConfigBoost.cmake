
message(STATUS "Boost - searching the static Boost library 1.69.0 with system")
option(USE_SYSTEM_BOOST  "Use the pre-compiled Boost library of your system" OFF)
# -----------------------------------------------------------------------------#

# Start the script
# -----------------------------------------------------------------------------#
set(Boost_USE_STATIC_LIBS ON) # Use the static library

# Use the boost internal library
# -----------------------------------------------------------------------------#
if(NOT ${USE_SYSTEM_BOOST})

	# Local OS Specific Variables
	# -----------------------------------------------------------------------------#
	set(BOOST_URL "https://github.com/Musicoll/KiwiDependenciesPrebuilt/releases/download/v1.3")
	set(BOOST_PKG_NAME "boost_1_69_0")

	# Boost internal directory
	# -----------------------------------------------------------------------------#
	if(NOT DEFINED KIWI_DEPENDENCIES_DIR)
		set(KIWI_DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/ThirdParty")
	endif()

	set(KIWI_BOOST_INTERN_DIR "${KIWI_DEPENDENCIES_DIR}/boost")
	set(BOOST_INTERN_INCLUDE_DIR "${KIWI_BOOST_INTERN_DIR}")

	# Local Generic Variables
	# -----------------------------------------------------------------------------#
	if(APPLE)
		set(BOOST_PKG_EXT "tar.gz")
		set(BOOST_INTERN_LIBRARIES_DIR "${KIWI_BOOST_INTERN_DIR}/stage/lib")
	elseif(UNIX)
		set(BOOST_PKG_EXT "tar.gz")
		set(BOOST_INTERN_LIBRARIES_DIR "${KIWI_BOOST_INTERN_DIR}/stage/lib")
	elseif(WIN32)
		set(BOOST_PKG_EXT "zip")
		set(BOOST_INTERN_LIBRARIES_DIR "${KIWI_BOOST_INTERN_DIR}/stage64/lib")
	endif()

	set(BOOST_PKG_FILE "${BOOST_PKG_NAME}.${BOOST_PKG_EXT}")
	set(BOOST_PKG_PATH "${BOOST_URL}/${BOOST_PKG_FILE}")

	# ---------------------------------------------------------------------------#
	set(Boost_INCLUDE_DIR "${BOOST_INTERN_INCLUDE_DIR}")
	set(Boost_LIBRARY_DIR "${BOOST_INTERN_LIBRARIES_DIR}")

	message(STATUS "Boost_INCLUDE_DIR ${Boost_INCLUDE_DIR}")
	message(STATUS "Boost_LIBRARY_DIR ${Boost_LIBRARY_DIR}")

	# Set up for the boost internal library
	# ---------------------------------------------------------------------------#
	if(NOT EXISTS ${KIWI_BOOST_INTERN_DIR}/bootstrap.log)

		# Remove the current invalid boost folder
		# -------------------------------------------------------------------------#
		if(EXISTS ${KIWI_BOOST_INTERN_DIR})
			message(STATUS "Remove current invalid boost folder ${KIWI_BOOST_INTERN_DIR}")
			file(REMOVE_RECURSE ${KIWI_BOOST_INTERN_DIR})
		endif()

		# Download the boost package
		# -------------------------------------------------------------------------#
		if(NOT EXISTS "${KIWI_DEPENDENCIES_DIR}/${BOOST_PKG_FILE}")
			message(STATUS "Download ${BOOST_PKG_FILE}")
			file(DOWNLOAD ${BOOST_PKG_PATH} "${KIWI_DEPENDENCIES_DIR}/${BOOST_PKG_FILE}")
		endif()

		# Unpack the boost package
		# -------------------------------------------------------------------------#
		message(STATUS "Unpack ${BOOST_PKG_FILE} to ${KIWI_BOOST_INTERN_DIR}")
		if(APPLE)
			execute_process(COMMAND tar xzf ./${BOOST_PKG_FILE} WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
		elseif(UNIX)
			execute_process(COMMAND tar xzf ./${BOOST_PKG_FILE} WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
		elseif(WIN32)
			execute_process(COMMAND 7z x ${BOOST_PKG_FILE} WORKING_DIRECTORY ${KIWI_DEPENDENCIES_DIR})
		endif()

		file(RENAME ${KIWI_DEPENDENCIES_DIR}/${BOOST_PKG_NAME} ${KIWI_BOOST_INTERN_DIR})

		# Configure the boost library
		# -------------------------------------------------------------------------#
		message(STATUS "Configure the boost library")
		if(APPLE)
			execute_process(COMMAND ./bootstrap.sh toolset=clang link=static WORKING_DIRECTORY ${KIWI_BOOST_INTERN_DIR})
		elseif(UNIX)
			execute_process(COMMAND ./bootstrap.sh toolset=gcc link=static WORKING_DIRECTORY ${KIWI_BOOST_INTERN_DIR})
		elseif(WIN32)
			execute_process(COMMAND bootstrap.bat WORKING_DIRECTORY ${KIWI_BOOST_INTERN_DIR})
		endif()

		# Compile the boost library
		# -------------------------------------------------------------------------#
		message(STATUS "Compile the boost library")
		if(APPLE)
			execute_process(COMMAND ./b2 cflags=-mmacosx-version-min=10.9 address-model=64 --with-system stage WORKING_DIRECTORY ${KIWI_BOOST_INTERN_DIR})
		elseif(UNIX)
			execute_process(COMMAND ./b2 --with-system stage WORKING_DIRECTORY ${KIWI_BOOST_INTERN_DIR})
		elseif(WIN32)
			execute_process(COMMAND b2 --toolset=msvc-14.0 -j4 --with-system --stagedir=stage64 variant=release architecture=x86 address-model=64 link=static WORKING_DIRECTORY ${KIWI_BOOST_INTERN_DIR})
		endif()

	endif()

endif(NOT ${USE_SYSTEM_BOOST})

# Find boost library on the system
# ---------------------------------------------------------------------------#
find_package(Boost 1.69.0 COMPONENTS system)

if(Boost_FOUND)
	message(STATUS "Boost include directory: " ${Boost_INCLUDE_DIRS})
	message(STATUS "Boost library directory: " ${Boost_LIBRARY_DIRS})
	message(STATUS "Boost libraries:" ${Boost_LIBRARIES})
else()
	message(SEND_ERROR "Boost not installed")
endif()
message(STATUS "")
