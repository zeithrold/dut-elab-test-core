//
// Created by Zeithrold on 2021/10/1.
//
#include <iostream>
#include <sqlite3.h>
#include "extlib/configor/include/configor/json.hpp"
#include "database.h"
#include "permission.h"
#define MAX_LEND_BOOK 30

using namespace std;
using namespace configor;

typedef string encrypted_string;

namespace dutelab {
    class Book {
    public:
        int book_id;
        string name;
        string isbn;
        string publisher;
        long registered_at;
        string registered_by;
        int max_amount;
        char** authors;
    };

    Book* query_book(string keyword);

    class User {
    public:
        int uid;
        string name;
        string uuid;
        string email;
        encrypted_string password;
        string permission_group;
        int lent_books[MAX_LEND_BOOK];
        bool check_permission(int permission);
        bool lend_book(int book_id);
        bool return_book(int book_id);
        bool extend_ddl(int book_id);

        // Admin operation
        bool register_book(Book book);
        bool add_book(int book_id);
        bool del_book(int book_id);
        bool remove_book(int book_id);
        bool add_user(User usr);
        bool remove_user(string user_uuid);
    };

    class HTTPAuthenticateInfo {
    public:
        string access_key;
        int expire_at;
    };

    HTTPAuthenticateInfo user_authenticate();
    bool user_refresh(string access_key);
    User* get_user(string access_key);
    User* get_user(sqlite3_stmt *stmt);
}
