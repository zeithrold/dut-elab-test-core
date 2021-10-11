//
// Created by Zeithrold on 2021/10/10.
//

#include "io.h"
#include "database.h"
#include "data.h"
#include "encrypt.h"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <unistd.h>
#define PRINT_STAR std::cout << "******************************************" << std::endl

using namespace std;

namespace dutelab {
    void fatal(const string& reason) {
        std::cout << "Fatal: " << reason << std::endl;
        std::cout << "Press enter to retry." << std::endl;
        getchar();
    }
    void start() {
        PRINT_STAR;
        std::cout << "DUT ELAB TEST PROJECT" << std::endl;
        std::cout << "Library Management System" << std::endl;
        std::cout << "Written by Zeithrold, open-sourced at GitHub under BSD-3 License." << std::endl;
        PRINT_STAR;
    }
    string login(){
        std::cout << "Login(enter your email address, 0 to exit)" << std::endl;
        char *email = nullptr;
        printf("email > ");
        scanf("%s", email);
        if (email[0] == '0') {
            return "exit";
        }
        if (!dutelab::query_email_exist(email)) {
            fatal("Email not exists in the database, try again.");
            return "";
            // TODO: clear std.
        }
        char *password = getpass("password (hidden)> ");
        bool login_result = dutelab::login(email, dutelab::encrypt(password));
        if (!login_result) {
            fatal("Password not match, try again.");
            return "";
        }
        return email;
    }
    int menu(string email) {
        const string user_menu_title[] = {
                "1. Query Book",
                "2. Borrow Book",
                "3. Extend DDL",
                "4. My Info"
        };
        const string admin_menu_title[] = {
                "11. Register Book",
                "12. Add Book",
                "13. Remove Book",
                "14. Add User",
                "15. Remove User"
        };
        User* usr = dutelab::get_user(email);
        PRINT_STAR;
        std::cout << "Welcome, " << usr->name << "!" << endl;
        std::cout << "Here's a plenty of menu for you to select." << endl;
        for (const auto & i : user_menu_title) {
            std::cout << i << endl;
        }
        if (usr->permission_group == "admin") {
            std::cout << "...and here's admin menu for you." << endl;
            for (const auto & i : admin_menu_title) {
                std::cout << i << endl;
            }
        }

        PRINT_STAR;
    }
}
