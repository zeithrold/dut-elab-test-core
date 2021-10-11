//
// Created by Zeithrold on 2021/10/1.
//
#include <sqlite3.h>
#include <cstdio>
#define ELAB_BOOK_LEND 0
#define ELAB_BOOK_RETURN 1

using namespace std;

namespace dutelab {
    void open_database();
    void close_database();
    bool query_email_exist(string email);
    bool io_book(int book_id, int type);
    sqlite3_stmt* query_user(string email);
    sqlite3_stmt* query_book(string keyword);
    sqlite3_stmt* query_book(int book_id);
}
