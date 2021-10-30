

#include "Sender.h"

static unsigned long (*send)(const unsigned char* address, const char* content, const unsigned long size) = 0;

void Sender_Create(const void *send_function) {
  send = (unsigned long (*)(const unsigned char*, const char*, const unsigned long)) send_function;
}

unsigned long Sender_Send(const unsigned char* address, const char* content, unsigned long size) {
  return send(address, content, size);
}

void Sender_Destroy(void) { send = 0;
}

