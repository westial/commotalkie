//
// Created by jaume on 1/18/21.
//
#include "Message.h"
#include "Sender.h"

static const char* address;

void MessagePublisher_Create(const void *push_function, const char* topic) {
  Sender_Create(push_function);
  address = topic;
}

int MessagePublisher_Push(Message *message) {
  if (MESSAGE_LENGTH == Sender_Send(address, (const char*) message, MESSAGE_LENGTH))
    return 1;
  return 0;
}

void MessagePublisher_Destroy() {}