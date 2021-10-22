//
// Created by jaume on 9/30/21.
//

#include "Pull.h"
static const char *subscriber_salt = 0;
static int (*subscriber_listen)(const char *, char *, const unsigned long) = 0;
static unsigned long (*subscriber_timer)(void) = 0;
static const unsigned long *subscriber_timeout_ms = 0;
static const unsigned char *subscriber_id = 0;

void SubscriberBuilder_Create() {
}

void SubscriberBuilder_SetSalt(const char *salt) {
  subscriber_salt = salt;
}

void SubscriberBuilder_SetListenCallback(
    int (*listen_fn)(const char *, char *, const unsigned long)) {
  subscriber_listen = listen_fn;
}

void SubscriberBuilder_SetTimeService(unsigned long (*service)(void)) {
  subscriber_timer = service;
}

void SubscriberBuilder_SetTimeout(const unsigned long *milliseconds) {
  subscriber_timeout_ms = milliseconds;
}

void SubscriberBuilder_SetId(const unsigned char *id) {
  subscriber_id = id;
}

int SubscriberBuilder_Build() {
  if (0 == subscriber_salt || '\0' == subscriber_salt[0] || 0 == subscriber_listen)
    return 0;
  Pull_Create(
      subscriber_salt,
      (void *) subscriber_listen,
      (void *) subscriber_timer,
      (0 == subscriber_timer) ? 0 : *subscriber_timeout_ms,
      subscriber_id
  );
  return 1;
}

void SubscriberBuilder_Destroy() {
  subscriber_salt = 0;
  subscriber_listen = 0;
  subscriber_timer = 0;
  subscriber_timeout_ms = 0;
  subscriber_id = 0;
}

