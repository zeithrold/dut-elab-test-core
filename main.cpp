#include <iostream>
#include "io.h"
#include "database.h"
#define PRINT_STAR std::cout << "******************************************" << std::endl

using namespace std;



int main() {
    std::cout << "Hello, World!" << std::endl;
    dutelab::open_database();
    bool is_interrupt = false;
    dutelab::start();
    while(true) {
        // LOGIN PHASE
        string email = dutelab::login();
        if (email.empty()) {
            continue;
        }
        if (email == "exit") {
            break;
        }
        while (!is_interrupt) {
            int selected_section = dutelab::menu(email);
            is_interrupt = dutelab::section(selected_section, email);
        }
        break;
    }
    dutelab::close_database();
}




