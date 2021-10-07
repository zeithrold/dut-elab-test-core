//
// Created by Zeithrold on 2021/10/7.
//

#include <cstdlib>
#include <sstream>
#include <string>
#include "database.h"

using namespace std;
using namespace dutelab;

sqlite3 *sql;
const char *path = "./data.db";
sqlite3_stmt *stmt = 0;

namespace dutelab {
    void open_database() {
        int open_result = sqlite3_open_v2(
                path, &sql, SQLITE_OPEN_READWRITE |
                SQLITE_OPEN_CREATE |
                SQLITE_OPEN_NOMUTEX |
                SQLITE_OPEN_SHAREDCACHE, 0);
        if (open_result == SQLITE_OK) {
            printf("Successfully opened SQLite Database.");
        } else {
            printf("Failed while opening Database.");
            exit(-1);
        }
    }
    void close_database() {
        sqlite3_close_v2(sql);
    }
    sqlite3_stmt* query_user(string uuid) {
        stringstream sql_sentence;
        sql_sentence << "SELECT * FROM dut_user WHERE uuid='" << uuid << "'";
        char* ss;
        sql_sentence >> ss;
        int result = sqlite3_prepare_v2(sql, ss, -1, &stmt, 0);
        if (result != SQLITE_OK) {
            throw "SQLite QUERY failed.";
        }
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            return 0;
        }
        return stmt;
    }
}

