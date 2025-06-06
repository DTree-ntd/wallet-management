cmake_minimum_required(VERSION 3.10)
project(wallet_management)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenSSL paths based on OS
if(WIN32)
    # Windows paths
    set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.a")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.a")
elseif(APPLE)
    # macOS paths (using Homebrew)
    set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.dylib")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib")
endif()

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add COTP library
add_subdirectory(lib/cotp)

add_executable(wallet_management
    main.cpp
    User.cpp
    UserManager.cpp
    OTPManager.cpp
)

target_include_directories(wallet_management PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(wallet_management
    cotp
    ${OPENSSL_SSL_LIBRARY}
    ${OPENSSL_CRYPTO_LIBRARY}
)