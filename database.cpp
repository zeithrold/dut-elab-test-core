//
// Created by Zeithrold on 2021/10/7.
//

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
#include "database.h"

using namespace std;
using namespace dutelab;

sqlite3 *sql;
const char *path = "./data.db";
sqlite3_stmt *stmt = nullptr;

namespace dutelab {
    void open_database() {
        // Just a default sqlite3 opening statement.
        int open_result = sqlite3_open_v2(
                path, &sql, SQLITE_OPEN_READWRITE |
                SQLITE_OPEN_CREATE |
                SQLITE_OPEN_NOMUTEX |
                SQLITE_OPEN_SHAREDCACHE, nullptr);
        if (open_result == SQLITE_OK) {
            printf("Successfully opened SQLite Database.");
        } else {
            printf("Failed while opening Database.");
            exit(-1);
        }
    }
    void close_database() {
        // Sure? is the sqlite3 close DB statement just that simple?
        sqlite3_close_v2(sql);
    }
    sqlite3_stmt* query_user(const string email) {
        stringstream sql_sentence;
        // For doubt concerning `stringstream`:
        // The SQL statement is dynamic, to merge string, `stringstream` is essential.
        sql_sentence << "SELECT * FROM dut_user WHERE email='" << email << "';";
        string ss;
        sql_sentence >> ss;
        char* s = (char *)ss.data();
        int result = sqlite3_prepare_v2(sql, s, -1, &stmt, 0);
        // Perhaps there's a better error handling system.
        if (result != SQLITE_OK) {
            throw "SQLite QUERY failed.";
        }
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            // G O L A N G
            return nullptr;
        }
        return stmt;
    }
    sqlite3_stmt* query_book(const string keyword) {
        stringstream sql_sentence;
        sql_sentence << "SELECT * FROM dut_book WHERE name like %" << keyword << "%;";
        string ss;
        sql_sentence >> ss;
        char* s = (char *)ss.data();
        int result = sqlite3_prepare_v2(sql, s, -1, &stmt, 0);
        if (result != SQLITE_OK) {
            throw "SQLite QUERY failed.";
        }
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            return nullptr;
        }
        return stmt;
    }
    sqlite3_stmt* query_book(const int book_id) {
        stringstream sql_sentence;
        sql_sentence << "SELECT * FROM dut_book WHERE book_id=" << book_id << ";";
        string ss;
        sql_sentence >> ss;
        char* s = (char *)ss.data();
        int result = sqlite3_prepare_v2(sql, s, -1, &stmt, 0);
        if (result != SQLITE_OK) {
            throw "SQLite QUERY failed.";
        }
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            return nullptr;
        }
        return stmt;
    }
    bool io_book(const int book_id, int type) {
        auto stmt = query_book(book_id);
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            cout << "Cannot find the target book." << endl;
            return false;
        }
        int current_amount = sqlite3_column_int(stmt, 7);
        int target_amount = type == ELAB_BOOK_LEND ? current_amount - 1 : current_amount + 1;
        stringstream sql_sentence;
        sql_sentence <<
            "UPDATE dut_book SET current_amount " <<
            target_amount <<
            "WHERE book_id=" <<
            book_id << ";";
        string ss;
        sql_sentence >> ss;
        char* s = (char *)ss.data();
        int result = sqlite3_prepare_v2(sql, s, -1, &stmt, 0);
        return result == SQLITE_OK;
    }
    bool query_email_exist(const string email) {
        sqlite3_stmt* stmt = query_user(email);
        return stmt != nullptr;
    }
}

