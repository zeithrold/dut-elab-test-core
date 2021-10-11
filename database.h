//
// Created by Zeithrold on 2021/10/1.
//
#include <sqlite3.h>
#include <cstdio>

using namespace std;

namespace dutelab {
    void open_database();
    void close_database();
    bool query_email_exist(string email);
    bool lend_book(int book_id);
    sqlite3_stmt* query_user(string email);
    sqlite3_stmt* query_book(string keyword);
    sqlite3_stmt* query_book(int book_id);
}
