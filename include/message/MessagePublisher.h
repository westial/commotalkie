//
// Created by jaume on 1/14/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGEPUBLISHER_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGEPUBLISHER_H_

#include "Message.h"
#include "Sender.h"

void MessagePublisher_Create(const void *push_function);

int MessagePublisher_Push(const char* topic, Message *message);

void MessagePublisher_Destroy();

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGEPUBLISHER_H_
