//
// Created by Zeithrold on 2021/10/7.
//

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <ctime>
#include "database.h"
#include "extlib/configor/include/configor/json.hpp"
#if defined(_WIN32)
#include <objbase.h>
#elif defined(__linux__)
#include <uuid/uuid.h>
#endif

using namespace std;
using namespace configor;
using namespace dutelab;

sqlite3 *sql;
const char *path = "./data.db";
sqlite3_stmt *stmt = nullptr;

int callback(void *data, int argc, char **argv, char **azColName) {return 0;}

namespace dutelab {
    void remove_reverse_slash(string* target) {
        target->erase(remove(target->begin(), target->end(), '\\'), target->end());
    }
    void open_database() {
        // Just a default sqlite3 opening statement.
        int open_result = sqlite3_open_v2(
                path, &sql, SQLITE_OPEN_READWRITE |
                SQLITE_OPEN_CREATE |
                SQLITE_OPEN_NOMUTEX |
                SQLITE_OPEN_SHAREDCACHE, nullptr);
        if (open_result == SQLITE_OK) {
            printf("Successfully opened SQLite Database.\n");
        } else {
            printf("Failed while opening Database.\n");
            exit(-1);
        }
    }
    void close_database() {
        // Sure? is the sqlite3 close DB statement just that simple?
        sqlite3_close_v2(sql);
    }
    sqlite3_stmt* db_query_user(const string& email) {
        sqlite3_stmt *stmt = nullptr;
        // For doubt concerning `stringstream`:
        // The SQL statement is dynamic, to merge string, `stringstream` is essential.
        string sql_sentence =
                "SELECT * FROM dut_user WHERE email='" + email + "';";
        int result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
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
    sqlite3_stmt* db_query_book(const string& keyword) {
        string sql_sentence = "SELECT * FROM dut_book WHERE name like '%" + keyword + "%';";
        int result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            throw "SQLite QUERY failed.";
        }
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            return nullptr;
        }
        return stmt;
    }
    sqlite3_stmt* db_query_book(const int book_id) {
        string sql_sentence = "SELECT * FROM dut_book WHERE book_id=" + to_string(book_id) + ";";
        int result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            throw "SQLite QUERY failed.";
        }
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            return nullptr;
        }
        return stmt;
    }
    bool db_io_book(const int book_id, const int type) {
        auto query_stmt = db_query_book(book_id);
        if (query_stmt == nullptr) {
            cout << "Cannot find the target book." << endl;
            return false;
        }
        sqlite3_finalize(query_stmt);
        auto stmt = db_query_book(book_id);
        int current_amount = sqlite3_column_int(stmt, 7);
        int target_amount;
        if (type == ELAB_BOOK_LEND) {
            target_amount = current_amount - 1;
        } else {
            target_amount = current_amount + 1;
        }
        if (target_amount < 0) {
            cout << "You don't have enough book to borrow." << endl;
            return false;
        }
        string sql_sentence =
            "UPDATE dut_book SET current_amount=" +
            to_string(target_amount) + " " +
            "WHERE book_id=" +
            to_string(book_id) + ";";
        sqlite3_exec(sql, (char *)sql_sentence.data(), nullptr, nullptr, nullptr);
        return true;
    }
    bool db_query_email_exist(const string& email) {
        auto stmt = db_query_user(email);
        bool result = stmt != nullptr;
        sqlite3_finalize(stmt);
        return result;
    }
    bool db_update_user_lent_books(const string& email, const string& lent_books) {
        string sql_sentence =
                "UPDATE dut_user SET lent_books='" +
                lent_books +
                "' WHERE email='" +
                email + "';";
        sqlite3_exec(sql, (char *)sql_sentence.data(), callback,  nullptr, nullptr);
        return true;
    }
    bool db_remove_book(int book_id) {
        if (db_query_book(book_id) == nullptr) {
            return false;
        }
        string sql_sentence =
            "SELECT * FROM dut_book WHERE book_id=" +
            to_string(book_id) +
            ";";
        auto result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            return false;
        }
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            sqlite3_finalize(stmt);
            // G O L A N G
            return false;
        }
        int max_book = sqlite3_column_int(stmt, 6);
        int current_book = sqlite3_column_int(stmt, 7);
        if (max_book != current_book) {
            return false;
        }
        sqlite3_finalize(stmt);
        sql_sentence.clear();
        sql_sentence =
                "DELETE FROM dut_book WHERE book_id=" +
                to_string(book_id) +
                ";";
        sqlite3_exec(sql, (char *)sql_sentence.data(), nullptr, nullptr, nullptr);

