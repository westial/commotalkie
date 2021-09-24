//
// Created by jaume on 1/28/21.
//

#include <string.h>
#include "MessageValidator.h"
#include "MessageSubscriber.h"
#include "MessageCrypter.h"

unsigned long countdown_millis;

void Pull_Create(
    const char *salt,
    const char *topic,
    const void *pull_function,
    const void *epoch_function,
    const unsigned long timeout_millis
    ) {
  MessageCrypter_Create(salt);
  MessageSubscriber_Create(pull_function, epoch_function, timeout_millis, topic);
  countdown_millis = timeout_millis;
}

Result Pull_Invoke(
    unsigned char *port,
    unsigned char *id,
    unsigned char *body) {
  Result result;
  Message message;
  Message decrypted;
  if (0 != countdown_millis) MessageSubscriber_CountDown(countdown_millis);
  result = MessageSubscriber_Pull(&message);
  if (Success != result) return result;
  if (!MessageValidator_Check(&message)) return NotValid;

  MessageCrypter_Decrypt((const char*) &message, &decrypted);
  *port = decrypted.meta[PORT_INDEX];
  *id = decrypted.meta[ID_INDEX];
  memcpy(body, decrypted.body, MESSAGE_BODY_LENGTH);
  return result;
}

void Pull_Destroy() {
  MessageCrypter_Destroy();
  MessageSubscriber_Destroy();
}