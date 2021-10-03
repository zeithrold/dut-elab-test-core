//
// Created by Zeithrold on 2021/10/1.
//
#include <iostream>


#ifndef DUT_ELAB_TEST_CORE_BOOK_H
#define DUT_ELAB_TEST_CORE_BOOK_H

using namespace std;

namespace dut_elab {
    class Book {
    public:
        int book_id;
        string name;
        string isbn;
        string publisher;
        string authors[];
        long registered_at;
        string registered_by;
        int max_amount;
    };
}

#endif //DUT_ELAB_TEST_CORE_BOOK_H
