# DUT Elab Test Project

This is a Simple Library Management System, written in C++ with the support of SQLite and [Nomango/configor](https://github.com/Nomango/configor).

The project is built `pass` on `Ubuntu 18.04`, `Ubuntu 20.04`, `macOS 10.15`, `macOS 11` at `clang` and `gcc` compiler.

**Windows is not supported.**

## Getting Started

1. Clone the repo and tun `git submodule update --init`
2. Be sure that SQLite3 is installed on your system. Although SQLite3 is basically installed at most of Linux Distributions and macOS, please check it out once again. You can test it by simply writing a C program with `#include <sqlite3.h>` and calling some of its functions.
3. Install Clang compiler, for the reasons above gcc is not supported.
4. Run `CC=clang CXX=clang++ cmake CMakeLists.txt; make`, which will compile the source code.
5. Run `cd output; cat ../initialDB.sql | sqlite3 data.db`, which will create a initial database.
6. Run `./dut_elab_test_core` and enjoy!
