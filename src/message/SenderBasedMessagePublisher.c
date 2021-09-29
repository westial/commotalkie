//
// Created by jaume on 1/18/21.
//
#include "Message.h"
#include "Sender.h"

void MessagePublisher_Create2(const void *push_function) {
  Sender_Create(push_function);
}

int MessagePublisher_Push(const char* topic, Message *message) {
  if (MESSAGE_LENGTH == Sender_Send(topic, (const char*) message, MESSAGE_LENGTH))
    return 1;
  return 0;
}

void MessagePublisher_Destroy() {
  Sender_Destroy();
}