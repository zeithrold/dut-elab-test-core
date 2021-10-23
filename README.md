# DUT Elab Test Project

This is a Simple Library Management System, written in C++ with the support of SQLite and [Nomango/configor](https://github.com/Nomango/configor).

The project is built `pass` on `Ubuntu 18.04`, `Ubuntu 20.04`, `macOS 10.15`, `macOS 11` at `clang` compiler.

Due to [Nomango/configor](https://github.com/Nomango/configor)'s bug(Commit 491c5f1), gcc compiler is **NOT** supported and **Windows is not supported** either.

## Getting Started

1. Be sure that SQLite3 is installed on your system. Although SQLite3 is basically installed at most of Linux Distributions and macOS, please check it out once again. You can test it by simply writing a C program with `#include <sqlite3.h>` and calling some of its functions.
2. Install Clang compiler, for the reasons above gcc is not supported.
3. Run `CC=clang CXX=clang++ cmake CMakeLists.txt; make`, which will compile the source code.
4. Run `cd output; cat ../initialDB.sql | sqlite3 data.db`, which will create a initial database.
5. Run `./dut_elab_test_core` and enjoy!