//        while (sqlite3_step(stmt) == SQLITE_ROW) {
//            string str_lent_books = (char *)sqlite3_column_text(stmt, 5);
//            remove_reverse_slash(&str_lent_books);
//            json lent_books = json::parse(str_lent_books);
//            int location = -1;
//            for (auto iter = lent_books.begin(); iter != lent_books.end(); iter++) {
//                if (iter.value() == book_id) {
//                    location = atoi(iter.key().c_str());
//                }
//            }
//            lent_books.erase(location);
//            string sql_sentence_minor = "UPDATE dut_user SET lent_books='" +
//                    lent_books.dump() +
//                    "' WHERE email='" +
//                    (char *)sqlite3_column_text(stmt, 2) + "';";
//            sqlite3_exec(sql, (char *)sql_sentence_minor.data(), nullptr, nullptr, nullptr);
//        }
        return true;
    }
    bool db_add_book(int book_id, unsigned int amount) {
        auto target_book = db_query_book(book_id);
        if (target_book == nullptr) {
            return false;
        }
        int before_max_book = sqlite3_column_int(target_book, 6);
        int target_max_book = before_max_book + amount;
        int current_book = sqlite3_column_int(target_book, 7);
        string sql_sentence =
            "UPDATE dut_book SET max_amount=" +
            to_string(target_max_book) + ", " +
            "current_amount=" + to_string(current_book + amount) + " " +
            "WHERE book_id=" +
            to_string(book_id) + ";";
        sqlite3_exec(sql, (char *)sql_sentence.data(), nullptr, nullptr, nullptr);
        return true;
    }
    bool db_del_book(int book_id, unsigned int amount) {
        auto target_book = db_query_book(book_id);
        if (target_book == nullptr) {
            return false;
        }
        int before_max_book = sqlite3_column_int(target_book, 6);
        int target_max_book = before_max_book - amount;
        int current_book = sqlite3_column_int(target_book, 7);
        if ((current_book - amount) < 0) {
            return false;
        }
        string sql_sentence =
                "UPDATE dut_book SET max_amount=" +
                to_string(target_max_book) + ", " +
                "current_amount=" +
                to_string(current_book - amount) + " "
                "WHERE book_id=" +
                to_string(book_id) + ";";
        sqlite3_exec(sql, (char *)sql_sentence.data(), nullptr, nullptr, nullptr);
        return true;
    }
    bool db_register_book(
            int book_id,
            const string& name,
            const string& isbn,
            const string& publisher,
            int max_amount,
            const std::vector<string>& authors,
            const string& registered_by
    ) {
        json arr_authors = authors;
        stringstream stream;
        string sql_sentence;
        string str_authors = arr_authors.dump();
        stream << "INSERT INTO dut_book ";
        stream << "(book_id, name, isbn, publisher, max_amount, current_amount, authors, registered_by, registered_at) ";
        stream << "VALUES (";
        stream << book_id << ",";
        stream << "'" << name << "'" << ",";
        stream << "'" << isbn << "'" << ",";
        stream << "'" << publisher << "'" <<",";
        stream << max_amount << ",";
        stream << max_amount << ",";
        stream << "'" << str_authors << "'" << ",";
        stream << "'" << registered_by << "'" << ",";
        stream << time(nullptr) << ");";
        sqlite3_exec(sql, (char *)stream.str().data(), nullptr, nullptr, nullptr);
        return true;
    }
    bool db_add_user(int uid,
                     const string& name,
                     const string& email,
                     const string& encrypted_password,
                     bool is_admin) {
        stringstream stream;
        string sql_sentence;
        stream << "INSERT INTO dut_user ";
        stream << "(uid, name, email, password, permission_group, lent_books) ";
        stream << "VALUES (";
        stream << uid << ","; // INT
        stream << "'" << name << "'" << ","; // TEXT
        stream << "'" << email << "'" << ","; // TEXT
        stream << "'" << encrypted_password << "'" << ","; // TEXT
        string permission_group;
        if (is_admin)
            permission_group = "admin";
        else
            permission_group = "user";
        stream << "'" << permission_group << "'" << ","; // TEXT
        stream << "'[]');";
        sql_sentence = stream.str();
        sqlite3_exec(sql, (char *)sql_sentence.data(), nullptr, nullptr, nullptr);
        return true;
    }
    bool db_del_user(string email) {
        stringstream stream;
        string sql_sentence;
        stream << "DELETE FROM dut_user WHERE email='" << email << "';";
        sqlite3_exec(sql, (char *)stream.str().data(), nullptr, nullptr, nullptr);
        return true;
    }
}

