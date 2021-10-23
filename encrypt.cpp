//
// Created by Zeithrold on 2021/10/8.
//

#include "encrypt.h"
#include <cstring>
#include <sstream>

using namespace std;

namespace dutelab {
    char* encrypt(char* origin) {
        unsigned long length = strlen(origin);
        stringstream stream;
        string result;
        for (int i = 0; i < length; i++) {
            stream << (char)(origin[i] + 5);
        }
        stream >> result;
        return (char *)result.data();
    }
    char* decrypt(char* origin) {
        int length = strlen(origin);
        char result[length + 1];
        for (int i = 0; i < length; i++) {
            result[i] = origin[i] - 5;
        }
        return result;
    }
}
