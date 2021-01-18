//
// Created by jaume on 1/14/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESENDER_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESENDER_H_

#include "Message.h"
#include "Sender.h"

void MessagePublisher_Create(const void *send_function, const char* address);

int MessagePublisher_Send(Message *message);

void MessagePublisher_Destroy();

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESENDER_H_
