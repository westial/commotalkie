//
// Created by jaume on 1/28/21.
//

#include "MessageCrypter.h"
#include "MessageSubscriber.h"
#include "MessageValidator.h"
#include <string.h>

static unsigned long countdown;
static const unsigned char *exclusive_id = 0;

void start_countdown();
void stop_countdown();
void pull(const unsigned char *topic, Message *message, Result *result,
          Message *decrypted);
void decrypt(Message *, Message *);
void parse(Message *output, unsigned char *port, unsigned char *id, char *body);
void parse_id(const Message *, unsigned char *);
int validate(const Message *, const unsigned char *);

void Pull_Create(const char *salt, const void *pull, const void *epoch,
                 const void *turn_on, const void *turn_off,
                 const unsigned long timeout_at, const unsigned char *to_id) {
  MessageCrypter_Create(salt);
  MessageSubscriber_Create(pull, epoch, turn_on, turn_off);
  countdown = timeout_at;
  exclusive_id = to_id;
}

Result Pull_Invoke(const unsigned char *topic, unsigned char *port,
                   unsigned char *id, unsigned char *body) {
  Result result;
  Message message;
  Message decrypted;
  start_countdown();
  pull(topic, &message, &result, &decrypted);
  stop_countdown();
  if (Success == result) {
    parse(&decrypted, port, id, (char *)body);
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
  MessageCrypter_Decrypt((const char *)input, output);
}

void parse_id(const Message *message, unsigned char *id) {
  *id = message->meta[ID_INDEX];
}

void parse(Message *output, unsigned char *port, unsigned char *id,
           char *body) {
  *port = output->meta[PORT_INDEX];
  parse_id(output, id);
  memcpy(body, output->body, MESSAGE_BODY_LENGTH);
}

void start_countdown() {
  if (0 != countdown)
    MessageSubscriber_StartCountDown(countdown);
}

void stop_countdown() {
  if (0 != countdown)
    MessageSubscriber_StopCountDown();
}

void pull(const unsigned char *topic, Message *message, Result *result,
          Message *decrypted) {
  unsigned char id;
  do {
    *result = MessageSubscriber_Pull(topic, message);
    if (Success != *result)
      break;
    decrypt(message, decrypted);
    parse_id(decrypted, &id);
  } while (!validate(message, &id));
}