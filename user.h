//
// Created by Zeithrold on 2021/10/1.
//
#include <iostream>
#include "database.h"

#ifndef DUT_ELAB_TEST_CORE_USER_H
#define DUT_ELAB_TEST_CORE_USER_H

using namespace std;

typedef string encrypted_string;

namespace DutElab {
    class User {
    public:
        int uid;
        string name;
        string uuid;
        string lent_books[];
        string email;
        encrypted_string password;
        string permission_group;
    };
}

#endif //DUT_ELAB_TEST_CORE_USER_H
