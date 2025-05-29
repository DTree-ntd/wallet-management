cmake_minimum_required(VERSION 3.10)
project(wallet_management)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wallet_management
    main.cpp
    User.cpp
    UserManager.cpp
)