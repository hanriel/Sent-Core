//
// Created by Данил Федосеев on 21.02.2021.
//

#ifndef SENT_CORE_USER_H
#define SENT_CORE_USER_H

#include <string>

struct User {
public:
    explicit User(const std::basic_string<char>& basicString) {
        std::strcpy(this->username, basicString.c_str());
    }

    char username[64]{};
};

#endif //SENT_CORE_USER_H
