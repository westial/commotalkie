//
// Created by jaume on 1/13/21.
//

#include <stdlib.h>
#include <string.h>
#include "MessageCrypter.h"

unsigned char salt[MESSAGE_LENGTH];

static void xor(const unsigned char* first, const unsigned char* second, char* result, unsigned long size) {
  for(unsigned int i = 0; i < size; ++ i)
    result[i] = (char)(first[i] ^ second[i]);
}

static void createSalt(const char* original, unsigned char* cleaned, const unsigned int length) {
  unsigned int index = length;
  while (index--) cleaned[index] = '\0';
  index = 0;
  while ('\0' != original[index] && length > ++index) {
      cleaned[index] = original[index];
  }
}

void MessageCrypter_Create(const char* originalSalt) {
  createSalt(originalSalt, salt, MESSAGE_LENGTH);
};

void MessageCrypter_Destroy(void) {
}

void MessageCrypter_Encrypt(const Message* message, char* encrypted) {
  xor((const unsigned char*)message, salt, encrypted, MESSAGE_LENGTH);
  memcpy(encrypted, message->meta, MESSAGE_META_LENGTH);
}

void MessageCrypter_Decrypt(const char* raw, Message* decrypted) {
  xor((const unsigned char*)raw, salt, (char*)decrypted, MESSAGE_LENGTH);
  memcpy(decrypted, (const unsigned char*)raw, MESSAGE_META_LENGTH);
}