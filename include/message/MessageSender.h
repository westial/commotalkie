//
// Created by jaume on 1/14/21.
//

#ifndef COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESENDER_H_
#define COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESENDER_H_

#include "Message.h"
#include "Sender.h"

void MessageSender_Create(const void *send_function, const char* address);

int MessageSender_Send(Message *message);

void MessageSender_Destroy();

#endif //COMMOTALKIE_INCLUDE_MESSAGE_MESSAGESENDER_H_
