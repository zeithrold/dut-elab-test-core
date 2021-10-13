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
        char email[40];
        printf("email > ");
        scanf("%s", email);
        if (email[0] == '0') {
            return "exit";
        }
        if (!dutelab::db_query_email_exist(email)) {
            fatal("Email not exists in the database, try again.");
            return "";
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
        std::cout << "Choose your select > ";
        int result = 0;
        scanf("%d", &result);
        return result;
    }
    void menu_query_book() {
        std::cout << "Type a keyword to search book." << endl;
        char target_keyword[40];
        printf("type a keyword > ");
        scanf("%s", target_keyword);
        if (target_keyword[0] == '\0') {
            dutelab::fatal("You didn't type a keyword.");
        }
        auto result = dutelab::search_book(target_keyword);
        if (result.empty()) {
            std::cout << "No matches found!" << endl;
            return;
        }
        PRINT_STAR;
        std::cout << "Here's result for you." << endl;
        for (const auto &item : result) {
            PRINT_STAR;
            std::cout << "Book ID: " << item->book_id << endl;
            std::cout << "Name: " << item->name << endl;
            std::cout << "ISBN: " << item->isbn << endl;
            std::cout << "Publisher: " << item->publisher << endl;
            std::cout << "Max Amount: " << item->max_amount << endl;
            std::cout << "Current Amount: " << item->current_amount << endl;
            std::cout << "Authors: " << endl;
            for (const auto &author : item->authors) {
                std::cout << "\t" << author << endl;
            }
            PRINT_STAR;
        }
        PRINT_STAR;
    }
    void menu_borrow_book(User* usr) {
        std::cout << "Type a book id to lock book." << endl;
        int target_book_id;
        printf("type a book id > ");
        scanf("%d", &target_book_id);
        usr->lend_book(target_book_id);
    }
    void menu_return_book(User* usr) {
        std::cout << "Type a book id to lock book." << endl;
        int target_book_id;
        printf("type a book id > ");
        scanf("%d", &target_book_id);
        usr->return_book(target_book_id);
    }
    void menu_show_usr_info(User* usr) {
        std::cout << "Here's your info." << endl;
        std::cout << "UID: " << usr->uid << endl;
        std::cout << "Name: " << usr->name << endl;
        std::cout << "Email: " << usr->email << endl;
        std::cout << "Permission Group: " << usr->permission_group << endl;
        std::cout << "Lent Books (stored by book id)" << endl;
        for (const auto &item : usr->lent_books) {
            std::cout << "\t" << item << endl;
        }
    }
    void section(int selected_section, string email) {
        auto usr = get_user(email);
        switch (selected_section) {
            case 1:
                menu_query_book();
            case 2:
                menu_borrow_book(usr);
            case 3:
                menu_return_book(usr);
            case 4:
                menu_show_usr_info(usr);
            //TODO: Admin section
            default:
                std::cout << "No matches" << endl;
        }
    }
}
