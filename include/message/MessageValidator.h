//
// Created by jaume on 1/13/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGEVALIDATOR_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGEVALIDATOR_H_

#include "Message.h"

int MessageValidator_Check(const Message* message);

void MessageValidator_Sign(Message* message);

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGEVALIDATOR_H_
