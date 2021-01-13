//
// Created by jaume on  1/8/21.
//

#ifndef COMMOTALKIE_SRC_MESSAGE_MESSAGEFORMATTER_H_
#define COMMOTALKIE_SRC_MESSAGE_MESSAGEFORMATTER_H_

#include "Message.h"

void MessageFormatter_Pack(const void* input, Message* message);

void MessageFormatter_Unpack(const Message* message, void* output);

#endif //COMMOTALKIE_SRC_MESSAGE_MESSAGEFORMATTER_H_
