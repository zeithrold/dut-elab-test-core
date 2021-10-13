//
// Created by Zeithrold on 2021/10/7.
//

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include "database.h"
#include "extlib/configor/include/configor/json.hpp"

using namespace std;
using namespace configor;
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
    sqlite3_stmt* db_query_user(const string& email) {
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
        string sql_sentence = "SELECT * FROM dut_book WHERE name like %" + keyword + "%;";
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
    bool db_io_book(const int book_id, int type) {
        auto stmt = db_query_book(book_id);
        if (sqlite3_step(stmt) != SQLITE_ROW) {
            cout << "Cannot find the target book." << endl;
            return false;
        }
        sqlite3_finalize(stmt);
        int current_amount = sqlite3_column_int(stmt, 7);
        int target_amount = type == ELAB_BOOK_LEND ? current_amount - 1 : current_amount + 1;
        string sql_sentence =
            "UPDATE dut_book SET current_amount=" +
            to_string(target_amount) + " " +
            "WHERE book_id=" +
            to_string(book_id) + ";";
        auto result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
        sqlite3_finalize(stmt);
        return result == SQLITE_OK;
    }
    bool db_query_email_exist(const string& email) {
        auto stmt = db_query_user(email);
        bool result = stmt != nullptr;
        sqlite3_finalize(stmt);
        return result;
    }
    bool db_remove_book(int book_id) {
        if (db_query_book(book_id) == nullptr) {
            return false;
        }
        string sql_sentence =
            "DELETE FROM dut_book WHERE book_id=" +
            to_string(book_id) +
            ";";
        auto result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            return false;
        }
        sqlite3_finalize(stmt);
        sql_sentence.clear();
        sql_sentence =
            "SELECT * FROM dut_user WHERE lent_books like %" +
            to_string(book_id) +
            "% ;";
        result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            return false;
        }
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            json lent_books = json::parse((char *)sqlite3_column_text(stmt, 5));
            int location = -1;
            for (auto iter = lent_books.begin(); iter != lent_books.end(); iter++) {
                if (iter.value() == book_id) {
                    location = atoi(iter.key().c_str());
                }
            }
            lent_books.erase(location);
            string sql_sentence_minor = "UPDATE dut_user SET lent_books=\"" + lent_books.dump() + "\";";
            sqlite3_exec(sql, (char *)sql_sentence_minor.data(), nullptr, nullptr, nullptr);
        }
        sqlite3_finalize(stmt);
        return true;
    }
    bool db_add_book(int book_id, unsigned int amount) {
        auto target_book = db_query_book(book_id);
        if (target_book == nullptr) {
            return false;
        }
        int before_max_book = sqlite3_column_int(target_book, 6);
        int target_max_book = before_max_book + amount;
        string sql_sentence =
            "UPDATE dut_book SET max_amount=" +
            to_string(target_max_book) + " " +
            "WHERE book_id=" +
            to_string(book_id) + ";";
        int result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
        sqlite3_finalize(stmt);
        return result == SQLITE_OK;
    }
    bool db_del_book(int book_id, unsigned int amount) {
        auto target_book = db_query_book(book_id);
        if (target_book == nullptr) {
            return false;
        }
        int before_max_book = sqlite3_column_int(target_book, 6);
        int target_max_book = before_max_book - amount;
        int current_book = sqlite3_column_int(stmt, 7);
        if (current_book > target_max_book) {
            return false;
        }
        string sql_sentence =
                "UPDATE dut_book SET max_amount=" +
                to_string(target_max_book) + " " +
                "WHERE book_id=" +
                to_string(book_id) + ";";
        int result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
        sqlite3_finalize(stmt);
        return result == SQLITE_OK;
    }
    bool db_register_book(
            int book_id,
            const string& name,
            const string& isbn,
            const string& publisher,
            int max_amount,
            const vector<string>& authors,
            const string& registered_by
    ) {
        json arr_authors = authors;
        stringstream stream;
        string sql_sentence;
        stream << "INSERT INTO dut_book ";
        stream << "(book_id, name, isbn, publisher, max_amount, current_amount, authors, registered_by, registered_at) ";
        stream << book_id << ",";
        stream << isbn << ",";
        stream << publisher << ",";
        stream << max_amount << ",";
        stream << max_amount << ",";
        stream << arr_authors.dump() << ",";
        stream << registered_by << ",";
        stream << time(nullptr) << ";";
        stream >> sql_sentence;
        int result = sqlite3_prepare_v2(sql, (char *)sql_sentence.data(), -1, &stmt, nullptr);
        if (result != SQLITE_OK) {
            return false;
        }
        sqlite3_finalize(stmt);
    }
}

