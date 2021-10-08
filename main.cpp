#include <iostream>
#include <unistd.h>
#include "extlib/unistd.h"
#include "database.h"
#include "data.h"
#include "encrypt.h"
#define PRINT_STAR std::cout << "******************************************" << std::endl
#define MAX_PASSWORD_LENGTH 16

using namespace std;

void fatal(string reason) {
    std::cout << "Fatal: " << reason << std::endl;
    std::cout << "Press enter to retry." << std::endl;
    getchar();
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    while(true) {
        // LOGIN PHASE
        PRINT_STAR;
        std::cout << "DUT ELAB TEST PROJECT" << std::endl;
        std::cout << "Library Management System" << std::endl;
        std::cout << "Written by Zeithrold, open-sourced at GitHub under BSD-3 License." << std::endl;
        PRINT_STAR;
        std::cout << "Login(enter your email address, 0 to exit)" << std::endl;
        char* email;
        printf("email > ");
        scanf("%s", email);
        if (email[0] == '0') {
            exit(0);
        }
        if (!dutelab::query_email_exist(email)) {
            fatal("Email not exists in the database, try again.");
            continue;
            // TODO: clear std.
        }
        char *password = getpass("password (hidden)> ");
        bool login_result = dutelab::login(email, dutelab::encrypt(password));
        if (!login_result) {
            fatal("Password not match, try again.");
        }

    }
}




