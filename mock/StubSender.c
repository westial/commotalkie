//
// Created by jaume on 1/15/21.
//

#include <stdlib.h>
#include "Sender.h"

static unsigned long (*send)(const char* address, const char* content, const unsigned long size) = 0;

void Sender_Create(const void *send_function) {
  send = (unsigned long (*)(const char*, const char*, const unsigned long)) send_function;
}

unsigned long Sender_Send(const char* address, const char* content, unsigned long size) {
  return send(address, content, size);
}

void Sender_Destroy(void) {
}

