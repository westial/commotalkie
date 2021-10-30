//
// Created by jaume on 1/26/21.
//
#include "MessageCrypter.h"
#include "MessagePublisher.h"
#include "MessageFormatter.h"
#include "MessageValidator.h"
#include <string.h>

void Publish_Create(
    const char *salt,
    const void *push_fn) {
  MessageCrypter_Create(salt);
  MessagePublisher_Create((const void *) push_fn);
}

void Publish_Invoke(
    const unsigned char *topic,
    const unsigned char port,
    const unsigned char id,
    const unsigned char *body) {
  unsigned char content[MESSAGE_LENGTH];
  Message message;
  char encrypted[MESSAGE_LENGTH];

  content[PORT_INDEX] = port;
  content[ID_INDEX] = id;
  memcpy(content + MESSAGE_META_LENGTH, body, MESSAGE_BODY_LENGTH);

  MessageFormatter_Pack(content, &message);
  MessageCrypter_Encrypt(&message, encrypted);
  MessageValidator_Sign((Message *) encrypted);
  MessagePublisher_Push(topic, (Message *)encrypted);
}

void Publish_Destroy() {
  MessageCrypter_Destroy();
  MessagePublisher_Destroy();
}