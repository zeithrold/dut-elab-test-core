//
// Created by Zeithrold on 2021/10/10.
//

#include "io.h"
#include "database.h"
#include "data.h"
#include "encrypt.h"
#include <cstdio>
#include <iostream>
#include <ctime>
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
    string reg() {
        int uid;
        char *name;
        char *email;
        char *password;
        printf("enter uid > ");
        scanf("%d", &uid);
        printf("enter name > ");
        scanf("%s", name);
        printf("enter email > ");
        scanf("%s", email);
        if (get_user(email) != nullptr) {
            dutelab::fatal("User exists.");
            return "";
        }
        password = getpass("password (hidden)> ");
        password = encrypt(password);
        db_add_user(
                uid,
                name,
                email,
                password,
                false
        );
        return email;
    }
    string login(){
        std::cout << "Login(enter your email address, reg to register, 0 to exit)" << std::endl;
        char email[40];
        printf("email > ");
        scanf("%s", email);
        if (email[0] == '0') {
            return "exit";
        }
        if (strcmp(email, "reg") == 0) {
            return reg();
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
                "4. My Info",
                "-1. Quit"
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
    void menu_admin_reg_book(User * usr) {
        if (usr->permission_group != "admin") {
            dutelab::fatal("You don't have permission to access the menu.");
            return;
        }
        int book_id;
        std::string name;
        std::string isbn;
        std::string publisher;
//        char isbn[40] = {0};
//        char publisher[4] = {0};
        long registered_at = time(0);
        int max_amount = 1;
        std::string txt_max_amount;
//        char txt_max_amount[40] = {0};
        string registered_by = usr->name;
        vector<string> authors;
        while(true) {
            printf("enter book's id > ");
            scanf("%d", &book_id);
            if (search_book(book_id) != nullptr) {
                dutelab::fatal("Duplicate book id, try again.");
                continue;
            }
            break; // I'm hurry!!!!!!
        }
        cin.ignore();
        printf("enter book's name > ");
        std::getline(std::cin, name);
        printf("enter book's isbn > ");
        std::getline(std::cin, isbn);
        printf("enter book's publisher > ");
        std::getline(std::cin, publisher);
        printf("enter book's maximum amount > ");
        std::getline(std::cin, txt_max_amount);
        max_amount = atoi((char *)txt_max_amount.data());
        printf("Now, enter authors of the book.\n");
        while (true) {
            std::string single_author;
            printf("enter book's author by line, empty to end > ");
            std::getline(std::cin, single_author);
            if (single_author == "") {
                break;
            }
            authors.emplace_back(single_author);
        }
        dutelab::db_register_book(
                book_id,
                name,
                isbn,
                publisher,
                max_amount,
                authors,
                usr->name
                );
    }
    void menu_admin_add_book(User *usr) {
        if (usr->permission_group != "admin") {
            dutelab::fatal("You don't have permission to access the menu.");
            return;
        }
        std::cout << "Type a book id to lock book." << endl;
        int target_book_id;
        printf("type a book id > ");
        scanf("%d", &target_book_id);
        if (search_book(target_book_id) == nullptr) {
            dutelab::fatal("No match book found");
            return;
        }
        unsigned int add_amount = 0;
        printf("type add amount > ");
        scanf("%d", &add_amount);
        usr->add_book(target_book_id, add_amount);
    }
    void menu_admin_del_book(User *usr) {
        if (usr->permission_group != "admin") {
            dutelab::fatal("You don't have permission to access the menu.");
            return;
        }
        std::cout << "Type a book id to lock book." << endl;
        int target_book_id;
        printf("type a book id > ");
        scanf("%d", &target_book_id);
        if (search_book(target_book_id) == nullptr) {
            dutelab::fatal("No match book found");
            return;
        }
        unsigned int add_amount = 0;
        printf("type del amount > ");
        scanf("%d", &add_amount);
        usr->del_book(target_book_id, add_amount);
    }
    void menu_admin_add_user(User *usr) {
        if (usr->permission_group != "admin") {
            dutelab::fatal("You don't have permission to access the menu.");
            return;
        }
        int uid;
        char *name;
        char *email;
        char *password;
        char *is_admin;
        bool admin;
        printf("enter uid > ");
        scanf("%d", &uid);
        fflush(stdin);
        printf("enter name > ");
        cin.getline(name, 40);
        fflush(stdin);
        printf("enter email > ");
        cin.getline(email, 40);
        fflush(stdin);
        if (get_user(email) != nullptr) {
            dutelab::fatal("User exists.");
            return;
        }
        password = getpass("password (hidden)> ");
        password = encrypt(password);
        printf("is it an admin? (yes to yes, other to no) > ");
        cin.getline(is_admin, 40);
        admin = !strcmp(is_admin, "yes");
        db_add_user(
                uid,
                name,
                email,
                password,
                admin
                );
    }
    void menu_admin_del_user(User *usr) {
        if (usr->permission_group != "admin") {
            dutelab::fatal("You don't have permission to access the menu.");
            return;
        }
        char *email = nullptr;
        printf("type email to remove > ");
        scanf("%s", email);
        if (get_user(email) == nullptr) {
            dutelab::fatal("No match user found.");
            return;
        }
        db_del_user(email);
    }
    bool section(int selected_section, string email) {
        auto usr = get_user(email);
        switch (selected_section) {
            case 1:
                menu_query_book();
                return false;
            case 2:
                menu_borrow_book(usr);
                return false;
            case 3:
                menu_return_book(usr);
                return false;
            case 4:
                menu_show_usr_info(usr);
                return false;
            case 11:
                menu_admin_reg_book(usr);
                return false;
            case 12:
                menu_admin_add_book(usr);
                return false;
            case 13:
                menu_admin_del_book(usr);
                return false;
            case 14:
                menu_admin_add_user(usr);
                return false;
            case 15:
                menu_admin_del_user(usr);
                return false;
            case -1:
                std::cout << "Bye" << endl;
                return true;
            default:
                std::cout << "No matches" << endl;
                return false;
        }
    }
}
