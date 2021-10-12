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
    bool db_query_email_exist(const string& email);
    bool db_io_book(int book_id, int type);
    bool db_register_book(
            int book_id,
            string name,
            string isbn,
            string publisher,
            int max_amount,
            vector<string> authors,
            string registered_by
            );
    bool db_add_book(int book_id, unsigned int amount);
    bool db_del_book(int book_id, unsigned int amount);
    bool db_remove_book(int book_id);
    sqlite3_stmt* db_query_user(const string& email);
    sqlite3_stmt* db_query_book(const string& keyword);
    sqlite3_stmt* db_query_book(int book_id);
}
