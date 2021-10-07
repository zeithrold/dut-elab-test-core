//
// Created by Zeithrold on 2021/10/1.
//
#include <sqlite3.h>
#include <cstdio>

using namespace std;

namespace dutelab {
    void open_database();
    void close_database();
    sqlite3_stmt* query_user(string uuid);
}
