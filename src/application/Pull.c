//
// Created by jaume on 1/28/21.
//

#include <string.h>
#include "MessageValidator.h"
#include "MessageSubscriber.h"
#include "MessageCrypter.h"

unsigned long countdown_millis;
unsigned char strict_id;

void handle_countdown();
void pull(Message*, Result*);
void decrypt(Message*, Message*);
void parse(Message*, unsigned char*, unsigned char*, unsigned char*);

void decrypt(Message* input, Message* output) {
  MessageCrypter_Decrypt((const char*) input, output);
}

void parse(
    Message* output,
    unsigned char *port,
    unsigned char *id,
    unsigned char *body) {
  *port = output->meta[PORT_INDEX];
  *id = output->meta[ID_INDEX];
  memcpy(body, output->body, MESSAGE_BODY_LENGTH);
}

void Pull_Create(
    const char *salt,
    const char *topic,
    const void *pull_function,
    const void *epoch_function,
    const unsigned long timeout_millis
    ) {
  MessageCrypter_Create(salt);
  MessageSubscriber_Create(pull_function, epoch_function, topic);
  countdown_millis = timeout_millis;
  strict_id = 0;
}

void handle_countdown() {
  if (0 != countdown_millis) MessageSubscriber_CountDown(countdown_millis);
}

void pull(Message* message, Result* result) {
  do {
    *result = MessageSubscriber_Pull(message);
    if (Success != *result) break;
  } while (!MessageValidator_Check(message));
}

Result Pull_Invoke(
    unsigned char *port,
    unsigned char *id,
    unsigned char *body) {
  Result result;
  Message message;
  Message decrypted;
  handle_countdown();
  pull(&message, &result);
  if (Success == result) {
    decrypt(&message, &decrypted);
    parse(&decrypted, port, id, body);
    return result;
  }
  return result;
}

void Pull_Destroy() {
  MessageCrypter_Destroy();
  MessageSubscriber_Destroy();
}