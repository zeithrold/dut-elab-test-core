cmake_minimum_required(VERSION 3.20)
project(dut_elab_test_core)

set(CMAKE_CXX_STANDARD 14)

link_libraries("/usr/local/Cellar/sqlite/3.36.0/lib/libsqlite3.0.dylib")


add_executable(dut_elab_test_core main.cpp database.h data.h io.h data.cpp database.cpp extlib/unistd.h encrypt.cpp encrypt.h io.cpp extlib/configor/include/configor/json.hpp)
