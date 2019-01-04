
message(STATUS "OpenSSL - searching the static OpenSSL library 1.69.0 with system")
message(STATUS "OpenSSL root dir default" ${OPENSSL_ROOT_DIR})
# -----------------------------------------------------------------------------#

# Start the script
# -----------------------------------------------------------------------------#
set(OPENSSL_USE_STATIC_LIBS ON) # Use the static library
#set(OPENSSL_ROOT_DIR "${PROJECT_SOURCE_DIR}/ThirdParty/openssl")
set(OPENSSL_ROOT_DIR "/usr/local/Cellar/openssl/1.0.2q")

# Find OpenSSL library on the system
# ---------------------------------------------------------------------------#
find_package(OpenSSL REQUIRED)

if(OPENSSL_FOUND)
	message(STATUS "OpenSSL include directory: " ${OPENSSL_INCLUDE_DIR})
	message(STATUS "OpenSSL libraries:" ${OPENSSL_LIBRARIES})
else()
	message(SEND_ERROR "OpenSSL not installed")
endif()
