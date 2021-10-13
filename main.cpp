#include <iostream>
#include "io.h"
#include "database.h"
#define PRINT_STAR std::cout << "******************************************" << std::endl

using namespace std;



int main() {
    std::cout << "Hello, World!" << std::endl;
    dutelab::open_database();
    while(true) {
        // LOGIN PHASE
        dutelab::start();
        string email = dutelab::login();
        if (email.empty()) {
            continue;
        }
        if (email == "exit") {
            break;
        }
        int selected_section = dutelab::menu(email);
        dutelab::section(selected_section, email);
    }
    dutelab::close_database();
}




