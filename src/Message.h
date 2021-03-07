//
// Created by Данил Федосеев on 21.02.2021.
//

#ifndef SENT_CORE_MESSAGE_H
#define SENT_CORE_MESSAGE_H


struct Message {
public:
    char username[64] {};
    char msg[1024];
    bool isLast;
};


#endif //SENT_CORE_MESSAGE_H
