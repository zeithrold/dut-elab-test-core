//
// Created by Zeithrold on 2021/10/7.
//
#include "data.h"
#include "extlib/configor/include/configor/json.hpp"
#include <cstring>
#include <sqlite3.h>

using namespace dutelab;
using namespace configor;

namespace dutelab {

    User* get_user(string access_key) {
        sqlite3_stmt* stmt = query_user(access_key);
        if (stmt == nullptr){
            return nullptr;
        }
        return get_user(stmt);
    }
    User* get_user(sqlite3_stmt *stmt) {
        string str_lent_books = (char *)sqlite3_column_text(stmt, 5);
        json arr_lent_books = json::parse(str_lent_books);
        User* result = new User();
        result->uid = sqlite3_column_int(stmt, 0);
        result->name = (char *)sqlite3_column_text(stmt, 1);
        result->uuid = (char *) sqlite3_column_text(stmt, 2);
        result->email = (char *)sqlite3_column_text(stmt, 3);
        result->password = (char *)sqlite3_column_text(stmt, 4);
        for (int i = 0; i < arr_lent_books.size(); i++) {
            result->lent_books[i] = arr_lent_books.get<int>(i);
        }
        return result;
    }
}
