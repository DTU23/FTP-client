cmake_minimum_required(VERSION 3.6)
project(lecture2)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp src/modules/FTPClient.cpp src/modules/FTPClient.h src/modules/DataSocket.cpp src/modules/DataSocket.h src/modules/Helper.cpp src/modules/Helper.h)
add_executable(lecture2 ${SOURCE_FILES})