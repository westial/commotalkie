//
// Created by jaume on 1/13/21.
//

#include <stdlib.h>
#include "MessageCrypter.h"

static unsigned char salt[MESSAGE_LENGTH];

static void xor(const unsigned char* first, const unsigned char* second, char* result) {
  for(unsigned int i = 0; i < MESSAGE_LENGTH; ++ i)
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
  xor((const unsigned char*)message, salt, encrypted);
}

void MessageCrypter_Decrypt(const char* raw, Message* decrypted) {
  xor((const unsigned char*)raw, salt, (char*)decrypted);
}