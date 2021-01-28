//
// Created by jaume on 1/26/21.
//
#include "MessageCrypter.h"
#include "MessagePublisher.h"
#include "MessageFormatter.h"
#include "MessageValidator.h"
#include <string.h>

void PublishEncryptedMessage_Create(const char* topic, const char* salt, const void* push_fn) {
  MessageCrypter_Create(salt);
  MessagePublisher_Create((const void *) push_fn, topic);
}

void PublishEncryptedMessage_Invoke(unsigned char port, unsigned char id, unsigned char* body) {
  unsigned char content[MESSAGE_LENGTH];
  Message message;
  char encrypted[MESSAGE_LENGTH];

  content[1] = port;
  content[2] = id;
  memcpy(content + 3, body, MESSAGE_BODY_LENGTH);

  MessageFormatter_Pack(content, &message);
  MessageValidator_Sign(&message);
  MessageCrypter_Encrypt(&message, encrypted);
  MessagePublisher_Push((Message *) encrypted);
}

void PublishEncryptedMessage_Destroy() {
  MessageCrypter_Destroy();
  MessagePublisher_Destroy();
}