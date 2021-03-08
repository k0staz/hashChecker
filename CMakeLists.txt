cmake_minimum_required(VERSION 3.17)
project(HashChecker)

set(CMAKE_CXX_STANDARD 17)

add_executable(HashChecker main.cpp md5.cpp md5.h SHA1.cpp SHA1.h)