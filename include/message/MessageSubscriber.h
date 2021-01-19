//
// Created by jaume on 1/19/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_

#include "Message.h"

void MessageSubscriber_Create(const void *pull_function, const char *topic);

int MessageSubscriber_Pull();

void MessageSubscriber_Destroy();

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESUBSCRIBER_H_
