cmake_minimum_required(VERSION 3.10)
project(wallet_management)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenSSL paths based on OS
if(WIN32)
    # Windows paths
    # - If using MSYS2: Default path is "C:/msys64/mingw64"
    # - If using custom OpenSSL: Set to your OpenSSL installation path
    set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.dll.a")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.dll.a")
elseif(APPLE)
    # macOS paths (using Homebrew)
    # - For custom installation: Set to your OpenSSL installation path
    set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3")
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.dylib")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib")
endif()

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(QRENCODE REQUIRED libqrencode)

# Find qrencode library
find_library(QRENCODE_LIBRARY NAMES qrencode)
find_path(QRENCODE_INCLUDE_DIR qrencode.h)

if(NOT QRENCODE_LIBRARY OR NOT QRENCODE_INCLUDE_DIR)
    if(WIN32)
        message(FATAL_ERROR "qrencode library not found. Please install it using MSYS2: 'pacman -S mingw-w64-x86_64-qrencode'")
    else()
        message(FATAL_ERROR "qrencode library not found. Please install it using 'brew install qrencode'")
    endif()
endif()

# Add COTP library
add_subdirectory(lib/cotp)

add_executable(wallet_management
    main.cpp
    User.cpp
    UserManager.cpp
    OTPManager.cpp
    PasswordHasher.cpp
)

target_include_directories(wallet_management PRIVATE 
    ${OPENSSL_INCLUDE_DIR}
    ${QRENCODE_INCLUDE_DIR}
)

target_link_libraries(wallet_management
    cotp
    ${OPENSSL_SSL_LIBRARY}
    ${OPENSSL_CRYPTO_LIBRARY}
    ${QRENCODE_LIBRARY}
)

# Copy DLLs to output directory on Windows
if(WIN32)
    add_custom_command(TARGET wallet_management POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENSSL_ROOT_DIR}/bin/libssl-3.dll"
            "${OPENSSL_ROOT_DIR}/bin/libcrypto-3.dll"
            "${QRENCODE_LIBRARY}"
            $<TARGET_FILE_DIR:wallet_management>
    )
endif()