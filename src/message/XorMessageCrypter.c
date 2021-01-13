//
// Created by jaume on 1/13/21.
//

#include "MessageCrypter.h"

static void xor(const unsigned char* first, const unsigned char* second, char* result) {
  for(unsigned int i = 0; i < MESSAGE_LENGTH; ++ i)
    result[i] = (char)(first[i] ^ second[i]);
}

static void cleanSalt(const char* original, unsigned char* cleaned, const unsigned int length) {
  unsigned int index = length;
  while (index--) cleaned[index] = '\0';
  index = 0;
  while ('\0' != original[index] && length > ++index) {
      cleaned[index] = original[index];
  }
}

void MessageCrypter_Encrypt(
    const Message* message,
    const char* salt,
    char* encrypted) {
  unsigned char cleaned[MESSAGE_LENGTH];
  cleanSalt(salt, cleaned, MESSAGE_LENGTH);
  xor((const unsigned char*)message, cleaned, encrypted);
}

void MessageCrypter_Decrypt(
    const char* raw,
    const char* salt,
    Message* decrypted) {
  unsigned char cleaned[MESSAGE_LENGTH];
  cleanSalt(salt, cleaned, MESSAGE_LENGTH);
  xor((const unsigned char*)raw, cleaned, (char*)decrypted);
}