//
// Created by Zeithrold on 2021/10/1.
//
#include <iostream>
#include "extlib/configor/include/configor/json.hpp"
#include "book.h"
#include "database.h"
#include "permission.h"

using namespace std;
using namespace configor;

typedef string encrypted_string;

namespace dutelab {
    class User {
    public:
        int uid;
        string name;
        string uuid;
        string email;
        encrypted_string password;
        string permission_group;
        int lent_books[];
        bool check_permission(int permission);
        bool lend_book(int book_id);
        bool return_book(int book_id);
        bool extend_ddl(int book_id);
        bool register_book(Book book);
    };

    class HTTPAuthenticateInfo {
    public:
        string access_key;
        int expire_at;
    };

    HTTPAuthenticateInfo user_authenticate();
    User get_user(string access_key);
}
