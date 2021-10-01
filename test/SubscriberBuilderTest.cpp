#include <CppUTest/TestHarness.h>
#include "Spy.h"

extern "C" {
#include <cstring>
#include "Pull.h"
#include "Message.h"
#include "SubscriberBuilder.h"
#include <MessageFormatter.h>
#include <MessageValidator.h>
}

// -----------------------------------------------------------------------------

static struct Spy pull_fn_spy;
static int stub_message_fn(const char *, const char *, int);
static unsigned long fake_epoch_ms_fn();
static unsigned char port, id;
static unsigned char body[MESSAGE_BODY_LENGTH];
static int stub_pull_nothing_yet_fn(const char*, const char*, int);
static unsigned long nothing_until_zero;

// -----------------------------------------------------------------------------

int stub_message_fn(const char *address, const char *content, const int size) {
  pull_fn_spy.calledCount++;
  Message message;
  MessageFormatter_Pack("0123456789AB", &message);
  MessageValidator_Sign(&message);
  memcpy((void *)content, (void *)&message, MESSAGE_LENGTH);
  return size;
}

unsigned long fake_epoch_ms_fn() {
  return 100;
}

int stub_pull_nothing_yet_fn(const char* address, const char* content, const int size) {
  if (--nothing_until_zero) {
    pull_fn_spy.calledCount++;
    return 0;
  }
  else return stub_message_fn(address, content, size);
}

// -----------------------------------------------------------------------------

TEST_GROUP(SubscriberBuilder) {
  void setup() override {
    pull_fn_spy.calledCount = 0;
    port = 0;
    id = 0;
    memset(body, 0, MESSAGE_BODY_LENGTH);
    nothing_until_zero = 10000;
  }
};

TEST(SubscriberBuilder, BuiltSubscriberPullsForIdOnly) {
  const unsigned long timeout = 999;
  id = '2';
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetListenCallback(stub_message_fn);
  SubscriberBuilder_SetTimeService(fake_epoch_ms_fn);
  SubscriberBuilder_SetTimeout(&timeout);
  SubscriberBuilder_SetId(&id);
  CHECK_TRUE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
  Pull_Invoke("address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(1, pull_fn_spy.calledCount);
}

TEST(SubscriberBuilder, BuiltSubscriberPullsForAnyId) {
  const unsigned long timeout = 999;
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetListenCallback(stub_message_fn);
  SubscriberBuilder_SetTimeService(fake_epoch_ms_fn);
  SubscriberBuilder_SetTimeout(&timeout);
  CHECK_TRUE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
  Pull_Invoke("address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(1, pull_fn_spy.calledCount);
}

TEST(SubscriberBuilder, BuildASubscriberNoTimeout) {
  Result result;
  id = '2';
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetListenCallback(stub_pull_nothing_yet_fn);
  SubscriberBuilder_SetId(&id);
  CHECK_TRUE(SubscriberBuilder_Build());
  SubscriberBuilder_Destroy();
  result = Pull_Invoke("address", &port, &id, body);
  Pull_Destroy();
  CHECK_EQUAL(10000, pull_fn_spy.calledCount);
  CHECK_EQUAL(Success, result);
}

TEST(SubscriberBuilder, BuildWithMinimumRequired) {
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  SubscriberBuilder_SetListenCallback(stub_message_fn);
  CHECK_TRUE(SubscriberBuilder_Build());
}

TEST(SubscriberBuilder, SaltIsRequired) {
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("");
  SubscriberBuilder_SetListenCallback(stub_message_fn);
  CHECK_FALSE(SubscriberBuilder_Build());
}

TEST(SubscriberBuilder, ListenCallbackIsRequired) {
  SubscriberBuilder_Create();
  SubscriberBuilder_SetSalt("salt");
  CHECK_FALSE(SubscriberBuilder_Build());
}