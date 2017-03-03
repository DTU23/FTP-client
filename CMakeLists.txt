cmake_minimum_required(VERSION 3.6)
project(lecture2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/lib/socket/FTPClient.cpp src/lib/socket/FTPClient.h src/lib/socket/DataSocket.cpp src/lib/socket/DataSocket.h src/lib/Helper.cpp src/lib/Helper.h)
add_executable(lecture2 ${SOURCE_FILES})