//
// Created by Zeithrold on 2021/10/7.
//
#include "data.h"
#include "extlib/configor/include/configor/json.hpp"
#include <cstring>
#include <utility>
#include <vector>
#include <algorithm>
#include <sqlite3.h>

using namespace dutelab;
using namespace configor;

void fatal(const string& reason) {
    std::cout << "Fatal: " << reason << std::endl;
    std::cout << "Press enter to retry." << std::endl;
    getchar();
}

namespace dutelab {
    void output_book(Book* target_book){
        cout << "Here's your target." << endl;
        cout << "Book ID:" << target_book->book_id <<endl;
        cout << "Name: " << target_book->name << endl;
        cout << "ISBN: " << target_book->isbn << endl;
        cout << "Publisher: " << target_book->publisher << endl;
    }
    User* get_user(sqlite3_stmt *stmt) {
        User* result = new User();
        result->uid = sqlite3_column_int(stmt, 0);
        result->name = (char *)sqlite3_column_text(stmt, 1);
        result->email = (char *)sqlite3_column_text(stmt, 2);
        result->password = (char *)sqlite3_column_text(stmt, 3);
        result->permission_group = (char *)sqlite3_column_text(stmt, 4);
        string str_lent_books = (char *)sqlite3_column_text(stmt, 5);
        json arr_lent_books = json::parse(str_lent_books);
        for (auto iter = arr_lent_books.begin(); iter != arr_lent_books.end(); iter++) {
            result->lent_books.push_back(iter.value());
        }
        sqlite3_finalize(stmt);
        return result;
    }

    User* get_user(string email) {
        sqlite3_stmt *stmt = db_query_user(std::move(email));
        if (stmt == nullptr) return nullptr;
        return get_user(stmt);
    }

    bool login(const string& email, const string& encrypted_password) {
        User* usr = get_user(email);
        if (usr == nullptr) return false;
        return strcmp((char *)usr->password.data(), (char *)encrypted_password.data()) == 0;
    }

    vector<Book *> search_book(string keyword) {
        vector<Book*> arr;
        sqlite3_stmt* stmt = db_query_book(std::move(keyword));
        if (stmt == nullptr) return arr;
        do {
            Book* tmp = new Book();
            tmp->book_id = sqlite3_column_int(stmt, 0);
            tmp->name = (char *) sqlite3_column_text(stmt, 1);
            tmp->isbn = (char *) sqlite3_column_text(stmt, 2);
            tmp->publisher = (char *) sqlite3_column_text(stmt, 3);
            tmp->registered_at = sqlite3_column_int64(stmt, 4);
            tmp->registered_by = (char *) sqlite3_column_text(stmt, 5);
            tmp->max_amount = sqlite3_column_int(stmt, 6);
            tmp->current_amount = sqlite3_column_int(stmt, 7);
            string authors_str = (char *) sqlite3_column_text(stmt, 8);
            json authors_arr = json::parse(authors_str);
            for (auto iter = authors_arr.begin(); iter != authors_arr.end(); iter++) {
                tmp->authors.push_back(iter.value());
            }
            arr.push_back(tmp);
        } while(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        return arr;
    }
    Book* search_book(int book_id) {
        auto stmt = db_query_book(book_id);
        if (stmt == nullptr) return nullptr;
        Book *tmp = new Book();
        tmp->book_id = sqlite3_column_int(stmt, 0);
        tmp->name = (char *) sqlite3_column_text(stmt, 1);
        tmp->isbn = (char *) sqlite3_column_text(stmt, 2);
        tmp->publisher = (char *) sqlite3_column_text(stmt, 3);
        tmp->registered_at = sqlite3_column_int64(stmt, 4);
        tmp->registered_by = (char *) sqlite3_column_text(stmt, 5);
        tmp->max_amount = sqlite3_column_int(stmt, 6);
        tmp->current_amount = sqlite3_column_int(stmt, 7);
        string authors_str = (char *) sqlite3_column_text(stmt, 8);
        json authors_arr = json::parse(authors_str);
        for (auto iter = authors_arr.begin(); iter != authors_arr.end(); iter++) {
            tmp->authors.push_back(iter.value());
        }
        sqlite3_finalize(stmt);
        return tmp;
    }
}
