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
        User* result = new User(stmt);
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
            Book* tmp = new Book(stmt);
            arr.push_back(tmp);
        } while(sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        return arr;
    }
    Book* search_book(int book_id) {
        auto stmt = db_query_book(book_id);
        if (stmt == nullptr) return nullptr;
        Book *tmp = new Book(stmt);
        sqlite3_finalize(stmt);
        return tmp;
    }

    Book::Book() {

    }
}
