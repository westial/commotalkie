//
// Created by jaume on 1/18/21.
//
#include "Message.h"
#include "Sender.h"

static const char* address;

void MessagePublisher_Create(const void *send_function, const char* address_) {
  Sender_Create(send_function);
  address = address_;
}

int MessagePublisher_Send(Message *message) {
  if (MESSAGE_LENGTH == Sender_Send(address, (const char*) message, MESSAGE_LENGTH))
    return 1;
  return 0;
}

void MessagePublisher_Destroy() {}