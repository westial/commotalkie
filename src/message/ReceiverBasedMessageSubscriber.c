//
// Created by jaume on 1/20/21.
//
#include "Message.h"
#include "Receiver.h"
#include "MessageSubscriber.h"

static const char* address;

void MessageSubscriber_Create(const void *pull_function, const char *topic) {
  Receiver_Create(pull_function, MESSAGE_LENGTH);
  address = topic;
}

Result MessageSubscriber_Pull(const Message *message) {
  int available = 0;
  while(0 == available) {
    available = Receiver_listen(address);
  }
  if (0 > available) return IOError;
  Receiver_read((const char *)message);
  return Success;
}

void MessageSubscriber_Destroy() {}