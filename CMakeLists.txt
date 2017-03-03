cmake_minimum_required(VERSION 3.6)
project(lecture2)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/lib/socket/ServerSocket.cpp src/lib/socket/ServerSocket.h src/lib/socket/FTPClient.cpp src/lib/socket/FTPClient.h)
add_executable(lecture2 ${SOURCE_FILES})