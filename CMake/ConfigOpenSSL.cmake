message(STATUS "OpenSSL - configuring OpenSSL static library")

# Start the script
# -----------------------------------------------------------------------------#

set(KIWI_OPENSSL_INTERN_DIR "${PROJECT_SOURCE_DIR}/ThirdParty/openssl")

# Set up for the OpenSSL internal library
# ---------------------------------------------------------------------------#
if(NOT EXISTS ${KIWI_OPENSSL_INTERN_DIR})

	if(APPLE)
		set(OPENSSL_PKG_NAME "openssl-1.1.1a-mac.tar.gz")
	elseif(UNIX)
		set(OPENSSL_PKG_NAME "openssl-1.1.1a-linux.tar.gz")
	elseif(WIN32)
		set(OPENSSL_PKG_NAME "openssl-1.1.1a-windows.zip")
	endif()

	# Download the OpenSSL if not there
	# -------------------------------------------------------------------------#
	if (NOT EXISTS "${PROJECT_SOURCE_DIR}/ThirdParty/${OPENSSL_PKG_NAME}")
		set(OPENSSL_URL "https://github.com/Musicoll/KiwiDependenciesPrebuilt/releases/download/v1.3")

		message(STATUS "Download ${OPENSSL_URL}/${OPENSSL_PKG_NAME}")
		file(DOWNLOAD "${OPENSSL_URL}/${OPENSSL_PKG_NAME}" "${PROJECT_SOURCE_DIR}/ThirdParty/${OPENSSL_PKG_NAME}")
	endif()

	# Unpack the boost package
	# -------------------------------------------------------------------------#
	message(STATUS "Unpack ${PROJECT_SOURCE_DIR}/ThirdParty/${OPENSSL_PKG_NAME} to ${KIWI_OPENSSL_INTERN_DIR}")
	if(APPLE)
		execute_process(COMMAND tar xzf ./${OPENSSL_PKG_NAME} WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/ThirdParty")
	elseif(UNIX)
		execute_process(COMMAND tar xzf ./${OPENSSL_PKG_NAME} WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/ThirdParty")
	elseif(WIN32)
		execute_process(COMMAND 7z x ${OPENSSL_PKG_NAME} WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/ThirdParty")
	endif()

endif()

# Find OpenSSL library on the system
# ---------------------------------------------------------------------------#
set(OPENSSL_USE_STATIC_LIBS ON) # Use the static library
set(OPENSSL_ROOT_DIR "${PROJECT_SOURCE_DIR}/ThirdParty/openssl")

find_package(OpenSSL REQUIRED)

if(OPENSSL_FOUND)
	message(STATUS "OpenSSL include directory: " ${OPENSSL_INCLUDE_DIR})
	message(STATUS "OpenSSL libraries: " ${OPENSSL_LIBRARIES})
else()
	message(SEND_ERROR "OpenSSL not installed")
endif()
