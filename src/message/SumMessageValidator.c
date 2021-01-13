//
// Created by jaume on 1/13/21.
//
#include "MessageValidator.h"

static unsigned char sum(const unsigned char* raw, unsigned int length) {
  unsigned char total = '\0';
  while (length--) {
    total += *(raw + length - 1);
  }
  return total;
}

int MessageValidator_Check(const Message* message) {
  const unsigned char expected = message->meta[0];
  return expected == sum(message->body, sizeof(message->body) / sizeof(message->body[0]));
}

void MessageValidator_Sign(Message* message) {
  message->meta[0] = sum(message->body, sizeof(message->body) / sizeof(message->body[0]));
}