//
// Created by Zeithrold on 2021/10/1.
//
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <sqlite3.h>
#include "extlib/configor/include/configor/json.hpp"
#include "database.h"
#define MAXIMUM_LEND_BOOKS 30

using namespace std;
using namespace configor;

void fatal(const string& reason);

namespace dutelab {
    class Book {
    public:
        int book_id;
        string name;
        string isbn;
        string publisher;
        long registered_at;
        string registered_by;
        unsigned int max_amount;
        unsigned int current_amount;
        vector<string> authors;
    };
    vector<Book *> search_book(string keyword);
    Book * search_book(int book_id);
    void output_book(Book* target_book);
    class User {
    public:
        int uid;
        string name;
        string uuid;
        string email;
        string password;
        string permission_group;
        vector<int> lent_books;
        bool lend_book(int book_id) {
            if (this->lent_books.size() >= MAXIMUM_LEND_BOOKS) {
                fatal("The amount of your lent books was on the maximum.");
                return false;
            }
            auto target_book = search_book(book_id);
            if (target_book == nullptr) {
                fatal("Cannot find the book.");
                return false;
            }
            auto it = find(this->lent_books.begin(), this->lent_books.end(), book_id);
            if (it != this->lent_books.end()) {
                fatal("You've already lent the book.");
                return false;
            }
            output_book(target_book);
            cout << "Is it true? (yes to confirm, other to revoke)";
            char *confirm = nullptr;
            scanf("%s", confirm);
            if (strcmp(confirm, "yes") != 0)
                return false;
            this->lent_books.push_back(target_book->book_id);
            return db_io_book(target_book->book_id, ELAB_BOOK_LEND);
        }

        bool return_book(int book_id) {
            auto it = find(this->lent_books.begin(), this->lent_books.end(), book_id);
            if (it == this->lent_books.end()) {
                fatal("You haven't lent the book yet.");
                return false;
            }
            auto target_book = search_book(book_id);
            output_book(target_book);
            cout << "Is it true? (yes to confirm, other to revoke)";
            char *confirm = nullptr;
            scanf("%s", confirm);
            if (strcmp(confirm, "yes") != 0)
                return false;
            this->lent_books.erase(std::remove(
                    this->lent_books.begin(), this->lent_books.end(), book_id), this->lent_books.end());
            return db_io_book(target_book->book_id, ELAB_BOOK_RETURN);
        }

        // Admin operation
        bool register_book(Book* book) const {
            if (this->permission_group != "admin") {
                return false;
            }
            return db_register_book(
                    book->book_id,
                    book->name,
                    book->isbn,
                    book->publisher,
                    book->max_amount,
                    book->authors,
                    this->name
                    );
        }
        bool add_book(int book_id, unsigned int amount) const {
            if (this->permission_group != "admin") {
                return false;
            }
            return db_add_book(book_id, amount);
        }
        bool del_book(int book_id, unsigned int amount) const {
            if (this->permission_group != "admin") {
                return false;
            }
            return db_del_book(book_id, amount);
        }
        bool remove_book(int book_id)  const {
            if (this->permission_group != "admin") {
                return false;
            }
            return db_remove_book(book_id);
        }
    };

    User* get_user(string email);
    User* get_user(sqlite3_stmt *stmt);
    bool login(const string& email, const string& encrypted_password);
}
