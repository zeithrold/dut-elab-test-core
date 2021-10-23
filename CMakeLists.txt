cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME dut_elab_test_core)
project(dut_elab_test_core)

set(CMAKE_CXX_STANDARD 14)

# LIB VAR
aux_source_directory(. SRC_LIST)
aux_source_directory(extlib/configor EXTLIB_CONFIGOR)
set(EXTLIB_UNISTD extlib/unistd.h)

find_package(SQLite3 REQUIRED)

#link_libraries("/usr/local/Cellar/sqlite/3.36.0/lib/libsqlite3.0.dylib")
add_executable(dut_elab_test_core
        main.cpp
        ${SRC_LIST}
        ${EXTLIB_CONFIGOR}
        ${EXTLIB_UNISTD}
        )

target_link_libraries(dut_elab_test_core ${SQLite3_LIBRARIES})

set(EXECUTABLE_OUTPUT_PATH ./output)

