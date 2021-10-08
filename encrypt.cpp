//
// Created by Zeithrold on 2021/10/8.
//

#include "encrypt.h"
#include <string>

using namespace std;

namespace dutelab {
    char* encrypt(char* origin) {
        int length = strlen(origin);
        char result[length + 1];
        for (int i = 0; i < length; i++) {
            result[i] = origin[i] + 10;
        }
        return result;
    }
    char* decrypt(char* origin) {
        int length = strlen(origin);
        char result[length + 1];
        for (int i = 0; i < length; i++) {
            result[i] = origin[i] - 10;
        }
        return result;
    }
}
