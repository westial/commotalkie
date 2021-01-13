//
// Created by jaume on  1/8/21.
//

#ifndef COMMOTALKIE_SRC_MESSAGE_MESSAGEFORMATTER_H_
#define COMMOTALKIE_SRC_MESSAGE_MESSAGEFORMATTER_H_

#include "Message.h"

Message MessageFormatter_Pack(const void* input);

void MessageFormatter_Unpack(Message, void* output);

#endif //COMMOTALKIE_SRC_MESSAGE_MESSAGEFORMATTER_H_
