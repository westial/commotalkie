//
// Created by jaume on 1/28/21.
//

#include <string.h>
#include "MessageValidator.h"
#include "MessageSubscriber.h"
#include "MessageCrypter.h"

static unsigned long countdown_millis;
static const unsigned char *exclusive_id = 0;

void handle_countdown();
void selective_pull(const char *topic,
                    Message *message,
                    Result *result,
                    Message *decrypted);
void decrypt(Message *, Message *);
void parse(Message *, unsigned char *, unsigned char *, unsigned char *);
void parse_id(const Message *, unsigned char *);
int validate(const Message *, const unsigned char *);

void Pull_Create(
    const char *salt,
    const void *pull_function,
    const void *epoch_function,
    const unsigned long timeout_millis,
    const unsigned char *to_id) {
  MessageCrypter_Create(salt);
  MessageSubscriber_Create(pull_function, epoch_function);
  countdown_millis = timeout_millis;
  exclusive_id = to_id;
}

Result Pull_Invoke(
    const char *topic,
    unsigned char *port,
    unsigned char *id,
    char *body) {
  Result result;
  Message message;
  Message decrypted;
  handle_countdown();
  selective_pull(topic, &message, &result, &decrypted);
  if (Success == result) {
    parse(&decrypted, port, id, body);
    return result;
  }
  return result;
}

void Pull_Destroy() {
  MessageCrypter_Destroy();
  MessageSubscriber_Destroy();
  exclusive_id = 0;
}

int check_id_constraint(const unsigned char *id) {
  return 0 == exclusive_id || *exclusive_id == *id;
}

int validate(const Message *message, const unsigned char *id) {
  return MessageValidator_Check(message) && check_id_constraint(id);
}

void decrypt(Message *input, Message *output) {
  MessageCrypter_Decrypt((const char *) input, output);
}

void parse_id(const Message *message, unsigned char *id) {
  *id = message->meta[ID_INDEX];
}

void parse(
    Message *output,
    unsigned char *port,
    unsigned char *id,
    unsigned char *body) {
  *port = output->meta[PORT_INDEX];
  parse_id(output, id);
  memcpy(body, output->body, MESSAGE_BODY_LENGTH);
}

void handle_countdown() {
  if (0 != countdown_millis) MessageSubscriber_CountDown(countdown_millis);
}

void selective_pull(
    const char *topic,
    Message *message,
    Result *result,
    Message *decrypted) {
  unsigned char id;
  do {
    *result = MessageSubscriber_Pull(topic, message);
    if (Success != *result) break;
    decrypt(message, decrypted);
    parse_id(decrypted, &id);
  } while (!validate(message, &id));
}