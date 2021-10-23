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
        std::vector<string> authors;
        Book ();
        Book (sqlite3_stmt* stmt) {
            this->book_id = sqlite3_column_int(stmt, 0);
            this->name = (char *) sqlite3_column_text(stmt, 1);
            this->isbn = (char *) sqlite3_column_text(stmt, 2);
            this->publisher = (char *) sqlite3_column_text(stmt, 3);
            this->registered_at = sqlite3_column_int64(stmt, 4);
            this->registered_by = (char *) sqlite3_column_text(stmt, 5);
            this->max_amount = sqlite3_column_int(stmt, 6);
            this->current_amount = sqlite3_column_int(stmt, 7);
            string authors_str = (char *) sqlite3_column_text(stmt, 8);
            remove_reverse_slash(&authors_str);
            json authors_arr = json::parse(authors_str);
            for (auto iter = authors_arr.begin(); iter != authors_arr.end(); iter++) {
                this->authors.push_back(iter.value());
            }
        }
    };
    std::vector<Book *> search_book(string keyword);
    Book * search_book(int book_id);
    void output_book(Book* target_book);
    class User {
    public:
        int uid;
        string name;
        string email;
        string password;
        string permission_group;
        std::vector<int> lent_books;
        User (sqlite3_stmt* stmt) {
            this->uid = sqlite3_column_int(stmt, 0);
            this->name = (char *)sqlite3_column_text(stmt, 1);
            this->email = (char *)sqlite3_column_text(stmt, 2);
            this->password = (char *)sqlite3_column_text(stmt, 3);
            this->permission_group = (char *)sqlite3_column_text(stmt, 4);
            string str_lent_books = (char *)sqlite3_column_text(stmt, 5);
            remove_reverse_slash(&str_lent_books);
            json arr_lent_books = json::parse(str_lent_books);
            for (auto iter = arr_lent_books.begin(); iter != arr_lent_books.end(); iter++) {
                this->lent_books.push_back(iter.value());
            }
        }
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
            cout << "Is it true? (yes to confirm, other to revoke) > ";
            cin.ignore();
            string confirm;
            std::getline(std::cin, confirm);
            if (confirm != "yes")
                return false;
            db_io_book(target_book->book_id, ELAB_BOOK_LEND);
            this->lent_books.push_back(target_book->book_id);
            json new_lent_books = this->lent_books;
            db_update_user_lent_books(this->email, new_lent_books.dump());
        }

        bool return_book(int book_id) {
            auto it = find(this->lent_books.begin(), this->lent_books.end(), book_id);
            if (it == this->lent_books.end()) {
                fatal("You haven't lent the book yet.");
                return false;
            }
            auto target_book = search_book(book_id);
            output_book(target_book);
            cout << "Is it true? (yes to confirm, other to revoke) > ";
            cin.ignore();
            string confirm;
            std::getline(std::cin, confirm);
            if (confirm != "yes")
                return false;
            this->lent_books.erase(std::remove(
                    this->lent_books.begin(), this->lent_books.end(), book_id), this->lent_books.end());
            json arr_lent_books = this->lent_books;
            db_update_user_lent_books(this->email, arr_lent_books.dump());
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
