//
// Created by jaume on 1/28/21.
//

#include <string.h>
#include "MessageValidator.h"
#include "MessageSubscriber.h"
#include "MessageCrypter.h"
#include "ApplicationConfig.h"

void ReadEncryptedMessage_Create(
    const char *salt,
    const char *topic,
    const void *pull_function) {
  MessageCrypter_Create(salt);
  MessageSubscriber_Create(pull_function, topic);
}

Result ReadEncryptedMessage_Invoke(
    unsigned char *port,
    unsigned char *id,
    unsigned char *body) {
  Result result;
  Message message;
  Message decrypted;

  result = MessageSubscriber_Pull(&message);
  if (Success != result) return result;
  if (!MessageValidator_Check(&message)) return NotValid;

  MessageCrypter_Decrypt((const char*) &message, &decrypted);
  *port = decrypted.meta[PORT_INDEX];
  *id = decrypted.meta[ID_INDEX];
  memcpy(body, decrypted.body, MESSAGE_BODY_LENGTH);
  return result;
}

void ReadEncryptedMessage_Destroy() {
  MessageCrypter_Destroy();
  MessageSubscriber_Destroy();
}