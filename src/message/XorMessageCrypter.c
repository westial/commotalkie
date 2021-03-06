#include "MessageCrypter.h"
#include <string.h>

static unsigned char salt[MESSAGE_LENGTH];

static void xor
    (const unsigned char *first, const unsigned char *second,
     unsigned char *result, unsigned long size) {
      for (unsigned int i = 0; i < size; ++i)
        result[i] = (first[i] ^ second[i]);
    }

    static void createSalt(const char *original, unsigned char *cleaned,
                           const unsigned int length) {
  unsigned int index = length;
  while (index--)
    cleaned[index] = '\0';
  index = 0;
  while ('\0' != original[index] && length >= index) {
    cleaned[index] = original[index];
    ++index;
  }
}

void MessageCrypter_Create(const char *originalSalt) {
  createSalt(originalSalt, salt, MESSAGE_BODY_LENGTH);
};

void MessageCrypter_Destroy(void) {}

void MessageCrypter_Encrypt(const Message *message,
                               unsigned char *encrypted) {
  xor((const unsigned char *)message + MESSAGE_META_LENGTH, salt,
      encrypted + MESSAGE_META_LENGTH, MESSAGE_BODY_LENGTH);
  memcpy(encrypted, message->meta, MESSAGE_META_LENGTH);
}

void MessageCrypter_Decrypt(const unsigned char *raw, Message *decrypted) {
  xor(raw + MESSAGE_META_LENGTH, salt,
      (unsigned char *)decrypted + MESSAGE_META_LENGTH, MESSAGE_BODY_LENGTH);
  memcpy(decrypted, raw, MESSAGE_META_LENGTH);
}