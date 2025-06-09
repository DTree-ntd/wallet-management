cmake_minimum_required(VERSION 3.10)
project(cotp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add all source files
set(COTP_SOURCES
    cotp.c
    otpuri.c
)

add_library(cotp STATIC ${COTP_SOURCES})

target_include_directories(cotp PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)
target_link_libraries(cotp ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY}) 