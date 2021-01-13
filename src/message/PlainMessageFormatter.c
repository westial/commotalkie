//
// Created by jaume on 1/8/21.
//
#include <string.h>
#include "MessageFormatter.h"

Message MessageFormatter_Pack(const void* input) {
  Message message;
  memcpy(message.meta, input, 2);
  memcpy(message.body, input + 2, 10);
  return message;
}

void MessageFormatter_Unpack(Message message, void* output) {
  memcpy(output, &message, 12);
}
