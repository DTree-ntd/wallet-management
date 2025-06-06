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
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.a")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.a")
    
    # Windows qrencode paths
    set(QRENCODE_ROOT_DIR "C:/msys64/mingw64")
    set(QRENCODE_INCLUDE_DIR "${QRENCODE_ROOT_DIR}/include")
    set(QRENCODE_LIBRARY "${QRENCODE_ROOT_DIR}/lib/libqrencode.dll.a")
elseif(APPLE)
    # macOS paths (using Homebrew)
    # - For custom installation: Set to your OpenSSL installation path
    set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3")
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.dylib")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib")
    
    # macOS qrencode paths
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(QRENCODE REQUIRED libqrencode)
endif()

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Verify qrencode library
if(WIN32)
    if(NOT EXISTS "${QRENCODE_LIBRARY}" OR NOT EXISTS "${QRENCODE_INCLUDE_DIR}/qrencode.h")
        message(FATAL_ERROR "qrencode library not found. Please install it using MSYS2: 'pacman -S mingw-w64-x86_64-qrencode'")
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
            "${QRENCODE_ROOT_DIR}/bin/libqrencode.dll"
            $<TARGET_FILE_DIR:wallet_management>
    )
endif()