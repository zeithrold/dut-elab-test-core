//
// Created by Zeithrold on 2021/10/1.
//
#include <iostream>
#include "database.h"

#ifndef DUT_ELAB_TEST_CORE_PERMISSION_H
#define DUT_ELAB_TEST_CORE_PERMISSION_H

#define ELAB_PERM_LOGIN 1001
#define ELAB_PERM_QUERY 1002
#define ELAB_PERM_LEND_BOOK 1003
#define ELAB_PERM_EXTEND_DDL 1004

#define ELAB_PERM_ADMIN_REG_BOOK 1101
#define ELAB_PERM_ADMIN_ADD_BOOK 1102
#define ELAB_PERM_ADMIN_REMOVE_BOOK 1103
#define ELAB_PERM_ADMIN_ADD_USER 1104
#define ELAB_PERM_ADMIN_REMOVE_USER 1105

using namespace std;
namespace DutElab
{
    class PermissionGroup
    {
    public:
        int pid;
        string name;
        int permissions[];
    };
}

#endif //DUT_ELAB_TEST_CORE_PERMISSION_H
