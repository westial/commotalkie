//
// Created by jaume on 1/19/21.
//
#include <stdlib.h>
#include <string.h>

static char* buffer = 0;
static int size = 0;
/**
 * Listen and get an event from the address.
 * @param address listening from
 * @param content recipient for the content
 * @param size maximum allowed buffer size
 * @return int number of bytes received or -1 in case of error. Returns 0 if
 * nothing.
 */
static int (*listen)(const char* address, const char* content, const unsigned long size) = 0;

void Receiver_Create(const void *listen_function, int buffer_size) {
  listen = (int (*)(const char*, const char*, const unsigned long)) listen_function;
  buffer = malloc(buffer_size);
  size = buffer_size;
}

void Receiver_read(char* content) {
  memcpy(content, buffer, size);
}

int Receiver_listen(const char* address) {
  return listen(address, buffer, size);
}

void Receiver_Destroy(void) {
  if (0 != buffer)
    free(buffer);
  listen = 0;
}