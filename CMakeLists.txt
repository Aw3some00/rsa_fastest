cmake_minimum_required(VERSION 3.29)
project(rsa_fastest)

set(CMAKE_CXX_STANDARD 20)

add_executable(rsa_fastest main.cpp
        cryptomath.h)
