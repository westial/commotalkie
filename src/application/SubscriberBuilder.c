//
// Created by jaume on 9/30/21.
//

#include "Pull.h"
static const char *subscriber_salt = 0;
static int (*subscriber_listen)(const unsigned char *, char *, const unsigned long) = 0;
static unsigned long (*subscriber_timer)(void) = 0;
static const unsigned long *subscriber_timeout_ms = 0;
static const unsigned char *subscriber_id = 0;

static void (*turn_on)() = 0;
static void (*turn_off)() = 0;

void SubscriberBuilder_Create() {}

void SubscriberBuilder_SetReceiverStateCallback(void (*turn_on_)(),
                                                void (*turn_off_)()) {
  turn_on = turn_on_;
  turn_off = turn_off_;
}

void SubscriberBuilder_SetSalt(const char *salt) { subscriber_salt = salt; }

void SubscriberBuilder_SetListenCallback(
    int (*listen_fn)(const unsigned char *address, char *content, const unsigned long size)) {
  subscriber_listen = listen_fn;
}

void SubscriberBuilder_SetTimeService(unsigned long (*service)(void)) {
  subscriber_timer = service;
}

void SubscriberBuilder_SetTimeout(const unsigned long *milliseconds) {
  subscriber_timeout_ms = milliseconds;
}

void SubscriberBuilder_SetId(const unsigned char *id) { subscriber_id = id; }

int SubscriberBuilder_Build() {
  if (0 == subscriber_salt || '\0' == subscriber_salt[0] ||
      0 == subscriber_listen || 0 == turn_on || 0 == turn_off)
    return 0;
  Pull_Create(subscriber_salt, (void *)subscriber_listen,
              (void *)subscriber_timer, (void *)turn_on, (void *)turn_off,
              (0 == subscriber_timer) ? 0 : *subscriber_timeout_ms,
              subscriber_id);
  return 1;
}

void SubscriberBuilder_Destroy() {
  subscriber_salt = 0;
  subscriber_listen = 0;
  subscriber_timer = 0;
  subscriber_timeout_ms = 0;
  subscriber_id = 0;
  turn_on = 0;
  turn_off = 0;
}
