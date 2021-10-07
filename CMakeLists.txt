cmake_minimum_required(VERSION 3.20)
project(dut_elab_test_core)

set(CMAKE_CXX_STANDARD 14)

add_executable(dut_elab_test_core main.cpp database.h data.h permission.h http.h io.h http.cpp)
